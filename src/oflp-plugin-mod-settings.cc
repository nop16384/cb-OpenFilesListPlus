/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin-mod-settings.hh"

#include    "oflp-settings.hh"

#include    "oflp-plugin.hh"
//  ............................................................................
#define GWR_OFLP_SANITY_CHECKS
#define GWR_LOG(FORMAT, ...)    GWRCB_LOG(FORMAT, __VA_ARGS__);
#define GWR_TKI(FORMAT, ...)    GWRCB_TKI(FORMAT, __VA_ARGS__);
#define GWR_TKE(FORMAT, ...)    GWRCB_TKE(FORMAT, __VA_ARGS__);
#define GWR_INF(FORMAT, ...)    GWRCB_INF(FORMAT, __VA_ARGS__);
#define GWR_WNG(FORMAT, ...)    GWRCB_WNG(FORMAT, __VA_ARGS__);
#define GWR_ERR(FORMAT, ...)    GWRCB_ERR(FORMAT, __VA_ARGS__);
//  ############################################################################
//
//                          SETTINGS
//
//  ############################################################################
void    OflpModSettings::   action                      (wxCommandEvent &   _e)
{
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::Settings::action()");
    //  ........................................................................
    //  ........................................................................
    //  widget is shown,
    if ( ! dw_settings )
        popup(_e);
    else
        popout(_e);

    OFLP_FUNC_EXIT_LOG();
}

void    OflpModSettings::   settings_window_activated   (bool _b)
{
    if ( _b )
    {
        GWRCB_INF( "%s", wxS("SETTINGS ACTIVATED") );
    }
    else
    {
        GWRCB_INF( "%s", wxS("SETTINGS DE-ACTIVATED") );

        //  if we close the settings window and user clicked on options
        //  button( which caused the deactivation ), the button_clicked callback
        //  will be called just after us, and the settings window will
        //  re-appear ! So we dont close if mouse is over the opt button
        //  ( thanks to the ::wxGetMousePosition() which avoid event connection
        //    overload )
        wxRect r = dw_settings->parent_screen_rect();
        if ( r.Contains( ::wxGetMousePosition() ) )
            return;

        popout();

        //  cf bugs#18
        if ( Manager::Get()->GetEditorManager()->GetActiveEditor() )
            oflp()->RefreshOpenFileState( Manager::Get()->GetEditorManager()->GetActiveEditor() );
    }
}

void    OflpModSettings::   popup                       (wxCommandEvent &   _e)
{
    //  ........................................................................
    //  ........................................................................
    //  ........................................................................
    //  create widget
    wxWindow    *   w   =   reinterpret_cast<wxWindow*>(_e.GetEventObject());
    wxRect          r   =   w->GetScreenRect();

    GWRCB_INF("x[%i] y[%i] h[%i] w[%i]", r.x, r.y, r.height, r.width);

    r.Offset( 0, r.GetHeight() );

    wxPoint         p   =   r.GetPosition();
    wxSize          s   =   r.GetSize();

    dw_settings = new OpenFilesListPlusSettings(w, p, s, a_opt_log, a_opt_sel);

    dw_settings->Connect(
        wxEVT_ACTIVATE                                                      ,
        wxActivateEventHandler(OpenFilesListPlus::evh_settings_activated)   ,
        NULL, oflp());

    dw_settings->Show();
    //  ........................................................................
    //GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

void    OflpModSettings::   popout                      (wxCommandEvent &   _e)
{
    popout();
}

void    OflpModSettings::   popout                      ()
{
    //  ........................................................................
    //  ........................................................................
    //  ........................................................................
    dw_settings->output(a_opt_log, a_opt_sel);

    delete dw_settings;
    dw_settings = NULL;
    //  ........................................................................
    //  act accordingly to options
    update_from_user_input();
    //  ........................................................................
    //GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

void    OflpModSettings::   update_from_user_input      ()
{
    if ( log_enabled() )
    {
        oflp::A_log_window    =   log_window();
        oflp::A_log_console   =   log_console();

        if ( log_window() )
        {
            GWR_INF( "%s", wxS("+log window") );
            oflp::Log_window_open( Manager::Get()->GetAppWindow() );
        }
        if ( ! log_window() )
        {
            GWR_INF( "%s", wxS("-log window") );
            oflp::Log_window_close();
        }
    }
    else
    {
        oflp::A_log_window    =   false;
        oflp::A_log_console   =   false;

        oflp::Log_window_close();
    }
}
//  ############################################################################
        OflpModSettings::   OflpModSettings()
{
    a_opt_log.enabled    = true;
    a_opt_log.window     = true;
    a_opt_log.console    = false;

    a_opt_sel.set_dclick(true);

    dw_settings = NULL;
}
