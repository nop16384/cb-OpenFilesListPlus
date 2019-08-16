/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    <wx/tooltip.h>

#include    "oflp-plugin-mod-flog.hh"

/*
#define     OFLP_DEBUG_TOOLTIPS
//  ............................................................................
#define ERG_OFLP_SANITY_CHECKS
#ifdef  OFLP_DEBUG_TOOLTIPS
    #define ERG_LOG(FORMAT, ...)    ERGCB_LOG(FORMAT, __VA_ARGS__);
    #define ERG_TKI(FORMAT, ...)    ERGCB_TKI(FORMAT, __VA_ARGS__);
    #define ERG_TKE(FORMAT, ...)    ERGCB_TKE(FORMAT, __VA_ARGS__);
    #define ERG_INF(FORMAT, ...)    ERGCB_INF(FORMAT, __VA_ARGS__);
    #define ERG_WNG(FORMAT, ...)    ERGCB_WNG(FORMAT, __VA_ARGS__);
    #define ERG_ERR(FORMAT, ...)    ERGCB_ERR(FORMAT, __VA_ARGS__);
#else
    #define ERG_LOG(FORMAT, ...)
    #define ERG_TKI(FORMAT, ...)
    #define ERG_TKE(FORMAT, ...)
    #define ERG_INF(FORMAT, ...)
    #define ERG_WNG(FORMAT, ...)
    #define ERG_ERR(FORMAT, ...)
#endif
*/
//  ################################################################################################
//
//                          SETTINGS
//
//  ################################################################################################
OflpModFLog::OflpModFLog()
{
    a_file.Open(
        wxString("/home/gwr/Src/C-C++/oflp.txt"),
		wxFile::write       ,
		wxS_DEFAULT         );
}
OflpModFLog::~OflpModFLog()
{
    a_file.Close();
}

void    OflpModFLog::   x_log(wxString const & _i_str)
{
    a_file.Write(_i_str, wxConvUTF8);
}

void    OflpModFLog::   x_log(char const* _i_str)
{
    x_log( wxString(_i_str) );
}

void    OflpModFLog::   x_log(std::string const& _i_str)
{
    x_log( wxString(_i_str) );
}


