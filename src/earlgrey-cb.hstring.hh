#ifndef __GWR_CB_HSTRING_HH__
#define __GWR_CB_HSTRING_HH__
//  ............................................................................
#include    <stdio.h>
#include    <wx/string.h>
#include    <wx/strconv.h>
//  ............................................................................
namespace   earlgreycb
{

/// ****************************************************************************
//! \class  HString
//!
//! \brief  wxString + a hash for speeding string equality tests.
//!
//! \detail The hash function is the famous djb2 string hash function from
//!     professor Daniel J. Bernstein.
/// ****************************************************************************
class   HString
{
  public:
    typedef     unsigned long           tHash;
    typedef     std::vector< tHash >    HArray;

  public:
    static  tHash   Hash(wxString const & _wxstr)
        {
            wxCharBuffer        ubuf    =   _wxstr.ToUTF8();                    //  transform the wxString
            size_t              len     =   strlen(ubuf.data());                //  ..to an hash-able
            char        *       p       =   ubuf.data();                        //  ..array of bytes

            tHash               hash    =   5381;
            size_t              i       =   0;
            int                 c       =   0;
            //  ................................................................
            //if ( len > 16 ) len = 16;                                         //  limit hash computing to 16 bytes

            while ( i != len )
            {
                c       =   *(p++);
                hash    = ( (hash << 5) + hash) + c;                            // hash * 33 + c
                i++;
            }
            return hash;
        }
    //  ........................................................................
  private:
    wxString    a_wxstr;
    tHash       a_hash;

  private:
    void                    p0_compute_djb2()               { a_hash = Hash(get()); }

  public:
    tHash                   hash()                          const   { return a_hash;    }
    wxString    const   &   get ()                          const   { return a_wxstr;   }
    void                    set (wxString const& _wxstr)
        {
            a_wxstr =   _wxstr;
            p0_compute_djb2();
        }

  public:
    HString()                   : a_hash(0) {}
    HString(wxString& _wxstr)               {   set(_wxstr);    }
    HString(wxString  _wxstr)               {   set(_wxstr);    }
   ~HString()                               {}
};

}

#endif                                                                          //  __GWR_CB_HSTRING_HH__


