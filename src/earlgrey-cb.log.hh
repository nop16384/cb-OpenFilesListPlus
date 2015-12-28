#ifndef __GWR_CB_LOG_HH__
#define __GWR_CB_LOG_HH__
//  ............................................................................
#include    <stdio.h>

#include    <wx/string.h>
//  ............................................................................
class   wxWindow;

namespace   earlgreycb
{

extern  bool    A_log_console;
extern  bool    A_log_window;
//  ............................................................................
extern  void    Log_console         (wxString&);
extern  void    Log_window          (wxString&);
extern  bool    Log_window_opened   ();
extern  void    Log_window_open     (wxWindow*);
extern  void    Log_window_close    ();

extern          void            Log_spc_inc();
extern          void            Log_spc_dec();
extern  const   wxChar      *   Log_spc_wxc();

extern          void            Log_function_mark   (const wxChar*);
extern          void            Log_function_enter  (const wxChar*);
extern          void            Log_function_exit   ();
//  ............................................................................
#define GWR_SIZE_T_TO_INT(V) ( static_cast< int >( V ) )
//  ............................................................................
#define     GWRCB_LOG(WXFORMAT, ...)                                            \
{                                                                               \
    wxString tmp =                                                              \
        wxString::Format(                                                       \
            WXFORMAT                    ,                                       \
            earlgreycb::Log_spc_wxc()   ,                                       \
            __VA_ARGS__                 );                                      \
                                                                                \
    if ( earlgreycb::A_log_console )                                            \
    {                                                                           \
        earlgreycb::Log_console  ( tmp );                                       \
    }                                                                           \
    if ( earlgreycb::A_log_window )                                             \
    {                                                                           \
        earlgreycb::Log_window   ( tmp );                                       \
    }                                                                           \
}
//  ............................................................................
#define GWRCB_TCS(FORMAT, ...)      GWRCB_LOG( wxS("%s"        FORMAT), __VA_ARGS__);
#define GWRCB_TKI(FORMAT, ...)      GWRCB_LOG( wxS("%s" "tki:" FORMAT), __VA_ARGS__);
#define GWRCB_TKW(FORMAT, ...)      GWRCB_LOG( wxS("%s" "tkw:" FORMAT), __VA_ARGS__);
#define GWRCB_TKE(FORMAT, ...)      GWRCB_LOG( wxS("%s" "TKE:" FORMAT), __VA_ARGS__);
#define GWRCB_INF(FORMAT, ...)      GWRCB_LOG( wxS("%s" "inf:" FORMAT), __VA_ARGS__);
#define GWRCB_WNG(FORMAT, ...)      GWRCB_LOG( wxS("%s" "wng:" FORMAT), __VA_ARGS__);
#define GWRCB_ERR(FORMAT, ...)      GWRCB_LOG( wxS("%s" "ERR:" FORMAT), __VA_ARGS__);

}

#endif                                                                          //  __GWR_CB_LOG_HH__
