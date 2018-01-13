#include    "oflp-common.hh"
//  ................................................................................................
#define GWR_OFLP_SANITY_CHECKS
//#define GWR_LOG(FORMAT, ...)    GWRCB_LOG(FORMAT, __VA_ARGS__);
//#define GWR_TKI(FORMAT, ...)    GWRCB_TKI(FORMAT, __VA_ARGS__);
//#define GWR_TKE(FORMAT, ...)    GWRCB_TKE(FORMAT, __VA_ARGS__);
//#define GWR_INF(FORMAT, ...)    GWRCB_INF(FORMAT, __VA_ARGS__);
//#define GWR_WNG(FORMAT, ...)    GWRCB_WNG(FORMAT, __VA_ARGS__);
#define GWR_ERR(FORMAT, ...)    GWRCB_ERR(FORMAT, __VA_ARGS__);


namespace oflp
{

//  ################################################################################################
//
//                          HSTRING
//
//  ################################################################################################
HString::tHash  HString::Hash   (wxString const & _wxstr)
{
    wxCharBuffer        ubuf    =   _wxstr.ToUTF8();                                                //  transform the wxString
    size_t              len     =   strlen(ubuf.data());                                            //  ..to an hash-able
    char        *       p       =   ubuf.data();                                                    //  ..array of bytes

    tHash               hash    =   5381;
    size_t              i       =   0;
    int                 c       =   0;
    //  ............................................................................................
    //if ( len > 16 ) len = 16;                                                                     //  dont limit hash computing to 16 bytes

    while ( i != len )
    {
        c       =   *(p++);
        hash    = ( (hash << 5) + hash) + c;                                                        // hash * 33 + c
        i++;
    }
    return hash;
}
//  ################################################################################################
//
//                          UID
//
//  ################################################################################################
void    UID::z_set  ( wxString const & _str )
{
    if ( _str.Len() != StrSize )
    {
        GWR_ERR("UID::z_set(wxString):bad string length [%lu][%s]", _str.Len(), _str.wc_str());
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
