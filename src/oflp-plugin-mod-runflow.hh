/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PLUGIN_MOD_RUNFLOW_HH__
#define __OFLP_PLUGIN_MOD_RUNFLOW_HH__

#include    "oflp-common.hh"

#include    "oflp-plugin-module.hh"
/// ****************************************************************************
//! \class  OflpModRunflow
//!
//! \brief  Module for
//!         - filtering spurious cb events
/// ****************************************************************************
class OflpModRunflow        :   public  OflpModule
{
    friend class    OpenFilesListPlus;
    //  ------------------------------------------------------------------------
  private:
    bool            a_plugin_attaching;

    bool            a_wsp_loading_complete;
    bool            a_wsp_closing_begin;
    bool            a_wsp_closing_complete;

    bool            a_pro_open;
    bool            a_pro_close;

  private:
    void            attaching   (bool _b)   { a_plugin_attaching        = _b;   }

    void            wlc         (bool _b)   { a_wsp_loading_complete    = _b;   }
    void            wcb         (bool _b)   { a_wsp_closing_begin       = _b;   }
    void            wcc         (bool _b)   { a_wsp_closing_complete    = _b;   }

    void            pop         (bool _b)   { a_pro_open                = _b;   }
    void            pcl         (bool _b)   { a_pro_close               = _b;   }

  public:
    bool            attaching() { return a_plugin_attaching;        }

    bool            wlc()       { return a_wsp_loading_complete;    }
    bool            wcb()       { return a_wsp_closing_begin;       }
    bool            wcc()       { return a_wsp_closing_complete;    }

    bool            pop()       { return a_pro_open;                }
    bool            pcl()       { return a_pro_close;               }
    //  --------------------------------------------------------------------------------------------
    //  module
  private:
    virtual     void    modinit()       {};
    virtual     void    modrelease()    {};
    //  --------------------------------------------------------------------------------------------
  public:
    OflpModRunflow()
    {
        a_plugin_attaching      =   false;

        a_wsp_loading_complete  =   false;
        a_wsp_closing_begin     =   false;
        a_wsp_closing_complete  =   false;

        a_pro_open              =   false;
        a_pro_close             =   false;
    }
    ~OflpModRunflow()
    {
    }
};

#endif                                                                          //  __OFLP_PLUGIN_MOD_RUNFLOW_HH__
