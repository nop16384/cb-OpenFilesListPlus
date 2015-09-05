 (001)  2015.08.21  Obtain keyboard focus in text control in OFLPPanelHeader
                    when changing the title

 (002)  2015.08.21  Terminate standard / productivity mode

+(003)  2015.08.21  Rescan all old methods for compatibility

+(004)  2015.08.21  When editor change from ofl, focus stay on ofl, is not given
                    to editor

 (005)  2015.08.21  When editing OFLPPanel title, the wxTextCtrl remains if
                    user click anywhere else ; should switch back to wxStaticText

+(006)  2015.08.21  Save layout & options

 (007)  2015.08.21  After DnD selection of item is lost

 (008)  2015.08.21  Verify icons

+(009)  2015.08.21  Double loop : mouse -> activate editor -> select editor -> OnTreeItemSelected -> refresh -> select editor

 (010)  2015.08.21  Possibility to hide the wxTreeCtrl

 (011)  2015.08.22  Double call to OFLPPanelHeader::switch_to_static()
                    because of TEXT_ENTER _AND_ KILL_FOCUS events

 (012)  2015.08.22  Hide up & down buttons when panel is minimized

+(013)  2015.08.28  - in gfx->bitmap(), check icon index bounds for image list
                    - shows a dummy bitmap if file not found

+(014)  2015.08.29  change font in popop menu

 (015)  2015.08.29  libxxx.so vs xxx.so in zip file & lib loading

+(016)  2015.08.30  Better panels resize : avoid vertical scrollbar if space
                    is available in other panels

 (017)  2015.08.30  When closing log window, menu options is not refreshed

 (018)  2015.08.30  View menu misplacement

 (019)  2015.08.30  C::B OFLP window title !

 (020)  2015.08.30  Put png files inside zip file

 (021)  2015.08.30  I get bunch of cb debug in my wxLog window
//  ############################################################################
(001)
2015.08.21  Done: call to SetFocus() )
//  ============================================================================
(002)
2015.08.21  Done
//  ============================================================================
(003)
2015.08.21  done : cbEVT_EDITOR_ACTIVATED
              RefreshOpenFilesTree() : select item if editor activated from tabs
2015.08.21  done : cbEVT_EDITOR_MODIFIED
            done : cbEVT_EDITOR_SAVED
              RefreshOpenFilesTree() : name + icon + tree re-sorting
//  ============================================================================
(004)
//  ============================================================================
(005)
2015.08.21  Done: by handling wxTextCtrl wxEVT_KILL_FOCUS event
//  ============================================================================
(006)
//  ============================================================================
(007)
2015.08.??  Done
//  ============================================================================
(008)
2015.08.??  Done, installed in the tree
//  ============================================================================
(009)
2015.09.02  - resizing the log window causes a cbEventEditorActivated
//  ============================================================================
(010)
2015.08.22  done
//  ============================================================================
(011)
2015.08.23  done : flag OFLPPanel::a_allow_kill_focus_event
//  ============================================================================
(012)
2015.08.22  done
//  ============================================================================
(013)
2015.08.29  - bitmap identifiers as enum in OpenFilesListPlus
//  ============================================================================
(014)
//  ============================================================================
(015)
2015.08.30  Done
//  ============================================================================
(016)
//  ============================================================================
(017)
2015.08.30  Done : subclassing wxLogWindow and disallowing closing it directly
//  ============================================================================
(018)
2015.08.30  Done : without doing nothing ! surely related to (019)
//  ============================================================================
(019)
2015.08.30  Done : in OnAttach(), change the name of the CodeBlocksDockEvent - it
            was the same as in OpenFilesListPlugin, that caused the bug
//  ============================================================================
(020)
2015.08.30  Done :
            * create xrc file -> symbolic names
            * zip png files & xrc file in plugin.zip
            * wxXmlResource::Get()->Load( symbolic name );
//  ============================================================================
(021)
2015.09.01  Done : by replacing the wxLog...stuff by a wxFrame + wxTextCtrl
