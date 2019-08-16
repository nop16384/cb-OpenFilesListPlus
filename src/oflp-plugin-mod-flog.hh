/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PLUGIN_MOD_FLOG_HH__
#define __OFLP_PLUGIN_MOD_FLOG_HH__

#include    <wx/time.h>
#include    <wx/file.h>

#include    "oflp-common.hh"

#include    "oflp-plugin-module.hh"
/// ************************************************************************************************
//! \class  OflpModFLog
//!
//! \brief  Module for logging to a file
/// ************************************************************************************************
class OflpModFLog   :    public  OflpModule
{
    friend class    OpenFilesListPlus;
    //  --------------------------------------------------------------------------------------------
  private:
    wxFile          a_file;

  public:
    void            x_log                   (wxString       const &);
    void            x_log                   (char           const *);
    void            x_log                   (std::string    const &);
    //  --------------------------------------------------------------------------------------------
  private:
    virtual     void    z_release() {}
    //  --------------------------------------------------------------------------------------------
  public:
    OflpModFLog();
    ~OflpModFLog ();
};

#endif                                                                                              //  __OFLP_PLUGIN_MOD_FLOG_HH__
