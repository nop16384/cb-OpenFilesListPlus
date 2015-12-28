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
#define GWR_ITERATE_V(C, V, IT)                                                 \
    for ( C::iterator IT = (V).begin() ; IT != (V).end() ; IT++ )
#define GWR_ITERATE_VC(V, IT)                                                   \
    for ( C::const_iterator IT = (V).begin() ; IT != (V).end() ; IT++ )
//  ............................................................................
#define GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF()                                   \
    lab_exit_success:                                                           \
        earlgreycb::Log_function_exit();                                        \
        return true;                                                            \
                                                                                \
    lab_exit_failure:                                                           \
        earlgreycb::Log_function_exit();                                        \
        return false;

#define GWR_LABELS_EXIT_SUCCESS_FAILURE_RET()                                   \
    lab_exit_success:                                                           \
        earlgreycb::Log_function_exit();                                        \
        return;                                                                 \
                                                                                \
    lab_exit_failure:                                                           \
        earlgreycb::Log_function_exit();                                        \
        return;
//  ............................................................................
//! Return immediately on degraded mode
#define OFLP_ON_DEGRADED__RET()                                                 \
    if ( degraded() )                                                           \
        return;
//! Set emergency mode and return on null pointer
#define OFLP_EMERGENCY__NULL_POINTER__RET(P)                                    \
    if ( ! P )                                                                  \
    {                                                                           \
        emergency();                                                            \
        return;                                                                 \
    }
//! Set emergency mode and goto lab_failure on null pointer
#define OFLP_EMERGENCY__NULL_POINTER__FAILURE(P)                                \
    if ( ! P )                                                                  \
    {                                                                           \
        oflp()->emergency();                                                    \
        goto lab_exit_failure;                                                  \
    }
//! Set emergency mode and return if ( CALL() != 0 )
#define OFLP_EMERGENCY__CALL__RET(C)                                            \
    if ( ! (C) )                                                                \
    {                                                                           \
        emergency();                                                            \
        return;                                                                 \
    }
//  ............................................................................
class   OflpOptLog
{
  public:
    bool    enabled;
    bool        window;
    bool        console;
};
class   OflpOptSel
{
  private:
    bool    a_sclick;
    bool    a_dclick;

  public:
    bool    sclick()            { return a_sclick; }
    bool    dclick()            { return a_dclick; }
    void    set_sclick(bool _b) { a_sclick = _b ; a_dclick = !_b; }
    void    set_dclick(bool _b) { a_dclick = _b ; a_sclick = !_b; }

  public:
    OflpOptSel()    { set_dclick(true); }
};
//  ............................................................................
//  declare all classes ( and some aliases ) for header files
class   EditorBase;

class   OpenFilesListPlus;

class   OpenFilesListPlusPanel;
class   OpenFilesListPlusHeader;
class   OpenFilesListPlusPanelHeader;
class   OpenFilesListPlusPanelItemData;
class   OpenFilesListPlusPanelItemInfo;
class   OpenFilesListPlusPanelOptions;
class   OpenFilesListPlusSettings;

typedef wxPanel                         OFLPCtrl;
typedef OpenFilesListPlusPanel          OFLPPanel;
typedef OpenFilesListPlusPanelItemData  OFLPPanelItemData;
typedef OpenFilesListPlusPanelItemInfo  OFLPPanelItemInfo;
typedef OpenFilesListPlusHeader         OFLPHeader;
typedef OpenFilesListPlusPanelHeader    OFLPPanelHeader;
typedef OpenFilesListPlusPanelOptions   OFLPPanelOptions;
typedef OpenFilesListPlusSettings       OFLPSettings;

class   OflpModGfx;
class   OflpModLayout;
class   OflpModPanels;
class   OflpModEditors;
class   OflpModMenuOptions;
class   OflpModSettings;
//  ............................................................................
WX_DEFINE_ARRAY(        EditorBase              *, EditorArray);
WX_DEFINE_ARRAY(        OpenFilesListPlusPanel  *, PanelArray);
WX_DECLARE_HASH_MAP(    int                 ,
                        wxBitmap*           ,
                        wxIntegerHash       ,
                        wxIntegerEqual      ,
                        BitmapPointerHash   );
//  ............................................................................
#endif                                                                          //  __OFLP_COMMON_HH__
