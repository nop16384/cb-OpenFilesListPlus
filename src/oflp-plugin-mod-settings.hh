/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PLUGIN_MOD_SETTINGS_HH__
#define __OFLP_PLUGIN_MOD_SETTINGS_HH__
//  ............................................................................
#include    "oflp-common.hh"

#include    "oflp-plugin-module.hh"

#include    "oflp-settings-options.hh"
/// ****************************************************************************
//! \class  OflpModSettings
//!
//! \brief  Contain all OFLP settings ( options, etc... )
/// ****************************************************************************
class OflpModSettings       : public OflpModule
{
    friend class OpenFilesListPlus;
    //  ------------------------------------------------------------------------
  public:
    enum
    {
        eFILE_SELECTION_MODE_STANDARD          =   0x0001  ,
        eFILE_SELECTION_MODE_PRODUCTIVITY      =   0x0002
    };
    //  ------------------------------------------------------------------------
  private:
    struct  OflpOptLog  a_opt_log;
    struct  OflpOptSel  a_opt_sel;

  protected:
    bool                mode_standard       ()  { return a_opt_sel.dclick();    }
    bool                mode_productivity   ()  { return a_opt_sel.sclick();    }
    bool                log_enabled         ()  { return a_opt_log.enabled;     }
    bool                log_console         ()  { return a_opt_log.console;     }
    bool                log_window          ()  { return a_opt_log.window;      }
    //  ------------------------------------------------------------------------
  private:
    OflpSettings    *   dw_settings;
    //  ........................................................................
  protected:
    void                action                      (wxCommandEvent&);
    void                settings_window_activated   (bool _b);

    void                popup   (wxCommandEvent&);
    void                popout  (wxCommandEvent&);
    void                popout  ();
    void                update_from_user_input();
    //  ------------------------------------------------------------------------
  public:
    OflpModSettings();
   ~OflpModSettings()   {}
};

#endif
