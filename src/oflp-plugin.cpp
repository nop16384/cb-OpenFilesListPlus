#include "sdk.h"                                                                // Code::Blocks SDK
//  ............................................................................
#ifndef CB_PRECOMP
    #include "manager.h"
    #include "configmanager.h"
    #include "configurationpanel.h"
    #include "editormanager.h"
    #include "projectmanager.h"
    #include "logmanager.h"
    #include "editorbase.h"
    #include "cbeditor.h"
    #include "sdk_events.h"
    #include "misctreeitemdata.h"

    #include "cbworkspace.h"
    #include "cbproject.h"
    #include "projectmanager.h"
#endif
//  ............................................................................
#include    "oflp-plugin.hh"
#include    "oflp-panel.hh"

#include "tinyxml/tinyxml.h"
#include "tinyxml/tinywxuni.h"
//  ............................................................................
#define GWR_OFLP_SANITY_CHECKS
#define GWR_LOG(FORMAT, ...)    GWRCB_LOG(FORMAT, __VA_ARGS__);
#define GWR_TKI(FORMAT, ...)    GWRCB_TKI(FORMAT, __VA_ARGS__);
#define GWR_TKE(FORMAT, ...)    GWRCB_TKE(FORMAT, __VA_ARGS__);
#define GWR_INF(FORMAT, ...)    GWRCB_INF(FORMAT, __VA_ARGS__);
#define GWR_WNG(FORMAT, ...)    GWRCB_WNG(FORMAT, __VA_ARGS__);
#define GWR_ERR(FORMAT, ...)    GWRCB_ERR(FORMAT, __VA_ARGS__);
//  ............................................................................
OpenFilesListPlus   *   OpenFilesListPlus::s_singleton  =   NULL;
//  ............................................................................
namespace
{
    // this auto-registers the plugin
    PluginRegistrant<OpenFilesListPlus> reg(_T("OpenFilesListPlus"));

    const int idViewOpenFilesPlus       = wxNewId();
    const int idMainPanel               = wxNewId();
}
//  ............................................................................
BEGIN_EVENT_TABLE(OpenFilesListPlus, cbPlugin)
    EVT_UPDATE_UI                   (idViewOpenFilesPlus, OpenFilesListPlus::evh_update_ui)
    EVT_MENU                        (idViewOpenFilesPlus, OpenFilesListPlus::evh_view_open_files_plus)
END_EVENT_TABLE()
//  ............................................................................
// constructor
OpenFilesListPlus::     OpenFilesListPlus()
{
    d_gfx       =   NULL;
    d_layout    =   NULL;
    // Make sure our resources are available.
    // In the generated boilerplate code we have no resources but when
    // we add some, it will be nice that this code is in place already ;)
    if(!Manager::LoadResource(_T("OpenFilesListPlus.zip")))
    {
        NotifyMissingFile(_T("OpenFilesListPlus.zip"));
    }
}

// destructor
OpenFilesListPlus::    ~OpenFilesListPlus()
{
}

void OpenFilesListPlus::OnAttach()
{
    s_singleton             =   this;
    aw_menu_view            =   NULL;

    a_panels_array.Clear();
    //  ........................................................................
    earlgreycb::A_log_console   =   false;
    earlgreycb::A_log_window    =   true;                                       //  enable log window at start
    earlgreycb::Log_window_open( Manager::Get()->GetAppWindow() );              //  enable log window at start

    GWR_INF("OpenFilesListPlugin::OnAttach [%p][%p]", this, Instance());
    //  ........................................................................
    //  gfx
    d_gfx       =   new Gfx(ConfigManager::GetDataFolder() + wxS("/images/"));
    //  ........................................................................
    //  layout
    d_layout    =   new Layout();
    //  ........................................................................
    //  menu options
    dw_menu_options =   new MenuOptions();
    menu_options()->check_item_set_log_console  (false);
    menu_options()->check_item_set_log_window   (true);                         //  cf enables log window above

    menu_options()                      ->Connect(
        wxEVT_COMMAND_MENU_SELECTED                                         ,
        wxCommandEventHandler(OpenFilesListPlus::evh_menu_option_checked) ,
        NULL, this                                                          );
    menu_options()->m1mode()            ->Connect(
        wxEVT_COMMAND_MENU_SELECTED                                         ,
        wxCommandEventHandler(OpenFilesListPlus::evh_menu_option_checked) ,
        NULL, this                                                          );
    menu_options()->m1log()             ->Connect(
        wxEVT_COMMAND_MENU_SELECTED                                         ,
        wxCommandEventHandler(OpenFilesListPlus::evh_menu_option_checked) ,
        NULL, this                                                          );
    //  ........................................................................
    //  create main wxPanel & its sizer, "bulk" panel
    dw_MainPanel    =   new wxPanel(Manager::Get()->GetAppWindow(), idMainPanel);
    dw_MainPanel->SetFont( gfx()->fnt8() );
    dw_MainSizer    =   new wxBoxSizer(wxVERTICAL);
    a_BulkPanel     =   panel_add( wxString::FromUTF8("bulk"), true );

    dw_MainSizer->SetSizeHints(dw_MainPanel);
    dw_MainPanel->SetSizer(dw_MainSizer);
    //  ........................................................................
    // add the tree to the docking system
    CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
    evt.name        = _T("OpenFilesListPlusPane");
    evt.title       = _("Open files list plus");
    evt.pWindow     = dw_MainPanel;
    evt.minimumSize.Set(50, 50);
    evt.desiredSize.Set(150, 100);
    evt.floatingSize.Set(100, 150);
    evt.dockSide    = CodeBlocksDockEvent::dsLeft;
    evt.stretch     = true;
    Manager::Get()->ProcessEvent(evt);
    //  ........................................................................
    // register event sinks
    Manager* pm = Manager::Get();

    //  workspace related, for layout
    pm->RegisterEventSink(cbEVT_WORKSPACE_LOADING_COMPLETE  , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_workspace_loading_complete   ));
    pm->RegisterEventSink(cbEVT_WORKSPACE_CHANGED           , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_workspace_changed            ));

    //  projects related, for layout
    pm->RegisterEventSink(cbEVT_PROJECT_OPEN    , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_project_open     ));
    pm->RegisterEventSink(cbEVT_PROJECT_CLOSE   , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_project_close    ));
    pm->RegisterEventSink(cbEVT_PROJECT_SAVE    , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_project_save     ));

    // basically everything editor related
    pm->RegisterEventSink(cbEVT_EDITOR_OPEN     , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_editor_opened    ));
    pm->RegisterEventSink(cbEVT_EDITOR_CLOSE    , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_editor_closed    ));
    pm->RegisterEventSink(cbEVT_EDITOR_ACTIVATED, new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_editor_activated ));
    pm->RegisterEventSink(cbEVT_EDITOR_MODIFIED , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_editor_modified  ));
    pm->RegisterEventSink(cbEVT_EDITOR_SAVE     , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_editor_saved     ));
    //pm->RegisterEventSink(cbEVT_EDITOR_DEACTIVATED, new cbEventFunctor<OpenFilesListPlugin, CodeBlocksEvent>(this, &OpenFilesListPlugin::OnEditorDeactivated));
}

void OpenFilesListPlus::OnRelease(bool appShutDown)
{
    if (appShutDown)
        return;

    // remove registered event sinks
    Manager::Get()->RemoveAllEventSinksFor(this);

    // remove tree from docking system
    CodeBlocksDockEvent evt(cbEVT_REMOVE_DOCK_WINDOW);
    evt.pWindow = dw_MainPanel;
    Manager::Get()->ProcessEvent(evt);

    // finally destroy the widgets
    earlgreycb::Log_window_close();

    dw_MainPanel->Destroy();
    dw_MainPanel = nullptr;
}

int OpenFilesListPlus:: Configure()
{
    //create and display the configuration dialog for your plugin
    cbConfigurationDialog dlg(Manager::Get()->GetAppWindow(), wxID_ANY, _("Your dialog title"));
    cbConfigurationPanel* panel = GetConfigurationPanel(&dlg);
    if (panel)
    {
        dlg.AttachConfigurationPanel(panel);
        PlaceWindow(&dlg);
        return dlg.ShowModal() == wxID_OK ? 0 : -1;
    }
    return -1;
}

void OpenFilesListPlus::BuildMenu(wxMenuBar* menuBar)
{
    // if not attached, exit
    if (!IsAttached())
        return;

    // add the open files list in the "View" menu
    int idx = menuBar->FindMenu(_("&View"));
    if (idx == wxNOT_FOUND)
        return;

    aw_menu_view                = menuBar       ->GetMenu(idx);
    wxMenuItemList  &   items   = aw_menu_view  ->GetMenuItems();

    // find the first separator and insert before it
    for (size_t i = 0; i < items.GetCount(); ++i)
    {
        if (items[i]->IsSeparator())
        {
            aw_menu_view->InsertCheckItem(i, idViewOpenFilesPlus, _("&Open files list plus"), _("Toggle displaying the open files list"));
            return;
        }
    }
    // not found, just append
    aw_menu_view->AppendCheckItem(idViewOpenFilesPlus, _("&Open files list plus"), _("Toggle displaying the open files list"));
}

void OpenFilesListPlus::BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data)
{
    //Some library module is ready to display a pop-up menu.
    //Check the parameter \"type\" and see which module it is
    //and append any items you need in the menu...
    //TIP: for consistency, add a separator as the first item...
    NotImplemented(_T("OpenFilesListPlus::BuildModuleMenu()"));
}

bool OpenFilesListPlus::BuildToolBar(wxToolBar* toolBar)
{
    //The application is offering its toolbar for your plugin,
    //to add any toolbar items you want...
    //Append any items you need on the toolbar...
    NotImplemented(_T("OpenFilesListPlus::BuildToolBar()"));

    // return true if you add toolbar items
    return false;
}
//  ############################################################################
// view menu toggle tree
void OpenFilesListPlus::evh_view_open_files_plus  (wxCommandEvent& event)
{
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = dw_MainPanel;
    Manager::Get()->ProcessEvent(evt);
}

void OpenFilesListPlus::evh_update_ui             (wxUpdateUIEvent& event)
{
    if (aw_menu_view)
    {
        bool isVis = IsWindowReallyShown((wxWindow*)dw_MainPanel);
        aw_menu_view->Check(idViewOpenFilesPlus, isVis);
    }

    // must do...
    event.Skip();
}
//  ############################################################################
//! \brief  RefreshOpenFilesTree
//!
//! \detail Given an editor :
//!     if have to remove : remove it
//!     else
//!       - if    editor in list  : ensure corresponding wxTreeItem is selected
//!         else                  : add a new wxTreeItem
//!       - verify wxTreeItem icon
//!       - verify wxTreeItem text
void OpenFilesListPlus::RefreshOpenFilesTree    (EditorBase* ed, bool remove)
{
    OFLPPanel                       *   panel   =   NULL;
    wxTreeItemId                    iid;
    //  ........................................................................
    earlgreycb::Log_function_enter(wxS("OpenFilesListPlugin::RefreshOpenFilesTree()"));
    //  ........................................................................
    if ( Manager::IsAppShuttingDown() )
        return;

    if ( ! ed )
    {
        GWR_TKE("%s", wxS("RefreshOpenFilesTree():activated editor NULL"));
        return;
    }

    EditorManager   * mgr   =   Manager::Get()->GetEditorManager();
    EditorBase      * aed   =   mgr->GetActiveEditor();

    dw_MainPanel->Freeze();

    wxTreeItemIdValue   cookie      = 0;
    wxString            shortname   = ed->GetShortName();
    panel                           = panel_find(ed);

    GWR_TKI("RefreshOpenFilesTree():activated editor [%s]", shortname.wc_str());
    GWR_TKI("RefreshOpenFilesTree():active    editor [%s]", aed ? aed->GetShortName().wc_str() : wxS("NULL"));
    GWR_TKI("RefreshOpenFilesTree():panel            [%p]", panel);

    //  find panel from editor
    if ( panel )
    {
        if ( !remove )
        {
            //  ensure visual infos are correct ( icon, name + eventually re-sorting )
            panel->editor_sync(ed);

            //  ensure selected if it is the active one
            if ( ed == aed )
            {
                if ( ! panel->editor_selected(ed) )
                {
                    GWR_TKI("%s", wxS("RefreshOpenFilesTree():selecting"));
                    panel->editor_select(ed);
                }
                else
                {
                    GWR_TKI("%s", wxS("RefreshOpenFilesTree():not selecting - optimization"));
                }
                panels_unselect(panel);
            }
        }
        else
        {
            panel->editor_del(ed);
        }
    }
    else
    {
        // not found, not to-remove and valid name: add new editor
        if ( !remove && ed->VisibleToTree() && !shortname.IsEmpty())
        {
            a_BulkPanel->editor_add(ed);
        }
    }
    dw_MainPanel->Thaw();

    earlgreycb::Log_function_exit();
}
//  ############################################################################
#include    "oflp-plugin-panels.cci"
#include    "oflp-plugin-events.cci"
#include    "oflp-plugin-layout.cci"


