#include    <wx/datetime.h>

#include    "oflp-common.hh"
#include    "oflp-util-uid.hh"
//  ................................................................................................
#define ERG_OFLP_SANITY_CHECKS
//#define ERG_LOG(FORMAT, ...)    ERGCB_LOG(FORMAT, __VA_ARGS__);
//#define ERG_TKI(FORMAT, ...)    ERGCB_TKI(FORMAT, __VA_ARGS__);
//#define ERG_TKE(FORMAT, ...)    ERGCB_TKE(FORMAT, __VA_ARGS__);
//#define ERG_INF(FORMAT, ...)    ERGCB_INF(FORMAT, __VA_ARGS__);
//#define ERG_WNG(FORMAT, ...)    ERGCB_WNG(FORMAT, __VA_ARGS__);
#define ERG_ERR(FORMAT, ...)    ERGCB_ERR(FORMAT, __VA_ARGS__);

namespace oflp
{

void    UID::z_set  ( wxString const & _str )
{
    if ( _str.Len() != UID::StrSize )
    {
        ERG_ERR("UID::z_set(wxString):bad string length [%lu][%s]", _str.Len(), _str.wc_str());
        a_str.set( UID::Null.str() );
    }
    else
        a_str.set( _str );
}

void    UID::z_set()
{
    unsigned short  us = 0;
    wxDateTime      dt = wxDateTime::Now();
    wxString        str;

    str.Append( wxString::Format(wxS("%03i"), dt.GetHour())         );
    str.Append( wxString::Format(wxS("%03i"), dt.GetMinute())       );
    str.Append( wxString::Format(wxS("%03i"), dt.GetSecond())       );
    str.Append( wxString::Format(wxS("%03i"), dt.GetMillisecond())  );
    str.Append( wxString::Format(wxS("%03i"), ::rand() % 1000)      );

    z_set( str );
}






}
