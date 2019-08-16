#include    "oflp-common.hh"
#include    "oflp-util-hstring.hh"
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

}
