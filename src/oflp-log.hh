/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __GWR_CB_LOG_HH__
#define __GWR_CB_LOG_HH__
//  ............................................................................
#include    <stdio.h>
//  ****************************************************************************
//  logs enabled
//  ****************************************************************************
#ifdef  OFLP_LOG

#include    <wx/string.h>
#include    <wx/colour.h>
#include    <wx/textctrl.h>

class   wxWindow;

namespace   oflp
{

extern  wxColour    A_colour_fun_ent;

extern  wxColour    A_colour_log_inf;
extern  wxColour    A_colour_log_wng;
extern  wxColour    A_colour_log_err;
extern  wxColour    A_colour_log_tki;
extern  wxColour    A_colour_log_tkw;
extern  wxColour    A_colour_log_tke;
//  ............................................................................
extern  bool        A_log_console;
extern  bool        A_log_window;

extern  void        Log_console                 (wxString&);
extern  void        Log_window                  (wxString&);
extern  void        Log_window_set_text_col_fg  (wxColour&);
extern  void        Log_window_set_text_attr    (wxTextAttr&);
extern  bool        Log_window_opened           ();
extern  void        Log_window_open             (wxWindow*);
extern  void        Log_window_close            ();

extern          void            Log_spc_inc();
extern          void            Log_spc_dec();
extern  const   wxChar      *   Log_spc_wxc();

extern          void            Log_function_mark   (const wxChar*);
extern          void            Log_function_enter  (const wxChar*);
extern          void            Log_function_exit   ();

extern          int             Log_szt2int         (size_t _szt);

//  ............................................................................
#define     GWRCB_LOG(WXFORMAT, ...)                                            \
{                                                                               \
    wxString tmp =                                                              \
        wxString::Format(                                                       \
            WXFORMAT                    ,                                       \
            oflp::Log_spc_wxc()         ,                                       \
            __VA_ARGS__                 );                                      \
                                                                                \
    if ( oflp::A_log_console )                                                  \
    {                                                                           \
        oflp::Log_console  ( tmp );                                             \
    }                                                                           \
    if ( oflp::A_log_window )                                                   \
    {                                                                           \
        oflp::Log_window   ( tmp );                                             \
    }                                                                           \
}
//  ............................................................................
#define GWRCB_FNE(FORMAT, ...)                                                  \
{                                                                               \
    oflp::Log_window_set_text_col_fg(oflp::A_colour_fun_ent);                   \
    GWRCB_LOG(wxS("%s"        FORMAT), __VA_ARGS__);                            \
}

#define GWRCB_TKI(FORMAT, ...)                                                  \
{                                                                               \
    oflp::Log_window_set_text_col_fg(oflp::A_colour_log_tki);                   \
    GWRCB_LOG(wxS("%s" "tki:" FORMAT), __VA_ARGS__);                            \
}
#define GWRCB_TKW(FORMAT, ...)                                                  \
{                                                                               \
    oflp::Log_window_set_text_col_fg(oflp::A_colour_log_tkw);                   \
    GWRCB_LOG(wxS("%s" "tkw:" FORMAT), __VA_ARGS__);                            \
}
#define GWRCB_TKE(FORMAT, ...)                                                  \
{                                                                               \
    oflp::Log_window_set_text_col_fg(oflp::A_colour_log_tke);                   \
    GWRCB_LOG(wxS("%s" "tke:" FORMAT), __VA_ARGS__);                            \
}
#define GWRCB_INF(FORMAT, ...)                                                  \
{                                                                               \
    oflp::Log_window_set_text_col_fg(oflp::A_colour_log_inf);                   \
    GWRCB_LOG(wxS("%s" "inf:" FORMAT), __VA_ARGS__);                            \
}
#define GWRCB_WNG(FORMAT, ...)                                                  \
{                                                                               \
    oflp::Log_window_set_text_col_fg(oflp::A_colour_log_wng);                   \
    GWRCB_LOG(wxS("%s" "wng:" FORMAT), __VA_ARGS__);                            \
}
#define GWRCB_ERR(FORMAT, ...)                                                  \
{                                                                               \
    oflp::Log_window_set_text_col_fg(oflp::A_colour_log_err);                   \
    GWRCB_LOG(wxS("%s" "err:" FORMAT), __VA_ARGS__);                            \
}

//  ............................................................................
#ifdef  OFLP_LOG_FUNCTIONS

#define OFLP_LOG_FUNC_ENTER(FUNCNAME)                                           \
        oflp::Log_function_enter(wxS(FUNCNAME));

#define OFLP_LOG_FUNC_EXIT()                                                    \
        oflp::Log_function_exit();

#define OFLP_LOG_FUNC_INC()                                                     \
        oflp::Log_spc_inc();

#define OFLP_LOG_FUNC_DEC()                                                     \
        oflp::Log_spc_dec();

#define OFLP_FUNC_ENTER_MARK(FUNCNAME)                                          \
        oflp::Log_function_mark(wxS(FUNCNAME));

#else

#define OFLP_LOG_FUNC_ENTER(FUNCNAME)
#define OFLP_LOG_FUNC_EXIT()
#define OFLP_FUNC_ENTER_MARK(FUNCNAME)

#endif                                                                          //  #ifdef OFLP_LOG_FUNCTIONS

}

#endif                                                                          //  #ifdef OFLP_LOG
//  ****************************************************************************
//  logs disabled
//  ****************************************************************************
#ifndef OFLP_LOG

class   wxWindow;

namespace oflp
{

extern  bool        A_log_console;
extern  bool        A_log_window;

extern  void        Log_window_open             (wxWindow*);
//extern  void        Log_window_open             ();
extern  void        Log_window_close            ();

//  ............................................................................
#define GWRCB_LOG(WXFORMAT, ...)
//  ............................................................................
#define GWRCB_TCS(FORMAT, ...)

#define GWRCB_TKI(FORMAT, ...)
#define GWRCB_TKW(FORMAT, ...)
#define GWRCB_TKE(FORMAT, ...)

#define GWRCB_INF(FORMAT, ...)
#define GWRCB_WNG(FORMAT, ...)
#define GWRCB_ERR(FORMAT, ...)
//  ............................................................................
#define OFLP_LOG_FUNC_ENTER(FUNCNAME)
#define OFLP_LOG_FUNC_EXIT()
#define OFLP_LOG_FUNC_INC()
#define OFLP_LOG_FUNC_DEC()
#define OFLP_FUNC_ENTER_MARK(FUNCNAME)

}

#endif                                                                          //  #ifndef OFLP_LOG

#endif                                                                          //  __GWR_CB_LOG_HH__
