/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PLUGIN_MOD_SETTINGS_HH__
#define __OFLP_PLUGIN_MOD_SETTINGS_HH__
//  ................................................................................................
#include    "oflp-common.hh"

#include    "oflp-plugin-module.hh"

#include    "oflp-settings-options.hh"
/// ************************************************************************************************
//! \class  OflpModSettings
//!
//! \brief  Contain all OFLP settings ( options, etc... )
/// ************************************************************************************************
class OflpModSettings       : public OflpModule
{
    friend class OpenFilesListPlus;
    //  --------------------------------------------------------------------------------------------
  private:
    OflpOptLog          a_opt_log;
    OflpOptSel          a_opt_sel;
    OflpOptDivTt        a_opt_div_tt;
    OflpOptColors       a_opt_colors;

  public:
    bool                mode_standard       ()  { return a_opt_sel.dclick();    }
    bool                mode_productivity   ()  { return a_opt_sel.sclick();    }
    bool                log_enabled         ()  { return a_opt_log.a_enabled;   }
    bool                log_console         ()  { return a_opt_log.a_console;   }
    bool                log_window          ()  { return a_opt_log.a_window;    }
    bool                div_tooltip_show    ()  { return a_opt_div_tt.a_show;   }
    wxColour       &    app_col_bg_p        ()  { return a_opt_colors.a_bg_p;   }
    wxColour       &    app_col_bg_h        ()  { return a_opt_colors.a_bg_h;   }
  //  --------------------------------------------------------------------------------------------
  private:
    OflpSettings    *   dw_settings;
    //  --------------------------------------------------------------------------------------------
  protected:
    void                action                      (wxCommandEvent&);
    void                settings_window_activated   (bool _b);

    void                popup   (wxCommandEvent&);
    void                popout  (wxCommandEvent&);
    void                popout  ();
    void                update_from_user_input();
    //  --------------------------------------------------------------------------------------------
  private:
    virtual     void    z_release() {}
    //  --------------------------------------------------------------------------------------------
  public:
    OflpModSettings();
   ~OflpModSettings()   {}
};

#endif
