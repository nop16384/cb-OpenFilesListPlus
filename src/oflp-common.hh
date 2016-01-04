/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_COMMON_HH__
#define __OFLP_COMMON_HH__
//  ............................................................................
//  wx
//  for compilers that support precompilation, includes <wx/wxprec.h>
#include <wx/wxprec.h>

//  else include normal wx headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
//  ............................................................................
//  C::B sdk
#include <codeblocks/sdk.h>

#ifndef CB_PRECOMP
    #include <codeblocks/manager.h>
    #include <codeblocks/configmanager.h>
    #include <codeblocks/configurationpanel.h>
    #include <codeblocks/editormanager.h>
    #include <codeblocks/projectmanager.h>
    #include <codeblocks/logmanager.h>
    #include <codeblocks/editorbase.h>
    #include <codeblocks/cbeditor.h>
    #include <codeblocks/sdk_events.h>
    #include <codeblocks/misctreeitemdata.h>

    #include <codeblocks/cbworkspace.h>
    #include <codeblocks/cbproject.h>
    #include <codeblocks/projectmanager.h>
#endif
//  ............................................................................
//  general defines - compilation options
#define EARLGREY_USE_STL_ACCESSORS
//  ............................................................................
#include    "oflp-log.hh"                                                       //  OFLP plugin log system
#include    "oflp-hstring.hh"                                                   //  HString
//  ............................................................................
#define OFLP_FILE_VERSION_MAJOR_STR     "1"
#define OFLP_FILE_VERSION_MINOR_STR     "0"
#define OFLP_FILE_VERSION_MAJOR_NUM     1
#define OFLP_FILE_VERSION_MINOR_NUM     0
//  ............................................................................
//  some macros
#include    "oflp-common-macros.hh"
//  ............................................................................
//  declare all classes ( and some aliases ) for header files
#include    "oflp-common-classes.hh"
//  ............................................................................
WX_DEFINE_ARRAY(        EditorBase  *, EditorArray);
WX_DEFINE_ARRAY(        OflpPanel   *, OflpPanelArray);

WX_DECLARE_HASH_MAP(    int                 ,
                        wxBitmap*           ,
                        wxIntegerHash       ,
                        wxIntegerEqual      ,
                        BitmapPointerHash   );
//  ............................................................................
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
//  ............................................................................
#endif                                                                          //  __OFLP_COMMON_HH__
