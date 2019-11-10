/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_UTIL_HSTRING_HH__
#define __OFLP_UTIL_HSTRING_HH__
//  ................................................................................................
#include    <wx/string.h>
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
    //typedef     std::vector< tHash >    HArray;                                                   //  unused

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


}

#endif                                                                                              //  __OFLP_UTIL_HSTRING_HH__


