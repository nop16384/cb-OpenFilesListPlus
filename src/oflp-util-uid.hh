/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_UTIL_UID_HH__
#define __OFLP_UTIL_UID_HH__
//  ................................................................................................
#include    <wx/string.h>
//  ................................................................................................
#include    "oflp-util-hstring.hh"
//  ................................................................................................
namespace   oflp
{

/// ************************************************************************************************
//! \class  UID
//!
//! \brief
//!
//! \detail
/// ************************************************************************************************
class   UID
{
  private:
    static const size_t     FldCard =   5;                                                          //! # of fields
    static const size_t     FldSize =   3;                                                          //! # of chars per filed
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

#endif                                                                                              //  __OFLP_UTIL_UID_HH__


