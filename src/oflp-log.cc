/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-common.hh"
#include    "oflp-log.hh"
#include    "oflp-plugin-mod-settings.hh"
//  ################################################################################################
//  logs enabled
//  ################################################################################################
#ifdef  OFLP_EMBED__LOG

namespace oflp
{

//  ================================================================================================
//  log frame
//  ================================================================================================
wxColour    A_colour_fun_ent    (   0 , 255 ,   0 );
wxColour    A_colour_log_inf    (  58 , 187 ,  58 );
wxColour    A_colour_log_wng    ( 197 , 126 ,   0 );
wxColour    A_colour_log_err    ( 255 ,   0 ,   0 );
wxColour    A_colour_log_tki    ( 167 , 167 , 167 );
wxColour    A_colour_log_tkw    ( 197 , 126 ,   0 );
wxColour    A_colour_log_tke    ( 255 ,   0 ,   0 );

wxFont      A_fnt_001( 8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxString::FromUTF8("monospace") );
wxFont      A_fnt_002( 8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD,   false, wxString::FromUTF8("monospace") );

wxTextAttr  A_att_001( A_colour_log_inf, wxNullColour, A_fnt_001);
wxTextAttr  A_att_002( A_colour_fun_ent, wxNullColour, A_fnt_002);

Spacer              a_spacer;
LogFrame        *   dw_log_frame    =   NULL;
//  ------------------------------------------------------------------------------------------------
const wxChar*
        Log_spc_wxc()
{
    return a_spacer.wcstr();
}
void    Log_spc_inc()
{
    a_spacer.inc();
}
void    Log_spc_dec()
{
    a_spacer.dec();
}

void    Log_function_mark   (const wxChar* _funcname)
{
    ERGCB_FNE("%s%s%s", wxS("----------  "), _funcname, wxS("  ---------- ") );
}
void    Log_function_enter  (const wxChar* _funcname)
{
    oflp::Log_spc_inc();

    oflp::Log_window_set_text_attr(oflp::A_att_002);
    ERGCB_LOG(wxS("%s%s"), _funcname);
    oflp::Log_window_set_text_attr(oflp::A_att_001);
}
void    Log_function_exit   ()
{
    oflp::Log_spc_dec();
}

void    Log_window_open     (wxWindow* _parent)
{
    if ( dw_log_frame )
    {
        return;
    }

    dw_log_frame    =   new LogFrame( _parent );
    dw_log_frame->Show();
}
void    Log_window_close    ()
{
    if ( ! dw_log_frame )
    {
        return;
    }

    delete dw_log_frame;
    dw_log_frame = NULL;
}

void    Log_console                 (wxString& _wxs)
{
    if ( oflp::modSettings()->log_console() )
        printf("%s\n", _wxs.mb_str(wxConvUTF8).data());
}
//  __ERG_NOTE__ the methods below just check if the LogFrame widget is not NULL. We dont use
//  OflpModSettings->log_window() as for Log_console() above, but we could, it * SHOULD * be
//  coherent since OflpModSettings open / close the window accordingly to the user's choice in the
//  popup menu.
void    Log_window                  (wxString& _wxs)
{
    if ( dw_log_frame )
    {
        dw_log_frame->log( _wxs );
    }
}
void    Log_window_set_text_col_fg  (wxColour& _wx_col)
{
    if ( dw_log_frame )
    {
        dw_log_frame->set_text_col_fg(_wx_col);
    }
}
void    Log_window_set_text_attr    (wxTextAttr& _att)
{
    if ( dw_log_frame )
    {
        dw_log_frame->set_text_attr(_att);
    }
}

}

#endif                                                                                              //  OFLP_EMBED__LOG
//  ################################################################################################
//  logs disabled
//  ################################################################################################
#ifndef OFLP_EMBED__LOG

//  instead of just defining 'class wxWindow', we have to #include wxWidgets stuff
//  else the Log_window_open(wxWindow*) wont be included in the shared library
#include    <wx/window.h>

namespace oflp
{

void                Log_window_open             (wxWindow*) {}
void                Log_window_close            ()          {}

}

#endif                                                                                              //  OFLP_EMBED__LOG



