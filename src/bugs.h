+(001)  2015.08.21  When editor change from ofl, focus stay on ofl, is not given
                    to editor

 (002)  2015.08.21  When editing OFLPPanel title, the wxTextCtrl remains if
                    user click anywhere else ; should switch back to wxStaticText

 (003)  2015.08.21  After DnD selection of item is lost

+(004)  2015.08.21  Double loop : mouse -> activate editor -> select editor -> OnTreeItemSelected -> refresh -> select editor

 (005)  2015.08.22  Double call to OFLPPanelHeader::switch_to_static()
                    because of TEXT_ENTER _AND_ KILL_FOCUS events

 (006)  2015.08.29  libxxx.so vs xxx.so in zip file & lib loading

 (007)  2015.08.30  When closing log window, menu options is not refreshed

 (008)  2015.08.30  View menu misplacement

 (009)  2015.08.30  C::B OFLP window title !

 (010)  2015.08.30  I get bunch of cb debug in my wxLog window

 (011)  2015.09.06  Saving file as a different name dont change the wxTreeItem text !!!

 (012)  2015.09.08  Bug : when error clicked on in "build log", file not selected
                    in any panel !
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
//  ============================================================================
(005)
2015.08.23  done : flag OFLPPanel::a_allow_kill_focus_event
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
2015.09.12  Done : item name verification was commented in OFLPPanel::editor_sync()
//  ============================================================================
(012)
2015.09.08  Done : wxTreeCtrl->GetSelection() is buggy - removed all calls

