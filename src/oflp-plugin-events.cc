/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-common.hh"
#include    "oflp-common-macros.hh"

#include    "oflp-cb.hh"

#include    "oflp-plugin.hh"

#include    "oflp-plugin-mod-layout.hh"
#include    "oflp-plugin-mod-panels.hh"
#include    "oflp-plugin-mod-settings.hh"
#include    "oflp-plugin-mod-runflow.hh"

#include    "oflp-panel.hh"
#include    "oflp-panel-utils.hh"
//  ................................................................................................
#define ERG_OFLP_SANITY_CHECKS

#include    "generated/oflp-plugin-events--log-defines.cci"
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
    oflp::modRunflow()->wlc(true);
    //  ............................................................................................
    if ( ! oflp::modLayout()->x_workspace_load() )                                                  // for example when opening C::B
    {
        oflp::modRunflow()->wlc(false);
        goto lab_exit_failure;
    }
    //  ............................................................................................
    //  We sort PanelAssignments-by inserting them in a vector following their index
    s = oflp::modLayout()->x_pnas_size();
    if ( !s )
    {
        oflp::modRunflow()->wlc(false);
        goto lab_exit_success;
    }

    v.resize(s);                                                                                    // dimension the vector

    for ( i = 0 ; i != s ; i ++ )
    {
        pnas                    = oflp::modLayout()->x_pnas_get(i);
        v[ pnas->vix() - 1 ]    = pnas;
    }
    //  ............................................................................................
    //  Now we can add them
    for ( size_t i = 0 ; i != s ; i++ )
    {
        oflp::modPanels()->z_add( v[i]->name(), v[i]->uid() );
    }

    SyncEditorsToLayout();

    oflp::modPanels()->x_resize_and_layout();
    //  ............................................................................................
    oflp::modRunflow()->wlc(false);
    //  ............................................................................................
    ERG_LABELS_EXIT_SUCCESS_FAILURE_RET();
}

void    OpenFilesListPlus:: evh_workspace_close_begin       (CodeBlocksEvent &_e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_workspace_close_begin()");
    //  ............................................................................................
    oflp::modRunflow()->wcb(true);
    //  ............................................................................................
    oflp::modLayout()->x_workspace_save();
    //  ............................................................................................
    oflp::modRunflow()->wcb(false);
    //  ............................................................................................
    OFLP_LOG_FUNC_EXIT();
}

void    OpenFilesListPlus:: evh_workspace_close_complete    (CodeBlocksEvent &_e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_workspace_close_complete()");
    //  ............................................................................................
    oflp::modRunflow()->wcc(true);
    //  ............................................................................................
    ERGCB_TKI("editors count[%i]", EditorManager::Get()->GetEditorsCount() );

    y_reset();
    //  ............................................................................................
    oflp::modRunflow()->wcc(true);
    //  ............................................................................................
    OFLP_LOG_FUNC_EXIT();
}

void    OpenFilesListPlus:: evh_project_open                (CodeBlocksEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_project_open()");
    //  ............................................................................................
    oflp::modRunflow()->pop(true);
    //  ............................................................................................
    oflp::modLayout()->x_project_load( _e.GetProject() );

    SyncEditorsToLayout();
    //  ............................................................................................
    oflp::modRunflow()->pop(false);
    //  ............................................................................................
    OFLP_LOG_FUNC_EXIT();
}

void    OpenFilesListPlus:: evh_project_close               (CodeBlocksEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_project_close()");
    //  ............................................................................................
    oflp::modRunflow()->pcl(true);
    //  ............................................................................................
    ERG_TKI("project[%p] files:[%i]", _e.GetProject(), _e.GetProject()->GetFilesCount());

    oflp::modLayout()->x_project_close( _e.GetProject() );

    SyncEditorsToLayout();
    //  ............................................................................................
    oflp::modRunflow()->pcl(true);
    //  ............................................................................................
    OFLP_LOG_FUNC_EXIT();
}

void    OpenFilesListPlus:: evh_project_save                (CodeBlocksEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_project_save()");

    oflp::modLayout()->x_project_save( _e.GetProject() );

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
    //L//CB::EdbInfos                                inf;
    oflp::CB::EdbInfos                          inf;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_editor_opened()");
    //  ............................................................................................
    if ( ! nn_edb )
    {
        ERG_ERR("%s", wxS("      ...EditorBase is NULL"));
        goto lab_exit_failure;
    }
    //  ............................................................................................
    //  check if editor is already open ( case of reload )
    panel = oflp::modPanels()->x_get_from_editor(nn_edb);
    if ( panel )
    {
        ERG_TKI("      ...editor already open in panel[%p][%s]", panel, panel->title().wc_str());
        goto lab_exit_success;
    }
    //  ............................................................................................
    //L//if ( ! CB::X_EditorBase_get_infos( &inf, CB::ePfas, nn_edb ) )
    if ( ! oflp::CB::X_EditorBase_get_infos( &inf, oflp::CB::ePfas, nn_edb ) )
    {
        ERG_TKW("%s", wxS("      ...could not get infos from EditorBase"));
        goto lab_add_to_panel;
    }
    ERG_TKI("      ...ProjectFile :[%p] fp[%s] rpfn[%s] rctlp[%s]", inf.pjf(), inf.a_fp, inf.a_rfp, wxS("---"));
    ERG_TKI("      ...cbProject   :[%p] [%s]", inf.prj(), inf.prj()->GetTitle().wc_str());
    ERG_TKI("      ...PfAs        :[%p] panel[%s]", inf.pfas(), inf.pfas()->uid().str().wc_str());
    //  ............................................................................................
    if ( ! oflp::modPanels()->x_get_from_uid( &panel, inf.pfas()->uid() ) );
    {
        ERG_TKI("%s", "      ...Panel is NULL (no panel with that name)");
        goto lab_add_to_panel;
    }
    ERG_TKI("      ...Panel [%p][%s]", panel->title().wc_str());
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
        if ( ! oflp::modPanels()->z_get_bulk()->editor_add(nn_edb) )
            goto lab_exit_failure;
    }

    oflp::modPanels()->x_resize_and_layout();
    //  ............................................................................................
    ERG_LABELS_EXIT_SUCCESS_FAILURE_RET();
}

void    OpenFilesListPlus:: evh_editor_closed               (CodeBlocksEvent& event)
{
    EditorBase                              *   nn_edb  =   event.GetEditor();
    cbEditor                                *   nn_edc  =   nullptr;
    oflp::CB::EdbInfos                          inf;
    OflpPanel                               *   panel   =   nullptr;
    wxString                                    fp, fn, rfp;
    //  ............................................................................................
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::evh_editor_closed()");
    //  ............................................................................................
    if ( ! nn_edb )
    {
        ERG_TKI("%s", wxS("      ...editor is NULL"));
        goto lab_exit;
    }
    ERG_TKI("      ...editor[%p][%s][%s]", OFLP_EDB_LOG(nn_edb));

    fn  =   nn_edb->GetTitle();
    fp  =   nn_edb->GetFilename();
    //  ============================================================================================
lab_sub_from_pjas:
    {

    //L//if ( ! CB::X_EditorBase_get_infos( &inf, CB::ePfas, nn_edb ) )
    if ( ! oflp::CB::X_EditorBase_get_infos( &inf, oflp::CB::ePfas, nn_edb ) )
    {
        ERG_TKW("%s", wxS("      ...could not get infos from EditorBase"));
        goto lab_sub_from_panel;
    }
    ERG_TKI("      ...ProjectFile :[%p] fp[%s] rpfn[%s] rctlp[%s]", inf.pjf(), inf.a_fp, inf.a_rfp, wxS("---"));
    ERG_TKI("      ...cbProject   :[%p] [%s]", inf.prj(), inf.prj()->GetTitle().wc_str());
    ERG_TKI("      ...PjAs        :[%p] cbProject [%p][%s]", inf.pjas(), inf.pjas()->project(), inf.pjas()->project()->GetTitle().wc_str());
    ERG_TKI("      ...PfAs        :[%p] panel[%s]", inf.pfas(), inf.pfas()->uid().str().wc_str());

    inf.pjas()->x_pfas_sub(inf.pfas());                                                             // sub the PfAs

    }
    //  ============================================================================================
lab_sub_from_panel:
    {

    panel = oflp::modPanels()->x_get_from_editor( nn_edb );
    if ( ! panel )
    {
        ERG_TKE("%s", "      ...no Panel found");                                                   // ERROR
        goto lab_exit;
    }
    ERG_TKI("      ...Panel [%p][%s][%s]", OFLP_PANEL_LOG(panel));

    panel->editor_del(nn_edb);

    oflp::modPanels()->x_resize_and_layout();

    }
    //  ============================================================================================
lab_exit:
    OFLP_LOG_FUNC_EXIT();
    return;
}

void    OpenFilesListPlus:: evh_editor_activated            (CodeBlocksEvent& event)    //  _ERG_KNO_   editors may be activated by tabs
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
        ERG_TKE("%s", wxS("      ...editor is NULL"));
        goto lab_exit;
    }
    //  ............................................................................................
    ERG_TKI("      ...editor[%s]", nn_edb->GetTitle().wc_str());
    //  ............................................................................................
    //  if no title, skip the RefreshOpenFileState() call ( C::B sends an       //  _ERG_CB_TRICK_  _ERG_OPTIM_
    //  "activated" event with an empty-title before sending an "opened"
    //  event with a non-empty-title when opening a file )
    if ( nn_edb->GetTitle().IsEmpty() )
    {
        ERG_TKI("%s", wxS("      ...title is EMPTY - dropping"));
        goto lab_exit;
    }
    //  ............................................................................................
    //  if loading/closing workspace/project, skip RefreshOpenFileState()       //  _ERG_CB_TRICK_  _ERG_OPTIM_
    //  call
    if ( ProjectManager::IsBusy() )
    {
        ERG_TKI("%s", wxS("      ...[OPTIM]Project manager is busy - dropping"));
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
        ERG_TKE("%s", wxS("      ...editor is NULL"));
        return;
    }
    //  ............................................................................................
    ERG_TKI("      ...editor[%s]", nn_edb->GetTitle().wc_str());
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
        ERG_TKE("%s", wxS("      ...editor is NULL"));
        goto lab_exit;
    }
    //  ............................................................................................
    ERG_TKI("      ...editor[%s]", nn_edb->GetTitle().wc_str());
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
        ERG_TKI("      ...invalid OflpPanelDataObject for panel[%p]", event.GetEventObject());
        goto lab_return;
    }

    //  In productivity mode, activation :
    //  - makes the corresponding editor the active editor ; since activation in
    //  wxWidgets correspond to a double click, that editor has already been
    //  made the active one.
    //  - reload the file
    if ( oflp::modSettings()->mode_productivity() )
    {
        //  reload the file
        Manager::Get()->GetEditorManager()->GetBuiltinEditor(inf.x_get_editor())->Reload();
    }
    //  In standard mode, activation :
    //  - makes the corresponding editor the active editor
    if ( oflp::modSettings()->mode_standard() )
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
    ERG_INF("%s", wxS("########## OpenFilesListPlus::OnTreeItemRightClick() ##########"));

    if (Manager::IsAppShuttingDown())
        return;

    #ifndef  MEMLEAKS

    if ( ! inf.IsOk() )
    {
        ERG_TKI("      ...invalid OflpPanelDataObject for panel[%p]", event.GetEventObject());
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
                ERG_TKE("      ...tree[%p] (eOkExceptData) with wrong wxTreeItemId DnD", inf.x_get_tree());
                goto lab_return;
            }
            else
            {
                ERG_TKI("      ...tree[%p] (eOkExceptData) with good wxTreeItemId DnD - dropping", inf.x_get_tree());
                goto lab_return;
            }
        }
        else
        {
            ERG_TKE("      ...event object[%p] invalid event - dropping", event.GetEventObject());
            goto lab_return;
        }
    }
    ERG_TKI("      ...panel[%p] editor[%p][%s]", inf.x_get_panel(), inf.x_get_editor(), inf.x_get_editor()->GetTitle().wc_str());

    //  in productivity mode, selecting a file makes the corresponding editor
    //  the active editor
    if ( oflp::modSettings()->mode_productivity() )
    {
        //  active selected editor
        Manager::Get()->GetEditorManager()->SetActiveEditor(inf.x_get_editor());
    }

    //  deselect all wxTreeItems in other Panels
    oflp::modPanels()->z_unselect_except(inf.x_get_panel());
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
    oflp::modPanels()->z_add(wxString::FromUTF8("panel"), oflp::UID() );

    oflp::modPanels()->x_resize_and_layout();
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
    ERG_INF("OFLP::evh_panel_header_button_del_CLICKED():EventObject[%p] ClientData()[%p]",
        _e.GetEventObject(), panel);

    OFLP_EMERGENCY__NULL_POINTER__RET( panel );
    //  ............................................................................................
    OFLP_EMERGENCY__CALL__RET( oflp::modPanels()->z_sub(panel) );

    oflp::modPanels()->x_resize_and_layout();
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
    ERG_INF("OFLP::evh_panel_header_button_up_CLICKED():EventObject[%p] ClientData()[%p]",
        _e.GetEventObject(), panel);

    OFLP_EMERGENCY__NULL_POINTER__RET( panel );
    //  ............................................................................................
    OFLP_EMERGENCY__CALL__RET( oflp::modPanels()->z_minmax(panel) );

    oflp::modPanels()->x_resize_and_layout();
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
    ERG_INF("OFLP::evh_panel_header_button_up_CLICKED():EventObject[%p] ClientData()[%p]",
        _e.GetEventObject(), panel);

    OFLP_EMERGENCY__NULL_POINTER__RET( panel );
    //  ............................................................................................
    OFLP_EMERGENCY__CALL__RET( oflp::modPanels()->z_move_up(panel) );
    oflp::modPanels()->x_resize_and_layout();
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
    ERG_INF("OFLP::evh_panel_header_button_dn_CLICKED():EventObject[%p] ClientData()[%p]",
        _e.GetEventObject(), panel);

    oflp::modPanels()->z_move_dn(panel);

    oflp::modPanels()->x_resize_and_layout();
}

void    OpenFilesListPlus::     evh_panel_header_button_clicked_opt (wxCommandEvent& _e)
{
    OFLP_ON_DEGRADED__RET();
    //  ............................................................................................
    oflp::modSettings()->action(_e);
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

    if ( _e.GetEventObject() != (wxObject*)( oflp::modSettings()->dw_settings ) )
        return;

    oflp::modSettings()->settings_window_activated( _e.GetActive() );
}
