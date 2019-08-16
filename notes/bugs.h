+(001)  2015.08.21  When editor change from oflp, focus stay on oflp, is not given
                    to editor

 (002)  2015.08.21  When editing OflpPanel title, the wxTextCtrl remains if
                    user click anywhere else ; should switch back to wxStaticText

 (003)  2015.08.21  After DnD selection of item is lost

 (004)  2015.08.21  Double loop : mouse -> activate editor -> select editor -> OnTreeItemSelected -> refresh -> select editor

 (005)  2015.08.22  Double call to OflpPanelHeader::switch_to_static()
                    because of TEXT_ENTER _AND_ KILL_FOCUS events

 (006)  2015.08.29  libxxx.so vs xxx.so in zip file & lib loading

 (007)  2015.08.30  When closing log window, menu options is not refreshed

 (008)  2015.08.30  View menu misplacement

 (009)  2015.08.30  C::B OFLP window title !

 (010)  2015.08.30  I get bunch of cb debug in my wxLog window

 (011)  2015.09.06  Saving file as a different name dont change the wxTreeItem text !!!

 (012)  2015.09.08  Bug : when error clicked on in "build log", file not selected
                    in any panel !

 (013)  2015.09.08  In std mode, selecting item then using "save workspace layout" menu
                    reset to old item selection ( i.e. active editor )

 (014)  2015.10.22  When minimizing, panel name is no more aligned ( vertically )
                    with other panels's names

 (015)  2015.11.29  in OpenFilesListPlus::RefreshOpenFileState(EditorBase* _nn_edb),
                    in limit cases there is no active editor ,_nn_edb is NULL -> segfault
                    ( call from void OpenFilesListPlus::RefreshOpenFilesLayout() )

 (016)  2015.12.24  Panel order is not respected @ workspace loading
                    ( cf todo#17 )

+(017)  2015.12.24  Experienced a project.oflp.layout not saved ( files not
                    assigned to any panel )

 (018)  2015.12.27  Select a file in dclick mode. single click another file.
                    Switch to sclick mode. Exit settings panel by single click
                    on the seconf file. First file stay edited, Second is selected.

 (019)  2015.12.31  File open but not belonging to any project is removed from
                    bulk @ close_workspace_complete -> editor is open, but
                    not in any panel

+(020)  2015.12.31  When installing OpenFilesListPlus.cbplugin, current open
                    editors are simply ignored :)

 (021)  2016.01.05  bool OflpModLayout::project_save() dont return any boolean

 (022)  2016.01.05  OflpPanelBulk ctor calls OflpPanel ctor, and all the
                    graphical setup is done twice :(

 (023)  2016.01.06  [oflp-panel-utils.cc]OpenFilesListPlusPanelDropTarget::OnData()
                    wxDropTarget::OnData() is not implemented on ms

+(024)  2019.06.17  Rename a file + add to active project -> bug when saving
//  ############################################################################
(001)
//  ============================================================================
(002)
2015.08.21  Done: by handling wxTextCtrl wxEVT_KILL_FOCUS event
//  ============================================================================
(003)
2015.08.??  Done
//  ============================================================================
(004)
2015.09.02  - resizing the log window causes a cbEventEditorActivated
2015.09.17  Done : all has been rewrited ; anyway C::B sends 2 activated events
//  ============================================================================
(005)
2015.08.23  done : flag OflpPanel::a_allow_kill_focus_event
//  ============================================================================
(006)
2015.08.30  Done
//  ============================================================================
(007)
2015.08.30  Done : subclassing wxLogWindow and disallowing closing it directly
//  ============================================================================
(008)
2015.08.30  Done : without doing nothing ! surely related to (019)
//  ============================================================================
(009)
2015.08.30  Done : in OnAttach(), change the name of the CodeBlocksDockEvent - it
            was the same as in OpenFilesListPlugin, that caused the bug
//  ============================================================================
(010)
2015.09.01  Done : by replacing the wxLog...stuff by a wxFrame + wxTextCtrl
//  ============================================================================
(011)
2015.09.12  Done : item name verification was commented in OflpPanel::editor_sync()
//  ============================================================================
(012)
2015.09.08  Done : wxTreeCtrl->GetSelection() is buggy - removed all calls
//  ============================================================================
(013)
2015.09.17  Done : all has been rewritten
//  ============================================================================
(014)
2015.12.28  Done : setting a "disabled state" bitmap to an empty one ( eBmpBarPadding )
            So for minimizing / maximizing, we dont wxWidgets::Show() the buttons,
            we just wxWidgets::Enable() them.
//  ============================================================================
(015)
2015.11.30  Done, verified all calls to methods intaking a _nn parameter :

    # = OK - verified
    $ = intaked parameter, previous caller should have been verified

01#     bool        OflpModEditors::    add (EditorBase* _nn_edb)
02#     bool        OflpModEditors::    sub (EditorBase* _nn_edb)
03#     bool        OflpModEditors::    mov (
                        OflpPanel   *   _nn_dst     ,
                        OflpPanel   *   _nn_src     ,
                        EditorBase  *   _nn_edb     )

04$     OflpPanel*  OflpModLayout::     file_assignment_find_panel_from_editor_base(EditorBase* _nn_edb)
05$     void        OflpModLayout::     file_assignment_update(
                        EditorBase  *   _nn_edb         ,
                        OflpPanel   *   _nn_dst_panel   )

06$     bool        OflpModPanels::     p0_editors_mov  (OflpPanel* _nn_dst, OflpPanel* _nn_src)
07#$    bool        OflpModPanels::     p0_editor_mov   (OflpPanel* _nn_dst, OflpPanel* _nn_src, EditorBase* _nn_edb)
08#     bool        OflpModPanels::     p0_sub          (OflpPanel* _nn_panel)
09#     bool        OflpModPanels::     p0_move_up      (OflpPanel* _nn_panel)
10?     bool        OflpModPanels::     p0_move_dn      (OflpPanel* _nn_panel)         from GetClientData() - TODO ?
11#     bool        OflpModPanels::     p0_minmax       (OflpPanel* _nn_panel)

12#     void        OpenFilesListPlus:: RefreshOpenFileState    (EditorBase* _nn_edb)
13#     void        OpenFilesListPlus:: RefreshOpenFileLayout   (EditorBase* _nn_edb)
//  ============================================================================
(016)
2015.12.27  Done : in OpenFilesListPlus::evh_workspace_loading_complete(), a
            sort is done on PanelAssignment-s before adding panels
//  ============================================================================
(017)
//  ============================================================================
(018)
2015.12.28  Done : call RefreshOpenFileState() on active editor after settings
            window has been closed in OflpModSettings::settings_window_activated()
//  ============================================================================
(019)
2015.12.28  Done : OflpModPanels::p0_reset() was hard remove of everything -
            was not properly coded yet
//  ============================================================================
(020)
//  ============================================================================
(021)
2016.01.05  Done ( return true )
//  ============================================================================
(022)
2016.01.05  Done : test is_bulk() in OflpPanel ctor
//  ============================================================================
(023)
2016.01.06  Done : just do dres = defaultDragResult;
