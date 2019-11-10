/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_COMMON_HH__
#define __OFLP_COMMON_HH__

//  ................................................................................................
//  wx
//  ................................................................................................
//  for compilers that support precompilation, includes <wx/wxprec.h>
#include <wx/wxprec.h>

//  else include normal wx headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
//  ................................................................................................
//  C::B sdk
//  ................................................................................................
#include <sdk.h>

#ifndef CB_PRECOMP

    #pragma message "-CBPRECOMP"

    #include <cbplugin.h>                                             //  for "class cbPlugin"
    #include <manager.h>
    #include <configmanager.h>
    #include <configurationpanel.h>
    #include <editormanager.h>
    #include <projectmanager.h>
    #include <logmanager.h>
    #include <editorbase.h>
    #include <cbeditor.h>
    #include <sdk_events.h>
    #include <misctreeitemdata.h>

    #include <cbworkspace.h>
    #include <cbproject.h>
    #include <projectmanager.h>
#else
    #pragma message "+CBPRECOMP"
#endif
//  ................................................................................................
//  general defines - compilation options
//  ................................................................................................
#include    "oflp-common-defines.hh"
#include    "oflp-log.hh"                                                                           //  OFLP plugin log system
//#include    "oflp-globals.hh"
//  ................................................................................................

//  ................................................................................................
//  some macros
//#include    "oflp-common-macros.hh"
//  ................................................................................................
//  declare all classes ( and some aliases ) for header files
#include    "oflp-common-classes.hh"
//  ................................................................................................
/*
WX_DEFINE_ARRAY(        EditorBase  *, EditorArray);
WX_DEFINE_ARRAY(        OflpPanel   *, OflpPanelArray);

WX_DECLARE_HASH_MAP(    int                 ,
                        wxBitmap*           ,
                        wxIntegerHash       ,
                        wxIntegerEqual      ,
                        BitmapPointerHash   );
*/
//  ................................................................................................
//  containers custom accessors for :
//      - avoiding modifying a container while looping on it
//  - The same macros as OFLP_STL_FOR etc... defined in oflp-common-macros.hhi and
//    using the custom containers are also defined.
//  - "devel" target use the custom
//    containers, "prod" target dont and use classical wx containers
/*
#ifdef  OFLP_COMPILE_TARGET_DEVEL
#include    "oflp-common-stl-devel.hhi"
#endif
#ifdef  OFLP_COMPILE_TARGET_PROD
#include    "oflp-common-stl-prod.hhi"
#endif
*/
//  ................................................................................................



#endif                                                                          //  __OFLP_COMMON_HH__
