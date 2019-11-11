In :
  evh_tree_item_activated()
  evh_tree_item_right_click()
  evh_tree_sel_changed()
No emergency is done if OflpPanelItemInfo object cant bre constructed from event
because this sometimes happens.
//  ================================================================================================
wxwidgets verion macros :
wxMAJOR_VERSION 	The major version of wxWidgets
wxMINOR_VERSION 	The minor version of wxWidgets
wxRELEASE_NUMBER 	The release number
wxSUBRELEASE_NUMBER 	The subrelease number which is 0 for all official releases
//  ================================================================================================
C::B environment variables for compiling OpenFilesListPlus
----------------------------------------------------------
When you compile a C::B plugin, you compile it for a particular version of C::B, which itself was
configured for a particular version of wxWidgets. The following environment variables have to be
defined for compiling OpenFilesListPlus :

    OFLP_CBSRC      directory of C::B sources you want to compile against
    OFLP_WXVERSION  version of wxWidgets the above C::B sources use
    OFLP_WXTOOLKIT  toolkit to use ( ex:for linux, gtk2 )
    OFLP_UNICODE    [yes|no] whether to use an unicode version of wxWidgets ; choose the same as
                    the above C::B sources use
    OFLP_WXDEBUG    [yes|no] wether to use a debug or release eversion of wxWidgets

//  ================================================================================================
BUILDING_PLUGIN
WXUSINGDLL
cbDEBUG
CB_PRECOMP
WX_PRECOMP
wxUSE_UNICODE
//  ================================================================================================
ProjectFile :
wxFileName 	file                            The full filename of this file.
wxString 	relativeFilename 	            The relative (to the project) filename of this file.
wxString 	relativeToCommonTopLevelPath    The relative filename to the common top-level path.
//  ================================================================================================
DND synopsys:

Init :
- All DnD src are connected to a DRAG_INIT event
- All DnD dst are associated with a wxDropTarget ; the wxDropTarget is associated with a wxDataObject ( DtObj )

Core :
- DRAG_INIT event occurs : the drag src fills a wxDataObject (DsObj) , and calls DoDragAndDrop()
- Drop done :
    1) DndDst->OnDrop() is called, have to return true
    2) DtObj->OnData() is called with its data copied from DsObj

OFLP_LOG should imply OFLP_MENU__LOG









