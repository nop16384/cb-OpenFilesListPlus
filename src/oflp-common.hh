#ifndef __OFLP_COMMON_HH__
#define __OFLP_COMMON_HH__
//  ............................................................................
//  for compilers that support precompilation, includes <wx/wxprec.h>
#include <wx/wxprec.h>
//  else include normal wx headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
//  ............................................................................
#include    "earlgrey-cb.log.hh"                                                //  OFLP plugin log system
#include    "earlgrey-cb.hstring.hh"                                            //  earlgreycb::HString
//  ............................................................................
#define OFLP_FILE_VERSION_MAJOR_STR     "1"
#define OFLP_FILE_VERSION_MINOR_STR     "0"
#define OFLP_FILE_VERSION_MAJOR_NUM     1
#define OFLP_FILE_VERSION_MINOR_NUM     0
//  ............................................................................
//  declare all classes ( and some aliases ) for header files
class   EditorBase;

class   OpenFilesListPlus;

class   OpenFilesListPlusPanel;
class   OpenFilesListPlusPanelHeader;
class   OpenFilesListPlusPanelItemData;
class   OpenFilesListPlusPanelItemInfo;

typedef wxPanel                         OFLPCtrl;
typedef OpenFilesListPlusPanel          OFLPPanel;
typedef OpenFilesListPlusPanelItemData  OFLPPanelItemData;
typedef OpenFilesListPlusPanelItemInfo  OFLPPanelItemInfo;
typedef OpenFilesListPlusPanelHeader    OFLPPanelHeader;

//  ............................................................................
WX_DEFINE_ARRAY(EditorBase*    , EditorArray);
//  ............................................................................
#endif                                                                          //  __OFLP_COMMON_HH__
