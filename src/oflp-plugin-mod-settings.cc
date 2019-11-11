/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin-mod-settings.hh"
#include    "oflp-plugin-mod-tooltips.hh"
#include    "oflp-plugin-mod-panels.hh"

#include    "oflp-settings.hh"

#include    "oflp-plugin.hh"
//  ................................................................................................
#define ERG_OFLP_SANITY_CHECKS

void    OflpModSettings::   modinit()
{
    if ( oflp::Modules::Instance()->settings()->emb_log() )
    {

    oflp::Log_window_open( Manager::Get()->GetAppWindow() );

    }
}

void    OflpModSettings::   clicked                     (wxCommandEvent &   _e)
{
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::Settings::action()");
    //  ............................................................................................
    //  ............................................................................................
    if ( ! dw_settings )
        popup(_e);
    else
        popout(_e);

    OFLP_LOG_FUNC_EXIT();
}

void    OflpModSettings::   settings_window_activated   (bool _b)
{
    if ( _b )
    {
        ERGCB_INF( "%s", wxS("SETTINGS ACTIVATED") );
    }
    else
    {
        ERGCB_INF( "%s", wxS("SETTINGS DE-ACTIVATED") );

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
            OpenFilesListPlus::Instance()->RefreshOpenFileState( Manager::Get()->GetEditorManager()->GetActiveEditor() );
    }
}

void    OflpModSettings::   popup                       (wxCommandEvent &   _e)
{
    //  ............................................................................................
    //  create widget
    wxWindow    *   w   =   reinterpret_cast<wxWindow*>(_e.GetEventObject());
    wxRect          r   =   w->GetScreenRect();

    //ERGCB_INF("x[%i] y[%i] h[%i] w[%i]", r.x, r.y, r.height, r.width);

    r.Offset( 0, r.GetHeight() );

    wxPoint         p   =   r.GetPosition();
    wxSize          s   =   r.GetSize();

    dw_settings = new OpenFilesListPlusSettings(w, p, s, a_opt_log, a_opt_sel, a_opt_div_tt, a_opt_colors);

    dw_settings->Connect(
        wxEVT_ACTIVATE                                                      ,
        wxActivateEventHandler(OpenFilesListPlus::evh_settings_activated)   ,
        NULL, OpenFilesListPlus::Instance());

    dw_settings->Show();
    //  ............................................................................................
    //ERG_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

void    OflpModSettings::   popout                      (wxCommandEvent &   _e)
{
    popout();
}

void    OflpModSettings::   popout                      ()
{
    //  ............................................................................................
    dw_settings->xport(a_opt_log, a_opt_sel, a_opt_div_tt, a_opt_colors);

    delete dw_settings;
    dw_settings = NULL;
    //  ............................................................................................
    //  act accordingly to options
    update_from_user_input();
    //  ............................................................................................
    //ERG_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

void    OflpModSettings::   update_from_user_input      ()
{
    if ( oflp::Modules::Instance()->settings()->emb_menu__log() )
    {

    if ( log_enabled() )
    {
        if ( log_window() )
        {
            ERGCB_INF( "%s", wxS("+log window") );
            oflp::Log_window_open( Manager::Get()->GetAppWindow() );
        }
        if ( ! log_window() )
        {
            ERGCB_INF( "%s", wxS("-log window") );
            oflp::Log_window_close();
        }
    }
    else
    {
        oflp::Log_window_close();
    }

    }

    oflp::Modules::Instance()->tooltips()->x_refresh_tooltips_visibility();

    oflp::Modules::Instance()->panels()->x_set_col__bg_p( app_col_bg_p() );
    oflp::Modules::Instance()->panels()->x_set_col__bg_h( app_col_bg_h() );
}

        OflpModSettings::   OflpModSettings()
{
    //  transform some defines options into OflpModSettings variables
    a_embed_log         =   false;
    a_embed_menu__log   =   false;
    a_embed_menu__dbg   =   false;

    #ifdef  OFLP_EMBED__LOG
    a_embed_log = true;
        #ifdef  OFLP_EMBED__MENU_LOG
        a_embed_menu__log = true;
        #endif
    #endif

    #ifdef  OFLP_EMBED__MENU_DBG
    a_embed_menu__dbg  = true;
    #endif

    //  __ERG_NOTE__ Since the settings are not saved on disk, if log is embedded, they will always
    //  be activated, and the LogFrame that will be shown in OflpModSettings::modinit(). Keep
    //  the two synchronized.
    a_opt_log.a_enabled = true;
    a_opt_log.a_window  = true;
    a_opt_log.a_console = false;

    a_opt_sel.sclick(true);

    a_opt_div_tt.a_show = true;

    a_opt_colors.a_bg_h.Set(0xd0, 0xd0, 0xd0);
    a_opt_colors.a_bg_p.Set(0xf2, 0xf2, 0xf2);

    dw_settings = NULL;
}
