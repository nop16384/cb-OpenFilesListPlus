/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PLUGIN_MOD_EDITORS_HH__
#define __OFLP_PLUGIN_MOD_EDITORS_HH__

#include    "oflp-common.hh"

#include    "oflp-plugin-module.hh"

/// ****************************************************************************
//! \class  OflpModEditors
//!
//! \brief  Module for all OFLP graphic stuff
/// ****************************************************************************
class OflpModEditors    :   public  OflpModule
{
    friend class    OpenFilesListPlus;
    //  ------------------------------------------------------------------------
  private:
    bool            add             (EditorBase* _nn_edb);
    bool            sub             (EditorBase* _nn_edb);
    bool            mov             (OflpPanel* _dst, OflpPanel* _src, EditorBase* _edb);

  public:
    void            dump            (EditorBase* _edb);
};

#endif
