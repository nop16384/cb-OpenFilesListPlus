/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_DEFINES_HH__
#define __OFLP_DEFINES_HH__

//  ................................................................................................
#define OFLP_FILE_VERSION_MAJOR_STR     "1"
#define OFLP_FILE_VERSION_MINOR_STR     "0"
#define OFLP_FILE_VERSION_MAJOR_NUM     1
#define OFLP_FILE_VERSION_MINOR_NUM     0
//  ................................................................................................
//  global log capability
//  ................................................................................................
#define     OFLP_LOG
//  ................................................................................................
//  log entry & exit of functions
//  ................................................................................................
#define     OFLP_LOG_FUNCTIONS
//  ................................................................................................
//  per c++ module logs
//  ................................................................................................
#define     OFLP_LOG_DND                                                                            //  2 modules : plugin-dnd & panel-utils-dnd
#define     OFLP_LOG_PLUGIN_EVENTS

#define     OFLP_LOG_PLUGIN_MOD_PANELS
#define     OFLP_LOG_PLUGIN_MOD_LAYOUT
#define     OFLP_LOG_PLUGIN_MOD_TOOLTIPS

//  #define     OFLP_LOG_PANEL
//  #define     OFLP_LOG_PANEL_HEADER
//  ................................................................................................



#endif                                                                                              //  __OFLP_DEFINES_HH__
