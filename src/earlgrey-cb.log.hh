#ifndef __GWR_CB_LOG_HH__
#define __GWR_CB_LOG_HH__
//  ............................................................................
#include    <stdio.h>
#include    <wx/log.h>
//  ............................................................................
namespace   earlgreycb
{

extern  bool    A_log_console;
extern  bool    A_log_window;
//  ............................................................................
extern  void    Log_console     (wxString&);
extern  void    Log_window      (wxString&);
extern  void    Log_window_show (wxWindow*);
extern  void    Log_window_hide ();
//  ............................................................................
#define GWR_SIZE_T_TO_INT(V) ( static_cast< int >( V ) )
//  ............................................................................
#define     GWRCB_LOG(FORMAT, ...)                                              \
{                                                                               \
    wxString tmp = wxString::Format( wxString::FromUTF8(FORMAT), __VA_ARGS__ ); \
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
#define GWRCB_TKI(FORMAT, ...)    GWRCB_LOG("tki:" FORMAT, __VA_ARGS__);
#define GWRCB_TKW(FORMAT, ...)    GWRCB_LOG("tkw:" FORMAT, __VA_ARGS__);
#define GWRCB_TKE(FORMAT, ...)    GWRCB_LOG("TKE:" FORMAT, __VA_ARGS__);
#define GWRCB_INF(FORMAT, ...)    GWRCB_LOG("inf:" FORMAT, __VA_ARGS__);
#define GWRCB_WNG(FORMAT, ...)    GWRCB_LOG("wng:" FORMAT, __VA_ARGS__);
#define GWRCB_ERR(FORMAT, ...)    GWRCB_LOG("ERR:" FORMAT, __VA_ARGS__);

}

#endif                                                                          //  __GWR_CB_LOG_HH__
