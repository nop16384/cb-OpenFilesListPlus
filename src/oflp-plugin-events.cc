/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin.hh"

#include    "oflp-plugin-mod-layout.hh"
#include    "oflp-plugin-mod-panels.hh"
#include    "oflp-plugin-mod-settings.hh"
#include    "oflp-plugin-mod-runflow.hh"

#include    "oflp-panel.hh"
//  ................................................................................................
#define GWR_OFLP_SANITY_CHECKS
#define GWR_LOG(FORMAT, ...)    GWRCB_LOG(FORMAT, __VA_ARGS__);
#define GWR_TKI(FORMAT, ...)    GWRCB_TKI(FORMAT, __VA_ARGS__);
#define GWR_TKW(FORMAT, ...)    GWRCB_TKW(FORMAT, __VA_ARGS__);
#define GWR_TKE(FORMAT, ...)    GWRCB_TKE(FORMAT, __VA_ARGS__);
#define GWR_INF(FORMAT, ...)    GWRCB_INF(FORMAT, __VA_ARGS__);
#define GWR_WNG(FORMAT, ...)    GWRCB_WNG(FORMAT, __VA_ARGS__);
#define GWR_ERR(FORMAT, ...)    GWRCB_ERR(FORMAT, __VA_ARGS__);
//  ################################################################################################
//
//                          EDITOR EVENTS
//
//  ################################################################################################
void    OpenFilesListPlus:: evh_workspace_loading_complete  (CodeBlocksEvent& _e)
{
    size_t                                          s       =   0;                                  // # of panels ( not including bulk )
    size_t                                          i       =   0;
    std::vector< OflpModLayout::PnAs const * >      v;
    OflpModLayout::PnAs              const      *   pnas    =   nullptr;
    //  ............................................................................................
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_workspace_loading_complete()");
    //  ............................................................................................
    runflow()->wlc(true);
    //  ............................................................................................
    if ( ! layout()->x_workspace_load() )                                                           // for example when opening C::B
    {
        runflow()->wlc(false);
        goto lab_exit_failure;
    }
    //  ............................................................................................
    //  We sort PanelAssignments-by inserting them in a vector following their index
    s = layout()->x_pnas_size();
    if ( !s )
    {
        runflow()->wlc(false);
        goto lab_exit_success;
    }

    v.resize(s);                                                                                    // dimension the vector

    for ( i = 0 ; i != s ; i ++ )
    {
        pnas                    = layout()->x_pnas(i);
        v[ pnas->index() - 1 ]  = pnas;
    }
    //  ............................................................................................
    //  Now we can add them
    for ( size_t i = 0 ; i != s ; i++ )
    {
        panels()->z_add( v[i]->name(), v[i]->uid() );
    }

    SyncEditorsToLayout();

    panels()->resize_and_layout();
    //  ............................................................................................
    runflow()->wlc(false);
    //  ............................................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RET();
}

void    OpenFilesListPlus:: evh_workspace_close_begin       (CodeBlocksEvent &_e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_workspace_close_begin()");
    //  ............................................................................................
    runflow()->wcb(true);
    //  ............................................................................................
    layout()->x_workspace_save();
    //  ............................................................................................
    runflow()->wcb(false);
    //  ............................................................................................
    OFLP_LOG_FUNC_EXIT();
}

void    OpenFilesListPlus:: evh_workspace_close_complete    (CodeBlocksEvent &_e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_workspace_close_complete()");
    //  ............................................................................................
    runflow()->wcc(true);
    //  ............................................................................................
    GWRCB_TKI("editors count[%i]", EditorManager::Get()->GetEditorsCount() );

    y_reset();
    //  ............................................................................................
    runflow()->wcc(true);
    //  ............................................................................................
    OFLP_LOG_FUNC_EXIT();
}

void    OpenFilesListPlus:: evh_project_open                (CodeBlocksEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_project_open()");
    //  ............................................................................................
    runflow()->pop(true);
    //  ............................................................................................
    layout()->x_project_load( _e.GetProject() );

    SyncEditorsToLayout();
    //  ............................................................................................
    runflow()->pop(false);
    //  ............................................................................................
    OFLP_LOG_FUNC_EXIT();
}

void    OpenFilesListPlus:: evh_project_close               (CodeBlocksEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_project_close()");
    //  ............................................................................................
    runflow()->pcl(true);
    //  ............................................................................................
    GWR_TKI("project[%p] files:[%i]", _e.GetProject(), _e.GetProject()->GetFilesCount());

    layout()->x_project_close( _e.GetProject() );

    SyncEditorsToLayout();
    //  ............................................................................................
    runflow()->pcl(true);
    //  ............................................................................................
    OFLP_LOG_FUNC_EXIT();
}

void    OpenFilesListPlus:: evh_project_save                (CodeBlocksEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_project_save()");

    layout()->x_project_save( _e.GetProject() );

    OFLP_LOG_FUNC_EXIT();
}

void    OpenFilesListPlus:: evh_editor_opened               (CodeBlocksEvent& event)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    //  this callback can be called when :
    //  - file opening
    //  - file reloading
    //  ............................................................................................
    EditorBase                              *   nn_edb  =   event.GetEditor();
    cbEditor                                *   nn_edc  =   nullptr;
    OflpPanel                               *   panel   =   nullptr;
    CB::EdbInfos                                inf;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_editor_opened()");
    //  ............................................................................................
    if ( ! nn_edb )
    {
        GWR_ERR("%s", wxS("      ...EditorBase is NULL"));
        goto lab_exit_failure;
    }
    //  ............................................................................................
    //  check if editor is already open ( case of reload )
    panel = panels()->x_panel_from_editor(nn_edb);
    if ( panel )
    {
        GWR_TKI("      ...editor already open in panel[%p][%s]", panel, panel->title().wc_str());
        goto lab_exit_success;
    }
    //  ............................................................................................
    if ( ! CB::X_EditorBase_get_infos( &inf, CB::eFlas, nn_edb ) )
    {
        GWR_TKW("%s", wxS("      ...could not get infos from EditorBase"));
        goto lab_add_to_panel;
    }
    GWR_TKI("      ...ProjectFile :[%p] fp[%s] rpfn[%s] rctlp[%s]", inf.pjf(), inf.a_fp, inf.a_rfp, wxS("---"));
    GWR_TKI("      ...cbProject   :[%p] [%s]", inf.prj(), inf.prj()->GetTitle().wc_str());
    GWR_TKI("      ...FlAs        :[%p] panel[%s]", inf.flas(), inf.flas()->uid().str().wc_str());
    //  ............................................................................................
    if ( ! panels()->x_panel_from_uid( &panel, inf.flas()->uid() ) );
    {
        GWR_TKI("%s", "      ...Panel is NULL (no panel with that name)");
        goto lab_add_to_panel;
    }
    GWR_TKI("      ...Panel [%p][%s]", panel->title().wc_str());
    //  ............................................................................................
    //  add the editor to the good panel
lab_add_to_panel:
    if ( panel )
    {
        if ( ! panel->editor_add(nn_edb) )
            goto lab_exit_failure;
    }
    else
    {
        if ( ! panels()->p0_bulk()->editor_add(nn_edb) )
            goto lab_exit_failure;
    }

    panels()->resize_and_layout();
    //  ............................................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RET();
}

void    OpenFilesListPlus:: evh_editor_closed               (CodeBlocksEvent& event)
{
    EditorBase                              *   nn_edb  =   event.GetEditor();
    cbEditor                                *   nn_edc  =   nullptr;
    CB::EdbInfos                                inf;
    OflpPanel                               *   panel   =   nullptr;
    wxString                                    fp, fn, rfp;
    //  ............................................................................................
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_editor_closed()");
    //  ............................................................................................
    if ( ! nn_edb )
    {
        GWR_TKI("%s", wxS("      ...editor is NULL"));
        goto lab_exit;
    }
    GWR_TKI("      ...editor[%p][%s][%s]", OFLP_EDB_LOG(nn_edb));

    fn  =   nn_edb->GetTitle();
    fp  =   nn_edb->GetFilename();
    //  ============================================================================================
lab_sub_from_pjas:
    {

    if ( ! CB::X_EditorBase_get_infos( &inf, CB::eFlas, nn_edb ) )
    {
        GWR_TKW("%s", wxS("      ...could not get infos from EditorBase"));
        goto lab_sub_from_panel;
    }
    GWR_TKI("      ...ProjectFile :[%p] fp[%s] rpfn[%s] rctlp[%s]", inf.pjf(), inf.a_fp, inf.a_rfp, wxS("---"));
    GWR_TKI("      ...cbProject   :[%p] [%s]", inf.prj(), inf.prj()->GetTitle().wc_str());
    GWR_TKI("      ...PjAs        :[%p] cbProject [%p][%s]", inf.pjas(), inf.pjas()->project(), inf.pjas()->project()->GetTitle().wc_str());
    GWR_TKI("      ...FlAs        :[%p] panel[%s]", inf.flas(), inf.flas()->uid().str().wc_str());

    inf.pjas()->sub(inf.flas());                                                                    // sub the FlAs

    }
    //  ============================================================================================
lab_sub_from_panel:
    {

    panel = panels()->x_panel_from_editor( nn_edb );
    if ( ! panel )
    {
        GWR_TKE("%s", "      ...no Panel found");                                                   // ERROR
        goto lab_exit;
    }
    GWR_TKI("      ...Panel [%p][%s][%s]", OFLP_PANEL_LOG(panel));

    panel->editor_del(nn_edb);

    panels()->resize_and_layout();

    }
    //  ============================================================================================
lab_exit:
    OFLP_LOG_FUNC_EXIT();
    return;
}

void    OpenFilesListPlus:: evh_editor_activated            (CodeBlocksEvent& event)    //  _GWR_KNO_   editors may be activated by tabs
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    EditorBase  *   nn_edb  =   event.GetEditor();
    ProjectFile *   pjf     =   NULL;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_editor_activated()");
    //  ............................................................................................
    //D editor_dump(edb);
    if ( ! nn_edb )
    {
        GWR_TKE("%s", wxS("      ...editor is NULL"));
        goto lab_exit;
    }
    //  ............................................................................................
    GWR_TKI("      ...editor[%s]", nn_edb->GetTitle().wc_str());
    //  ............................................................................................
    //  if no title, skip the RefreshOpenFileState() call ( C::B sends an       //  _GWR_CB_TRICK_  _GWR_OPTIM_
    //  "activated" event with an empty-title before sending an "opened"
    //  event with a non-empty-title when opening a file )
    if ( nn_edb->GetTitle().IsEmpty() )
    {
        GWR_TKI("%s", wxS("      ...title is EMPTY - dropping"));
        goto lab_exit;
    }
    //  ............................................................................................
    //  if loading/closing workspace/project, skip RefreshOpenFileState()       //  _GWR_CB_TRICK_  _GWR_OPTIM_
    //  call
    if ( ProjectManager::IsBusy() )
    {
        GWR_TKI("%s", wxS("      ...[OPTIM]Project manager is busy - dropping"));
        goto lab_exit;
    }
    //  ............................................................................................
    RefreshOpenFileState(nn_edb);
    //  ............................................................................................
lab_exit:
    OFLP_LOG_FUNC_EXIT();
}

void    OpenFilesListPlus:: evh_editor_deactivated          (CodeBlocksEvent& event)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    //RefreshOpenFileState(event.GetEditor());
}

void    OpenFilesListPlus:: evh_editor_modified             (CodeBlocksEvent& event)
{
    EditorBase  *   nn_edb  =   event.GetEditor();
    //  ............................................................................................
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_FUNC_ENTER_MARK("OFLP::evh_editor_modified()");
    //  ............................................................................................
    if ( ! nn_edb )
    {
        GWR_TKE("%s", wxS("      ...editor is NULL"));
        return;
    }
    //  ............................................................................................
    GWR_TKI("      ...editor[%s]", nn_edb->GetTitle().wc_str());
    //  ............................................................................................
    RefreshOpenFileState(nn_edb);
}

void    OpenFilesListPlus:: evh_editor_saved                (CodeBlocksEvent& event)
{
    EditorBase  *   nn_edb  =   event.GetEditor();
    //  ............................................................................................
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_editor_saved()");
    //  ............................................................................................
    if ( ! nn_edb )
    {
        GWR_TKE("%s", wxS("      ...editor is NULL"));
        goto lab_exit;
    }
    //  ............................................................................................
    GWR_TKI("      ...editor[%s]", nn_edb->GetTitle().wc_str());
    //  ............................................................................................
    RefreshOpenFileState(nn_edb);
    //  ............................................................................................
lab_exit:
    OFLP_LOG_FUNC_EXIT();
}
//  ################################################################################################
//
//                          TREE ITEM EVENTS
//
//  ################################################################################################
void    OpenFilesListPlus:: evh_tree_item_activated     (wxTreeEvent& event)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OflpPanelTiEventInfo    inf( event );
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::OnTreeItemActivated()");
    //  ............................................................................................
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
        Manager::Get()->GetEditorManager()->GetBuiltinEditor(inf.x_get_editor())->Reload();
    }
    //  In standard mode, activation :
    //  - makes the corresponding editor the active editor
    if ( settings()->mode_standard() )
    {
        //  active selected editor
        Manager::Get()->GetEditorManager()->SetActiveEditor(inf.x_get_editor());
    }
    //  ............................................................................................
lab_return:
    OFLP_LOG_FUNC_EXIT();
}

void    OpenFilesListPlus:: evh_tree_item_right_click   (wxTreeEvent& event)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    wxPoint                 pt;
    OflpPanelTiEventInfo    inf( event );
    //  ............................................................................................
    GWR_INF("%s", wxS("########## OpenFilesListPlus::OnTreeItemRightClick() ##########"));

    if (Manager::IsAppShuttingDown())
        return;

    #ifndef  MEMLEAKS

    if ( ! inf.IsOk() )
    {
        GWR_TKI("      ...invalid OflpPanelDataObject for panel[%p]", event.GetEventObject());
        goto lab_return;
    }

    pt = inf.x_get_tree()->ClientToScreen(event.GetPoint());
    inf.x_get_editor()->DisplayContextMenu(pt,mtOpenFilesList);

    #endif
    //  ............................................................................................
lab_return:
    return;
}

void    OpenFilesListPlus:: evh_tree_sel_changed        (wxTreeEvent &event)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OflpPanelTiEventInfo    inf( event );
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::OnTreeSelChanged()");

    if(Manager::IsAppShuttingDown())
        goto lab_return;

    if ( ! inf.IsOk(OflpPanelTiEventInfo::eOkAll) )
    {
        if ( inf.IsOk(OflpPanelTiEventInfo::eOkExceptData) )                                        // _ERG_TECH_ (001)
        {
            if ( OflpPanel::X_last_wxTreeItemId_dragged_and_dropped() != inf.GetId() )
            {
                GWR_TKE("      ...tree[%p] (eOkExceptData) with wrong wxTreeItemId DnD", inf.x_get_tree());
                goto lab_return;
            }
            else
            {
                GWR_TKI("      ...tree[%p] (eOkExceptData) with good wxTreeItemId DnD - dropping", inf.x_get_tree());
                goto lab_return;
            }
        }
        else
        {
            GWR_TKE("      ...event object[%p] invalid event - dropping", event.GetEventObject());
            goto lab_return;
        }
    }
    GWR_TKI("      ...panel[%p] editor[%p][%s]", inf.x_get_panel(), inf.x_get_editor(), inf.x_get_editor()->GetTitle().wc_str());

    //  in productivity mode, selecting a file makes the corresponding editor
    //  the active editor
    if ( settings()->mode_productivity() )
    {
        //  active selected editor
        Manager::Get()->GetEditorManager()->SetActiveEditor(inf.x_get_editor());
    }

    //  deselect all wxTreeItems in other Panels
    panels()->p0_unselect_except(inf.x_get_panel());
    //  ............................................................................................
lab_return:
    OFLP_LOG_FUNC_EXIT();
}
//  ################################################################################################
//
//                          PANEL HEADER EVENTS
//
//  ################################################################################################
void    OpenFilesListPlus::     evh_panel_header_button_clicked_add (wxCommandEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    panels()->z_add(wxString::FromUTF8("panel"), oflp::UID() );

    panels()->resize_and_layout();
}

void    OpenFilesListPlus::     evh_panel_header_button_clicked_del (wxCommandEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    //  fasten your seatbelt for the cast
    //  event -> wxObject -> wxButton -> GetClientData() -> OflpPanel*
    OflpPanel   *   panel   = static_cast< OflpPanel* >
    (
        ( static_cast< wxButton* >( _e.GetEventObject() ) )->GetClientData()
    );
    //  ............................................................................................
    GWR_INF("OFLP::evh_panel_header_button_del_CLICKED():EventObject[%p] ClientData()[%p]",
        _e.GetEventObject(), panel);

    OFLP_EMERGENCY__NULL_POINTER__RET( panel );
    //  ............................................................................................
    OFLP_EMERGENCY__CALL__RET( panels()->p0_sub(panel) );

    panels()->resize_and_layout();
}

void    OpenFilesListPlus::     evh_panel_header_button_clicked_mm  (wxCommandEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    //  fasten your seatbelt for the cast
    //  event -> wxObject -> wxButton -> GetClientData() -> OflpPanel*
    OflpPanel   *   panel   = static_cast< OflpPanel* >
    (
        ( static_cast< wxButton* >( _e.GetEventObject() ) )->GetClientData()
    );
    //  ............................................................................................
    GWR_INF("OFLP::evh_panel_header_button_up_CLICKED():EventObject[%p] ClientData()[%p]",
        _e.GetEventObject(), panel);

    OFLP_EMERGENCY__NULL_POINTER__RET( panel );
    //  ............................................................................................
    OFLP_EMERGENCY__CALL__RET( panels()->p0_minmax(panel) );

    panels()->resize_and_layout();
}

void    OpenFilesListPlus::     evh_panel_header_button_clicked_up  (wxCommandEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    //  fasten your seatbelt for the cast
    //  event -> wxObject -> wxButton -> GetClientData() -> OflpPanel*
    OflpPanel   *   panel = static_cast< OflpPanel* >
    (
        ( static_cast< wxButton* >( _e.GetEventObject() ) )->GetClientData()
    );
    //  ............................................................................................
    GWR_INF("OFLP::evh_panel_header_button_up_CLICKED():EventObject[%p] ClientData()[%p]",
        _e.GetEventObject(), panel);

    OFLP_EMERGENCY__NULL_POINTER__RET( panel );
    //  ............................................................................................
    OFLP_EMERGENCY__CALL__RET( panels()->p0_move_up(panel) );
    panels()->resize_and_layout();
}

void    OpenFilesListPlus::     evh_panel_header_button_clicked_down(wxCommandEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
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
    //  ............................................................................................
    settings()->action(_e);
    //  ............................................................................................
    //  this handler is connect to a wxButton, so
    //  - _e.GetEventObject() is not NULL
    //  - _e.GetEventObject() can be casted to a wxWindow
    //  static_cast< wxWindow* >( _e.GetEventObject() )->PopupMenu(menu_options());
}
//  ################################################################################################
//
//                          MENU EVENTS
//
//  ################################################################################################
void    OpenFilesListPlus::     evh_settings_activated              (wxActivateEvent& _e)
{
    OFLP_FUNC_ENTER_MARK("oflp::evh_settings_activated()");

    if ( _e.GetEventObject() != (wxObject*)( settings()->dw_settings ) )
        return;

    settings()->settings_window_activated( _e.GetActive() );
}
