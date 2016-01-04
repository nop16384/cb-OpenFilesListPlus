/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin.hh"
#include    "oflp-panel.hh"
#include    "oflp-settings.hh"
//  ............................................................................
#include    "oflp-plugin-module.hh"
#include    "oflp-plugin-mod-gfx.hh"
#include    "oflp-plugin-mod-layout.hh"
#include    "oflp-plugin-mod-editors.hh"
#include    "oflp-plugin-mod-panels.hh"
#include    "oflp-plugin-mod-settings.hh"
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
void    OflpModule::init()
{
    a_instance              =   OpenFilesListPlus::Instance();

    a_module_gfx            =   oflp()->gfx();
    a_module_layout         =   oflp()->layout();
    a_module_panels         =   oflp()->panels();
    a_module_editors        =   oflp()->editors();
    a_module_settings       =   oflp()->settings();
}
//  ............................................................................
namespace
{
    // this auto-registers the plugin
    PluginRegistrant<OpenFilesListPlus> reg(_T("OpenFilesListPlus"));

    const int idViewOpenFilesPlus       = wxNewId();
}
//  ............................................................................
BEGIN_EVENT_TABLE(OpenFilesListPlus, cbPlugin)
    EVT_UPDATE_UI                   (idViewOpenFilesPlus, OpenFilesListPlus::evh_update_ui)
    EVT_MENU                        (idViewOpenFilesPlus, OpenFilesListPlus::evh_view_open_files_plus)
END_EVENT_TABLE()
//  ............................................................................
void    OpenFilesListPlus:: dump_project_manager_state()
{
    OFLP_FUNC_ENTER_MARK("p0_dump_project_manager()");

    ProjectManager* pjm = Manager::Get()->GetProjectManager();

    GWR_TKI("  Workspace loading:[%i]", pjm->IsLoadingWorkspace()   );
    GWR_TKI("  Workspace closing:[%i]", pjm->IsClosingWorkspace()   );
    GWR_TKI("  Project   loading:[%i]", pjm->IsLoadingProject()     );
    GWR_TKI("  Project   closing:[%i]", pjm->IsClosingProject()     );
    GWR_TKI("  Project   l/c    :[%i]", pjm->IsLoadingOrClosing()   );
}
//  ............................................................................
// constructor
OpenFilesListPlus::     OpenFilesListPlus()
{
    a_mode_degraded     =   false;

    a_dnd_panel_src     =   NULL;
    a_dnd_panel_dst     =   NULL;
    a_dnd_editor_base   =   NULL;

    d_gfx               =   NULL;
    d_layout            =   NULL;
    d_editors           =   NULL;
    d_panels            =   NULL;
    d_settings          =   NULL;
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
    //  ........................................................................
    //  this is for debugging only : enable log window at very startup of plugin
    oflp::A_log_window    =   true;                                       //  enable log window at start
    oflp::A_log_console   =   false;
    oflp::Log_window_open( Manager::Get()->GetAppWindow() );              //  enable log window at start

    GWR_INF("OpenFilesListPlugin::OnAttach [%p][%p]", this, Instance());
    //  ........................................................................
    //  create & init the modules
    d_gfx           =   new OflpModGfx(ConfigManager::GetDataFolder() + wxS("/images/"));
    d_layout        =   new OflpModLayout();
    d_editors       =   new OflpModEditors();
    d_panels        =   new OflpModPanels();
    d_settings      =   new OflpModSettings();

    //  once all modules are created, init them with pointers to each others
    gfx()           ->OflpModule::init();
    layout()        ->OflpModule::init();
    editors()       ->OflpModule::init();
    panels()        ->OflpModule::init();
    settings()      ->OflpModule::init();
    //  ........................................................................
    //  create main wxPanel & its sizer, create "bulk" panel
    panels()->init();
    //  ........................................................................
    // add the tree to the docking system
    #ifndef MEMLEAKS
    CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
    evt.name        = _T("OpenFilesListPlusPane");
    evt.title       = _("Open files list plus");
    evt.pWindow     = panels()->p0_main();
    evt.minimumSize.Set(50, 50);
    evt.desiredSize.Set(150, 100);
    evt.floatingSize.Set(100, 150);
    evt.dockSide    = CodeBlocksDockEvent::dsRight;
    evt.stretch     = true;
    Manager::Get()->ProcessEvent(evt);
    #endif
    //  ........................................................................
    // register event sinks
    Manager* pm = Manager::Get();

    //  workspace related, for layout
    pm->RegisterEventSink(cbEVT_WORKSPACE_LOADING_COMPLETE  , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_workspace_loading_complete   ));
    pm->RegisterEventSink(cbEVT_WORKSPACE_CLOSING_BEGIN     , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_workspace_close_begin        ));
    pm->RegisterEventSink(cbEVT_WORKSPACE_CLOSING_COMPLETE  , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_workspace_close_complete     ));

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
    #ifndef MEMLEAKS
    CodeBlocksDockEvent evt(cbEVT_REMOVE_DOCK_WINDOW);
    evt.pWindow = panels()->p0_main();
    Manager::Get()->ProcessEvent(evt);
    #endif

    // finally destroy the widgets
    oflp::Log_window_close();

    panels()->p0_main()->Destroy();
    panels()->p0_main() = NULL;
}

int OpenFilesListPlus:: Configure()
{
    #ifndef MEMLEAKS
    //create and display the configuration dialog for your plugin
    cbConfigurationDialog dlg(Manager::Get()->GetAppWindow(), wxID_ANY, _("Your dialog title"));
    cbConfigurationPanel* panel = GetConfigurationPanel(&dlg);
    if (panel)
    {
        dlg.AttachConfigurationPanel(panel);
        PlaceWindow(&dlg);
        return dlg.ShowModal() == wxID_OK ? 0 : -1;
    }
    #endif
    return -1;
}

void OpenFilesListPlus::BuildMenu(wxMenuBar* menuBar)
{
    #ifndef MEMLEAKS

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

    #endif
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
void OpenFilesListPlus::evh_view_open_files_plus    (wxCommandEvent& event)
{
    #ifndef MEMLEAKS
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = panels()->p0_main();
    Manager::Get()->ProcessEvent(evt);
    #endif
}

void OpenFilesListPlus::evh_update_ui               (wxUpdateUIEvent& event)
{
    if (aw_menu_view)
    {
        bool isVis = IsWindowReallyShown((wxWindow*)panels()->p0_main());
        aw_menu_view->Check(idViewOpenFilesPlus, isVis);
    }

    // must do...
    event.Skip();
}
//  ############################################################################
bool    OpenFilesListPlus:: FindCbProjectForFile    (wxString const & _abs_fpath, cbProject** _pro, ProjectFile** _pjf)
{
    *(_pro) = Manager::Get()->GetProjectManager()->FindProjectForFile(
        _abs_fpath      ,
        _pjf            ,
        false, false    );

    return ( (*_pro) != NULL );
}
//! \brief  Delete all OflpPanels, forget about the layout
void OpenFilesListPlus::reset()
{
    panels()->p0_reset();
    layout()->reset();
}

//! \brief  Emergency @ runtime
void OpenFilesListPlus::emergency()
{
    GWR_ERR( "%s", wxS("### EMERGENCY ###") );
    degrade();
}

//! \brief  Set OFLP in degraded mode
void OpenFilesListPlus::degrade()
{
    a_mode_degraded = true;

    wxColour c( 0xd1, 0xcd, 0xce );
    panels()->p0_set_bgs(c);
}

//! \brief  Tell if OFLP is in degraded mode
bool OpenFilesListPlus::degraded()
{
    return a_mode_degraded;
}

//! \fn     RefreshOpenFileState
//!
//! \brief  Given an editor :
//!       - if    editor in list  : ensure corresponding wxTreeItem is selected
//!         else                  : add a new wxTreeItem
//!       - verify wxTreeItem icon
//!       - verify wxTreeItem text
void OpenFilesListPlus::RefreshOpenFileState    (EditorBase* _nn_edb)
{
    EditorManager   *   emgr  =   Manager::Get()->GetEditorManager();
    EditorBase      *   aedb  =   emgr->GetActiveEditor();

    OflpPanel       *   panel   =   NULL;
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::RefreshOpenFileState()");
    //  ........................................................................
    if ( Manager::IsAppShuttingDown() )
        return;
    //  ........................................................................
    panels()->p0_main()->Freeze();
    //  ........................................................................
    wxString            shortname   = _nn_edb->GetShortName();
    panel                           = panels()->get(_nn_edb);

    GWR_TKI("     ...editor         :[%p][%s]", _nn_edb, shortname.wc_str());
    GWR_TKI("     ...panel          :[%p]"    , panel);
    GWR_TKI("     ...active editor  :[%s]"    , aedb ? aedb->GetShortName().wc_str() : wxS("NULL"));
    //  ........................................................................
    //  we found the editor in an OflpPanel
    if ( panel )
    {
        //  ....................................................................
        //  sync wxTreeItem infos - verify selection state

        //  sync wxTreeItem infos ( icon, name + eventually re-sorting )
        panel->editor_sync(_nn_edb);

        if ( _nn_edb == aedb )
        {
            if ( ! panel->editor_selected(_nn_edb) )
            {
                GWR_TKI("%s", wxS("      ...selecting"));
                panel->editor_select(_nn_edb);
            }
            else
            {
                GWR_TKI("%s", wxS("      ...not selecting (optimization)"));
            }
            panels()->p0_unselect_except(panel);
        }
    }
    //  ........................................................................
    //  editor was not found in any OflpPanel -> emergency
    else
    {
        //  emergency();                                                        //  _GWR_TODO_ "Start here" is not in any panel !
    }
    //  ........................................................................
    panels()->p0_main()->Thaw();
    //  ........................................................................
    OFLP_FUNC_EXIT_LOG();
}

//! \fn     RefreshOpenFileLayout
//!
//! \brief  Given an editor, put it in the right OflpPanel
void OpenFilesListPlus::RefreshOpenFileLayout   (EditorBase* _nn_edb)
{
    EditorManager   *   emgr        =   Manager::Get()->GetEditorManager();
    EditorBase      *   aedb        =   emgr->GetActiveEditor();
    wxString            shortname   =   _nn_edb->GetShortName();

    OflpPanel         *   psrc    =   NULL;
    OflpPanel         *   pdst    =   NULL;
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::RefreshOpenFileLayout()");
    //  ........................................................................
    if ( Manager::IsAppShuttingDown() )
        goto lab_exit;
    //  ........................................................................
    //  get infos on editor
    psrc                            = panels()->get(_nn_edb);
    if ( ! psrc )
    {
        GWR_TKE("      ...editor [%p][%s] was _NOT_ found in any panel", _nn_edb, shortname.wc_str());
        goto lab_exit;
    }
    GWR_TKI("      ...editor         :[%p][%s]", _nn_edb, shortname.wc_str());
    GWR_TKI("      ...panel          :[%p]"    , psrc);
    GWR_TKI("      ...active editor  :[%s]"    , aedb ? aedb->GetShortName().wc_str() : wxS("NULL"));
    //  ........................................................................
    //  find file assignment ; if found, move editor to its assigned panel
    //  if it lays in another panel.
    pdst = layout()->file_assignment_find_panel_from_editor_base(_nn_edb);

    //  no assignment - set dst to bulk
    if ( ! pdst )
    {
        GWR_TKI("%s", wxS("      ...not assigned"));
        pdst    =   panels()->p0_bulk();
    }
    //  ........................................................................
lab_eventually_move:
    //  move if different panels
    if ( psrc != pdst )
    {
        GWR_TKI("      ...moving editor to panel[%s]", pdst->title().wc_str());
        panels()->p0_editor_mov( pdst, psrc, _nn_edb );                         //  _GWR_TODO_ needs panels()::resize() call here !!!
    }
    //  ........................................................................
lab_exit:
    OFLP_FUNC_EXIT_LOG();
}

//! \fn     RefreshOpenFilesLayout
//!
//! \brief  RefreshOpenFileLayout() on all editors
void OpenFilesListPlus::RefreshOpenFilesLayout  ()
{
    EditorManager   *   emgr  =   Manager::Get()->GetEditorManager();
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::RefreshOpenFilesLayout()");
    //  ........................................................................
    //  optim : if workspace loading, first calls to RefreshOpenFileLayout()    //  _GWR_OPTIM_
    //  occurs _BEFORE_ panels are created. So if no panel exist, drop
    if ( layout()->panel_assignment_array().size() == 0 )
    {
        GWR_TKI("%s", wxS("      ...no panel is present - dropping"));
        goto lab_exit;
    }
    //  ........................................................................
    panels()->p0_main()->Freeze();

    for ( int i = 0 ; i != emgr->GetEditorsCount() ; i++ )
    {
        RefreshOpenFileLayout( emgr->GetEditor(i) );
    }

    panels()->resize_and_layout();

    if ( emgr->GetActiveEditor() )
        RefreshOpenFileState( emgr->GetActiveEditor() );
    else
    {
        GWR_TKE("%s", wxS("      ...editor is NULL"));
    }

    panels()->p0_main()->Thaw();
    //  ........................................................................
lab_exit:
    OFLP_FUNC_EXIT_LOG();
}
//  ############################################################################
//#include    "oflp-plugin-events.cci"
//#include    "oflp-plugin-mod-layout.cci"
//#include    "oflp-plugin-mod-editors.cci"
//#include    "oflp-plugin-mod-panels.cci"
//#include    "oflp-plugin-mod-settings.cci"

// dont create a "oflp-plugin-gfx.cci" file for 2 lines
BitmapPointerHash   OflpModGfx::A_bitmap_hash;
wxImageList         OflpModGfx::A_img_list(16, 16);



