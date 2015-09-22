 (001)  2015.08.21  Obtain keyboard focus in text control in OFLPPanelHeader
                    when changing the title

 (002)  2015.08.21  Terminate standard / productivity mode

+(003)  2015.08.21  Rescan all old methods for compatibility

 (004)  2015.08.21  Save layout & options

 (005)  2015.08.21  Verify icons

 (006)  2015.08.21  Possibility to hide the wxTreeCtrl

 (007)  2015.08.22  Hide up & down buttons when panel is minimized

+(008)  2015.08.28  - in gfx->bitmap(), check icon index bounds for image list
                    - shows a dummy bitmap if file not found

+(009)  2015.08.29  change font in popop menu

+(010)  2015.08.30  Better panels resize : avoid vertical scrollbar if space
                    is available in other panels

 (011)  2015.08.30  Put png files inside zip file

+(012)  2015.09.06  Memory leaks ?

 (013)  2015.09.12  Select active editor after workspace laoding

 (014)  2015.09.12  Put up & down arrows near mini/maxi-mize button in OFLPPanelHeader

 (015)  2015.09.13  Layout : apply() : rewrite for handling all cases

+(016)  2015.09.13  Layout : C::B save everything relationships

+(017)  2015.09.13  Layout : OFLPPanels ordering at loading

 (018)  2015.09.13  OFLP::on_editor_modified() : DONT call refresh for
                    a letter typed !!!

+(019)  2015.09.13  Unique panel names / workspace

+(020)  2015.09.15  boolean return values check

 (021)  2015.09.19  oflp-plugin.hh : remove "friend class OFLPPanel" declaration

 (022)  2015.09.20  "An inner class is a friend of the class it is defined within."
                    find something else, every module has access to everything !!!
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
2015.09.12  Done : layout import
2015.09.20  Done : finished import / export
//  ============================================================================
(005)
2015.08.??  Done, installed in the tree
//  ============================================================================
(006)
2015.08.22  done
//  ============================================================================
(007)
2015.08.22  done
//  ============================================================================
(008)
2015.08.29  - bitmap identifiers as enum in OpenFilesListPlus
//  ============================================================================
(009)
//  ============================================================================
(010)
2015.09.2   Done first version of Panels::p0_resize()
//  ============================================================================
(011)
2015.08.30  Done :
            * create xrc file -> symbolic names
            * zip png files & xrc file in plugin.zip
            * wxXmlResource::Get()->Load( symbolic name );
//  ============================================================================
(012)
//  ============================================================================
(013)
2015.09.17  Done : in cbEVT_EDITOR_ACTIVATED handler
//  ============================================================================
(014)
2015.09.20  Done
//  ============================================================================
(015)
2015.09.17  Done : apply() stuff has been moved in OFLP event handlers
//  ============================================================================
(016)
//  ============================================================================
(017)
//  ============================================================================
(018)
2015.09.17  Done : bad remark - "modified" event is not sent when a letter
            is typed, but only when the file was modified
//  ============================================================================
(019)
//  ============================================================================
(020)
//  ============================================================================
(021)
2015.09.??  Done
//  ============================================================================
(022)
2015.09.21  Done
