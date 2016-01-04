/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin.hh"

#include    "oflp-plugin-mod-layout.hh"
#include    "oflp-plugin-mod-panels.hh"
#include    "oflp-plugin-mod-settings.hh"
#include    "oflp-plugin-mod-editors.hh"

#include    "oflp-panel.hh"
//  ............................................................................
#define GWR_OFLP_SANITY_CHECKS
#define GWR_LOG(FORMAT, ...)    GWRCB_LOG(FORMAT, __VA_ARGS__);
#define GWR_TKI(FORMAT, ...)    GWRCB_TKI(FORMAT, __VA_ARGS__);
#define GWR_TKE(FORMAT, ...)    GWRCB_TKE(FORMAT, __VA_ARGS__);
#define GWR_INF(FORMAT, ...)    GWRCB_INF(FORMAT, __VA_ARGS__);
#define GWR_WNG(FORMAT, ...)    GWRCB_WNG(FORMAT, __VA_ARGS__);
#define GWR_ERR(FORMAT, ...)    GWRCB_ERR(FORMAT, __VA_ARGS__);
//  ############################################################################
//
//                          EDITOR EVENTS
//
//  ############################################################################
void    OpenFilesListPlus:: evh_workspace_loading_complete  (CodeBlocksEvent& _e)
{
    size_t                                          s   =   0;                  //  # of panels ( not including bulk )
    std::vector< OflpModLayout::PanelAssignment* >  v;
    //  ........................................................................
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::evh_workspace_loading_complete()");

    layout()->workspace_load();

    s = layout()->panel_assignment_array().size();
    v.resize(s);                                                                //  dimension the vector
    //  ........................................................................
    //  We sort PanelAssignments-by inserting them in a vector following
    //  their index.
    for ( OflpModLayout::PanelAssignmentArray::const_iterator
        it  =   layout()->panel_assignment_array().begin()  ;
        it  !=  layout()->panel_assignment_array().end()    ;
        it++                                                )
    {
        v[ (*it)->index() - 1 ] = (*it);
    }
    //  ........................................................................
    //  Now we can add them
    for ( size_t i = 0 ; i != s ; i++ )
    {
        panels()->p0_add( v[i]->name(), false );
    }

    RefreshOpenFilesLayout();

    panels()->resize_and_layout();

    OFLP_FUNC_EXIT_LOG();
}

void    OpenFilesListPlus:: evh_workspace_close_begin       (CodeBlocksEvent &_e)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::evh_workspace_close_begin()");

    layout()->workspace_close();

    OFLP_FUNC_EXIT_LOG();
}

void    OpenFilesListPlus:: evh_workspace_close_complete    (CodeBlocksEvent &_e)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::evh_workspace_close_complete()");

    GWRCB_TKI("editors count[%i]", EditorManager::Get()->GetEditorsCount() );

    reset();

    OFLP_FUNC_EXIT_LOG();
}

void    OpenFilesListPlus:: evh_project_open                (CodeBlocksEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::evh_project_open()");

    layout()->project_load( _e.GetProject() );

    RefreshOpenFilesLayout();

    OFLP_FUNC_EXIT_LOG();
}

void    OpenFilesListPlus:: evh_project_close               (CodeBlocksEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::evh_project_close()");

    GWR_TKI("project[%p] files:[%i]", _e.GetProject(), _e.GetProject()->GetFilesCount());

    layout()->project_close( _e.GetProject() );

    RefreshOpenFilesLayout();

    OFLP_FUNC_EXIT_LOG();
}

void    OpenFilesListPlus:: evh_project_save                (CodeBlocksEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::evh_project_save()");

    layout()->project_save( _e.GetProject() );

    OFLP_FUNC_EXIT_LOG();
}

void    OpenFilesListPlus:: evh_editor_opened               (CodeBlocksEvent& event)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    //  this callback can be called when :
    //  - file opening
    //  - file reloading
    //  ........................................................................
    EditorBase  *   nn_edb  =   event.GetEditor();
    OflpPanel   *   panel   =   NULL;
    wxString        s;
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::evh_editor_opened()");
    //  ........................................................................
    //D editor_dump(edb);
    if ( ! nn_edb )
    {
        GWR_TKE("%s", wxS("      ...editor is NULL"));
        goto lab_exit;
    }
    //  ........................................................................
    //  check if editor is already open ( case of reload )
    panel = panels()->get(nn_edb);
    if ( panel )
    {
        GWR_TKI("      ...editor already open in panel[%s]", panel->title().wc_str());
        goto lab_exit;
    }
    //  ........................................................................
    //  add the editor
    editors()->add(nn_edb);

    panels()->resize_and_layout();
    //  ........................................................................
lab_exit:
    OFLP_FUNC_EXIT_LOG();
    return;
}

void    OpenFilesListPlus:: evh_editor_closed       (CodeBlocksEvent& event)
{
    EditorBase  *   nn_edb  =   event.GetEditor();
    //  ........................................................................
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::evh_editor_closed()");
    //  ........................................................................
    if ( ! nn_edb )
    {
        GWR_TKI("%s", wxS("      ...editor is NULL"));
        goto lab_exit;
    }
    //  ........................................................................
    //  sub the editor
    editors()->sub(nn_edb);

    panels()->resize_and_layout();
    //  ........................................................................
lab_exit:
    OFLP_FUNC_EXIT_LOG();
    return;
}

void    OpenFilesListPlus:: evh_editor_activated    (CodeBlocksEvent& event)    //  _GWR_KNO_   editors may be activated by tabs
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    EditorBase  *   nn_edb  =   event.GetEditor();
    ProjectFile *   pjf     =   NULL;
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::evh_editor_activated()");
    //  ........................................................................
    //D editor_dump(edb);
    if ( ! nn_edb )
    {
        GWR_TKE("%s", wxS("      ...editor is NULL"));
        goto lab_exit;
    }
    //  ........................................................................
    //  if loading/closing workspace/project, skip RefreshOpenFileState()       //  _GWR_CB_TRICK_  _GWR_OPTIM_
    //  call
    if ( ProjectManager::IsBusy() )
    {
        GWR_TKI("%s", wxS("      ...Project manager is BUSY - dropping"));
        goto lab_exit;
    }
    //  ........................................................................
    //  if no shortname, skip the RefreshOpenFileState() call ( C::B sends an   //  _GWR_CB_TRICK_  _GWR_OPTIM_
    //  "activated" event with an empty-shortname before sending an "opened"
    //  event with a non-empty-shortname when opening a file )
    if ( nn_edb->GetShortName().IsEmpty() )
    {
        GWR_TKI("%s", wxS("      ...shortname is EMPTY - dropping"));
        goto lab_exit;
    }
    //  ........................................................................
    RefreshOpenFileState(nn_edb);
    //  ........................................................................
lab_exit:
    OFLP_FUNC_EXIT_LOG();
}

void    OpenFilesListPlus:: evh_editor_deactivated  (CodeBlocksEvent& event)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    //RefreshOpenFileState(event.GetEditor());
}

void    OpenFilesListPlus:: evh_editor_modified     (CodeBlocksEvent& event)
{
    EditorBase  *   nn_edb  =   event.GetEditor();
    //  ........................................................................
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    OFLP_FUNC_ENTER_MARK("OFLP::evh_editor_modified()");

    if ( ! nn_edb )
    {
        GWR_TKE("%s", wxS("      ...editor is NULL"));
        return;
    }

    RefreshOpenFileState(nn_edb);
}

void    OpenFilesListPlus:: evh_editor_saved        (CodeBlocksEvent& event)
{
    EditorBase  *   nn_edb  =   event.GetEditor();
    //  ........................................................................
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::evh_editor_saved()");
    //  ........................................................................
    if ( ! nn_edb )
    {
        GWR_TKE("%s", wxS("      ...editor is NULL"));
        goto lab_exit;
    }

    RefreshOpenFileState(nn_edb);
    //  ........................................................................
lab_exit:
    OFLP_FUNC_EXIT_LOG();
}
//  ############################################################################
//
//                          TREE ITEM EVENTS
//
//  ############################################################################
void    OpenFilesListPlus:: evh_tree_item_activated     (wxTreeEvent& event)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    OflpPanelItemInfo   inf( event );
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::OnTreeItemActivated()");
    //  ........................................................................
    if (Manager::IsAppShuttingDown())
        goto lab_return;

    if ( ! inf.IsOk() )
    {
        GWR_TKI("      ...invalid OflpPanelDataObject for panel[%p]", event.GetEventObject());
        goto lab_return;
    }

    //  In productivity mode, activation :
    //  - makes the corresponding editor the active editor ; since activation in
    //  wxWidgets correspond to a double click, that editor has already been
    //  made the active one.
    //  - reload the file
    if ( settings()->mode_productivity() )
    {
        //  reload the file
        Manager::Get()->GetEditorManager()->GetBuiltinEditor(inf.GetEditor())->Reload();
    }
    //  In standard mode, activation :
    //  - makes the corresponding editor the active editor
    if ( settings()->mode_standard() )
    {
        //  active selected editor
        Manager::Get()->GetEditorManager()->SetActiveEditor(inf.GetEditor());
    }
    //  ........................................................................
lab_return:
    OFLP_FUNC_EXIT_LOG();
}

void    OpenFilesListPlus:: evh_tree_item_right_click   (wxTreeEvent& event)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    wxPoint             pt;
    OflpPanelItemInfo   inf( event );
    //  ........................................................................
    //GWR_INF("%s", wxS("########## OpenFilesListPlus::OnTreeItemRightClick() ##########"));

    if (Manager::IsAppShuttingDown())
        return;

    #ifndef  MEMLEAKS

    if ( ! inf.IsOk() )
    {
        GWR_TKI("      ...invalid OflpPanelDataObject for panel[%p]", event.GetEventObject());
        goto lab_return;
    }

    pt = inf.GetTree()->ClientToScreen(event.GetPoint());
    inf.GetEditor()->DisplayContextMenu(pt,mtOpenFilesList);

    #endif
    //  ........................................................................
lab_return:
    return;
}

void    OpenFilesListPlus:: evh_tree_sel_changed        (wxTreeEvent &event)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    OflpPanelItemInfo   inf( event );
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::OnTreeSelChanged()");

    if(Manager::IsAppShuttingDown())
        goto lab_return;

    if ( ! inf.IsOk() )
    {
        GWR_TKI("      ...invalid OflpPanelDataObject for panel[%p]", event.GetEventObject());
        goto lab_return;
    }
    GWR_TKI("      ...[%p][%s]", inf.GetPanel(), inf.GetEditor()->GetShortName().wc_str());

    //  in productivity mode, selecting a file makes the corresponding editor
    //  the active editor
    if ( settings()->mode_productivity() )
    {
        //  active selected editor
        Manager::Get()->GetEditorManager()->SetActiveEditor(inf.GetEditor());
    }

    //  deselect any other wxTreeCtrl's selected item
    panels()->p0_unselect_except(inf.GetPanel());
    //  ........................................................................
lab_return:
    OFLP_FUNC_EXIT_LOG();
}
//  ############################################################################
//
//                          PANEL HEADER EVENTS
//
//  ############################################################################
void    OpenFilesListPlus::     evh_panel_header_button_clicked_add (wxCommandEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    panels()->p0_add(wxString::FromUTF8("panel"), false);

    panels()->resize_and_layout();
}

void    OpenFilesListPlus::     evh_panel_header_button_clicked_del (wxCommandEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    //  fasten your seatbelt for the cast
    //  event -> wxObject -> wxButton -> GetClientData() -> OflpPanel*
    OflpPanel   *   panel   = static_cast< OflpPanel* >
    (
        ( static_cast< wxButton* >( _e.GetEventObject() ) )->GetClientData()
    );
    //  ........................................................................
    GWR_INF("OFLP::evh_panel_header_button_del_CLICKED():EventObject[%p] ClientData()[%p]",
        _e.GetEventObject(), panel);

    OFLP_EMERGENCY__NULL_POINTER__RET( panel );
    //  ........................................................................
    OFLP_EMERGENCY__CALL__RET( panels()->p0_sub(panel) );

    panels()->resize_and_layout();
}

void    OpenFilesListPlus::     evh_panel_header_button_clicked_mm  (wxCommandEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    //  fasten your seatbelt for the cast
    //  event -> wxObject -> wxButton -> GetClientData() -> OflpPanel*
    OflpPanel   *   panel   = static_cast< OflpPanel* >
    (
        ( static_cast< wxButton* >( _e.GetEventObject() ) )->GetClientData()
    );
    //  ........................................................................
    GWR_INF("OFLP::evh_panel_header_button_up_CLICKED():EventObject[%p] ClientData()[%p]",
        _e.GetEventObject(), panel);

    OFLP_EMERGENCY__NULL_POINTER__RET( panel );
    //  ........................................................................
    OFLP_EMERGENCY__CALL__RET( panels()->p0_minmax(panel) );

    panels()->resize_and_layout();
}

void    OpenFilesListPlus::     evh_panel_header_button_clicked_up  (wxCommandEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    //  fasten your seatbelt for the cast
    //  event -> wxObject -> wxButton -> GetClientData() -> OflpPanel*
    OflpPanel   *   panel = static_cast< OflpPanel* >
    (
        ( static_cast< wxButton* >( _e.GetEventObject() ) )->GetClientData()
    );
    //  ........................................................................
    GWR_INF("OFLP::evh_panel_header_button_up_CLICKED():EventObject[%p] ClientData()[%p]",
        _e.GetEventObject(), panel);

    OFLP_EMERGENCY__NULL_POINTER__RET( panel );
    //  ........................................................................
    OFLP_EMERGENCY__CALL__RET( panels()->p0_move_up(panel) );
    panels()->resize_and_layout();
}

void    OpenFilesListPlus::     evh_panel_header_button_clicked_down(wxCommandEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    //  fasten your seatbelt for the cast
    //  event -> wxObject -> wxButton -> GetClientData() -> OflpPanel*
    OflpPanel   *   panel = static_cast< OflpPanel* >
    (
        ( static_cast< wxButton* >( _e.GetEventObject() ) )->GetClientData()
    );
    GWR_INF("OFLP::evh_panel_header_button_dn_CLICKED():EventObject[%p] ClientData()[%p]",
        _e.GetEventObject(), panel);

    panels()->p0_move_dn(panel);

    panels()->resize_and_layout();
}

void    OpenFilesListPlus::     evh_panel_header_button_clicked_opt (wxCommandEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ........................................................................
    settings()->action(_e);
    //  ........................................................................
    //  this handler is connect to a wxButton, so
    //  - _e.GetEventObject() is not NULL
    //  - _e.GetEventObject() can be casted to a wxWindow
    //  static_cast< wxWindow* >( _e.GetEventObject() )->PopupMenu(menu_options());
}
//  ############################################################################
//
//                          MENU EVENTS
//
//  ############################################################################
void    OpenFilesListPlus::     evh_settings_activated              (wxActivateEvent& _e)
{
    OFLP_FUNC_ENTER_MARK("oflp::evh_settings_activated()");

    if ( _e.GetEventObject() != (wxObject*)( settings()->dw_settings ) )
        return;

    settings()->settings_window_activated( _e.GetActive() );
}
