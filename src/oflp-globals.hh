/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __GWR_CB_GLOBALS_HH__
#define __GWR_CB_GLOBALS_HH__
//  ................................................................................................
#include    <stdio.h>
#include    <wx/string.h>
#include    <wx/strconv.h>
#include    <wx/datetime.h>
//  ................................................................................................
#include    <vector>
//  ................................................................................................
namespace   oflp
{

/// ************************************************************************************************
//! \class  HString
//!
//! \brief  wxString + a hash for speeding string equality tests.
//!
//! \detail The hash function is the famous djb2 string hash function from
//!     professor Daniel J. Bernstein.
/// ************************************************************************************************
class   HString
{
  public:
    typedef     unsigned long           tHash;
    typedef     std::vector< tHash >    HArray;

  public:
    static  tHash   Hash(wxString const & _wxstr);
   //  ............................................................................................
  private:
    wxString    a_wxstr;
    tHash       a_hash;

  private:
    void                    z_compute_djb2()                { a_hash = Hash(get()); }

  public:
    tHash                   hash()                          const   { return a_hash;                        }
    wxString    const   &   get ()                          const   { return a_wxstr;                       }
    void                    set (wxString const& _wxstr)            { a_wxstr =   _wxstr; z_compute_djb2(); }

  public:
    HString()                   : a_hash(0) {}
    HString(wxString& _wxstr)               {   set(_wxstr);    }
    HString(wxString  _wxstr)               {   set(_wxstr);    }
   ~HString()                               {}
};


class   UID
{
  private:
    static const size_t     FldCard =   5;
    static const size_t     FldSize =   3;
    static const size_t     StrSize =   FldCard * FldSize;

    oflp::HString           a_str;

    static       wxString   NullStr;

  public:
    static UID const        Null;


  private:
    void                    z_set( wxString const & _str );
    void                    z_set();

  public:
    wxString const &        str()   const   { return a_str.get();           }

    void                    renew()         { z_set();                      }

    bool                    is_null()       { return ( a_str.get().Cmp( UID::NullStr ) == 0 );      }

  public:
    UID(wxString & _str)
    {
        z_set( _str );
    }
    UID()
    {
        z_set();
    }
    UID(UID const & _uid)
    {
        a_str.set( _uid.str() );
    }
};


}

#endif                                                                                              //  __GWR_CB_GLOBALS_HH__


