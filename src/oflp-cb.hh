/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

/***************************************************************************************************
 * Name:      cb-oflp-plugin
 * Purpose:   Code::Blocks plugin
 * Author:     ()
 * Created:   2015-08-23
 * Copyright:
 * License:   GPL
 **************************************************************************************************/
#ifndef __OFLP_CB_HH__
#define __OFLP_CB_HH__
//  ................................................................................................
#include    "oflp-plugin-mod-layout.hh"
//  ................................................................................................
namespace   oflp
{

/// ************************************************************************************************
//! \class  CB
//!
//! \brief  CB related stuff
//!
//! \detail
/// ************************************************************************************************
class   CB
{
  public:
    enum    _eEdbInfoType
    {
        ePjf    =   (int)0x01   ,
        ePrj    =   (int)0x02   ,
        ePjas   =   (int)0x04   ,
        ePfas   =   (int)0x08
    };
    typedef int eEdbInfoType;

    class   EdbInfos
    {
        friend class    CB;

      public:
        eEdbInfoType                a_inf;

        wxString                    a_fn;
        wxString                    a_fp;
        wxString                    a_rfp;

      private:
        ProjectFile             *   a_pjf;
        cbProject               *   a_prj;

        OflpModLayout::PjAs             *   a_pjas;
        OflpModLayout::PfAs     const   *   a_pfas;

      public:
        ProjectFile             const   *   pjf()   { return a_pjf;     }
        cbProject               const   *   prj()   { return a_prj;     }
        OflpModLayout::PjAs             *   pjas()  { return a_pjas;    }
        OflpModLayout::PfAs     const   *   pfas()  { return a_pfas;    }

        void                                clear()
                                            {
                                                a_inf   =   0;
                                                a_fn.Empty(); a_fp.Empty(); a_rfp.Empty();
                                                a_pjf   = nullptr;
                                                a_prj   = nullptr;
                                                a_pjas  = nullptr;
                                                a_pfas  = nullptr;
                                            }
        EdbInfos()  { clear();  }
        ~EdbInfos() {           }
    };

  public:
    static  bool    X_cbEditor_from_file_path   (cbEditor** _ed, wxString const & _fp);
    static  bool    X_EditorBase_get_infos      (EdbInfos* _inf, eEdbInfoType _type, EditorBase* _nn_edb);
};

}

#endif                                                                                              // __OFLP_CB_HH__
