/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin-mod-editors.hh"

#include    "oflp-plugin.hh"

#include    "oflp-plugin-mod-panels.hh"
#include    "oflp-plugin-mod-layout.hh"

#include    "oflp-panel.hh"
//  ............................................................................
#define GWR_OFLP_SANITY_CHECKS
#define GWR_LOG(FORMAT, ...)    GWRCB_LOG(FORMAT, __VA_ARGS__);
#define GWR_TKI(FORMAT, ...)    GWRCB_TKI(FORMAT, __VA_ARGS__);
#define GWR_TKE(FORMAT, ...)    GWRCB_TKE(FORMAT, __VA_ARGS__);
#define GWR_INF(FORMAT, ...)    GWRCB_INF(FORMAT, __VA_ARGS__);
#define GWR_WNG(FORMAT, ...)    GWRCB_WNG(FORMAT, __VA_ARGS__);
#define GWR_ERR(FORMAT, ...)    GWRCB_ERR(FORMAT, __VA_ARGS__);
//  ############################################################################
//
//                          EDITORS
//
//  ############################################################################
//  ============================================================================
void    OflpModEditors::    dump(
//  ============================================================================
//! \fn     dump()
//!
//! \brief  Log all infos about an EditorBase.
    EditorBase  *   _edb)
{
    wxString        shn;
    wxString        rfn;
    bool            bin;
    wxString        sbin;
    cbEditor    *   cbed    =   NULL;
    ProjectFile *   pjf     =   NULL;
    //  ........................................................................
    bin =   _edb->IsBuiltinEditor();
    sbin.assign( bin ? wxS("Y") : wxS("N") );
    //  ........................................................................
    if ( ! _edb )
    {
        shn.assign(wxS("EMPTY"));
        rfn.assign(wxS("NULL"));
        goto lab_dump;
    }
    //  ........................................................................
    shn = _edb->GetShortName();
    if ( shn.IsEmpty() )
    {
        shn.assign(wxS("EMPTY"));
        rfn.assign(wxS("???"));
        goto lab_dump;                                                          //  dont try to play with cbed if shn is empty
    }
    //  ........................................................................
    if ( !bin )
    {
        rfn.assign(wxS("???"));
        goto lab_dump;
    }
    //  ........................................................................
    cbed    =   reinterpret_cast< cbEditor* >(_edb);
    pjf     =   cbed->GetProjectFile();
    if ( ! pjf )
    {
        rfn.assign(wxS("NULL"));
    }
    else
    {
        rfn = pjf->relativeFilename;
    }

lab_dump:
    GWR_TKI("OFLP::editor_dump():[%p] bin[%s] shn[%s] rfn[%s]", _edb, sbin.wc_str(), shn.wc_str(), rfn.wc_str());
}
//  ############################################################################
bool    OflpModEditors::    add (EditorBase* _nn_edb)
{
    OflpPanel                               *   panel   =   NULL;
    OflpModLayout::FileAssignment   const   *   fa      =   NULL;
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::Editors::add()");
    //  ........................................................................
    OFLP_EMERGENCY__NULL_POINTER__FAILURE(_nn_edb);
    //  ........................................................................
    fa      = layout()->file_assignment_find(_nn_edb);
    if ( fa )
        panel   =   panels()->get_by_name( fa->pname() );
    GWR_TKI("  ...[%p][%s] -> [%p][%s]" ,
        _nn_edb , _nn_edb->GetShortName().wc_str()                  ,
        panel   , panel ? panel->title().wc_str() : wxS("Bulk")     );

    //  assignment found, at to the good panel
    if ( panel )
    {
        if ( ! panel->editor_add(_nn_edb) )
            goto lab_exit_failure;
    }
    //  assignment not found, at to bulk
    else
    {
        if ( ! panels()->p0_bulk()->editor_add(_nn_edb) )
            goto lab_exit_failure;
    }
    //  ........................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

bool    OflpModEditors::    sub (EditorBase* _nn_edb)
{
    OflpPanel       *   panel   =   NULL;
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::Editors::sub()");
    //  ........................................................................
    panel = panels()->get(_nn_edb);

    GWR_TKI("  ...[%p][%s] from panel [%p][%s]" ,
        _nn_edb , _nn_edb->GetShortName().wc_str()                  ,
        panel   , panel ? panel->title().wc_str() : wxS("NULL")     );

    if ( panel )
    {
        if ( ! panel->editor_del(_nn_edb) )
            goto lab_exit_failure;
    }
    else
    {
        goto lab_exit_failure;
    }
    //  ........................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

bool    OflpModEditors::    mov (
    OflpPanel   *   _nn_dst     ,
    OflpPanel   *   _nn_src     ,
    EditorBase  *   _nn_edb     )
{
    if ( ! _nn_src->editor_del( _nn_edb ) )
        return false;

    if ( ! _nn_dst->editor_add( _nn_edb ) )
        return false;

    layout()->file_assignment_update( _nn_edb, _nn_dst );
    return true;
}

