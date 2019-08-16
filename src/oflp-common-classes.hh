#ifndef __OFLP_COMMON_CLASSES_HH__
#define __OFLP_COMMON_CLASSES_HH__

//  declare all classes ( and some aliases ) for header files
class   EditorBase;

class   OpenFilesListPlus;

class   OpenFilesListPlusHeader;
class   OpenFilesListPlusPanel;
class   OpenFilesListPlusPanelHeader;
class   OpenFilesListPlusPanelTreeItemData;
class   OpenFilesListPlusPanelTreeItemEventInfo;
class   OpenFilesListPlusSettings;

class   OflpPanelDndDataObject;
class   OpenFilesListPlusPanelDropTarget;

typedef OpenFilesListPlusHeader                 OflpHeader;
typedef OpenFilesListPlusPanel                  OflpPanel;
typedef OpenFilesListPlusPanelHeader            OflpPanelHeader;
typedef OpenFilesListPlusPanelTreeItemData      OflpPanelTiData;
typedef OpenFilesListPlusPanelTreeItemEventInfo OflpPanelTiEventInfo;
typedef OpenFilesListPlusSettings               OflpSettings;

class   OflpModGfx;
class   OflpModLayout;
class   OflpModPanels;
class   OflpModMenuOptions;
class   OflpModSettings;
class   OflpModRunflow;
class   OflpModTooltips;
class   OflpModFLog;

WX_DEFINE_ARRAY(        EditorBase  *, EditorArray);

WX_DEFINE_ARRAY(        OflpPanel   *, OflpPanelArray);

WX_DECLARE_HASH_MAP(    int                 ,
                        wxBitmap*           ,
                        wxIntegerHash       ,
                        wxIntegerEqual      ,
                        BitmapPointerHash   );

#endif

