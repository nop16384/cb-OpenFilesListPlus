/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-common.hh"
//  ................................................................................................
#include    "oflp-cb.hh"
//  ................................................................................................
#include    <configurationpanel.h>
//  ................................................................................................
#define ERG_OFLP_SANITY_CHECKS
#define ERG_LOG(FORMAT, ...)    ERGCB_LOG(FORMAT, __VA_ARGS__);
#define ERG_TKI(FORMAT, ...)    ERGCB_TKI(FORMAT, __VA_ARGS__);
#define ERG_TKW(FORMAT, ...)    ERGCB_TKW(FORMAT, __VA_ARGS__);
#define ERG_TKE(FORMAT, ...)    ERGCB_TKE(FORMAT, __VA_ARGS__);
#define ERG_INF(FORMAT, ...)    ERGCB_INF(FORMAT, __VA_ARGS__);
#define ERG_WNG(FORMAT, ...)    ERGCB_WNG(FORMAT, __VA_ARGS__);
#define ERG_ERR(FORMAT, ...)    ERGCB_ERR(FORMAT, __VA_ARGS__);
//  ################################################################################################
namespace   oflp
{


bool    CB:: X_cbEditor_from_file_path   (cbEditor** _ed, wxString const & _fp)
{
    int             s       =   0;
    int             i       =   0;
    EditorBase  *   edb     =   nullptr;
    cbEditor    *   ed      =   nullptr;
    //  ............................................................................................
    s = Manager::Get()->GetEditorManager()->GetEditorsCount();

    for ( i = 0 ; i != s ; i++ )
    {
        edb =   Manager::Get()->GetEditorManager()->GetEditor(i);

        if ( ! edb->IsBuiltinEditor() )
            continue;

        ed = reinterpret_cast< cbEditor* >( edb );

        if ( ed->GetFilename().Cmp(_fp) == 0 )
        {
            *(_ed) = ed;
            return true;
        }
    }

    *(_ed) = nullptr;
    return false;
}

bool    CB:: X_EditorBase_get_infos      (EdbInfos* _inf, eEdbInfoType _type, EditorBase* _nn_edb)
{
    cbEditor    *   edc =   reinterpret_cast< cbEditor* >( _nn_edb );
    wxStopWatch     stw;
    //  ............................................................................................
    OFLP_LOG_FUNC_INC();
    //  ............................................................................................
    _inf->clear();
    _inf->a_fn = _nn_edb->GetTitle();
    //  ............................................................................................
    if ( ! _nn_edb->IsBuiltinEditor() )
    {
        ERG_TKW("oflp::CB::X_EditorBase_get_infos():not a builtin editor [%p][%s]", _nn_edb, _inf->a_fn.wc_str());
        goto lab_exit_failure;
    }
    _inf->a_fp  =   edc->GetFilename();
    //  ............................................................................................
    //  logical stuff :
    //  cbProject <=> ProjectFile           , we take ePjf as reference
    //  FlAs       => PjAs & ProjectFile
    if ( _type & CB::ePrj )  _type |= ( CB::ePjf );
    if ( _type & CB::ePjas ) _type |= ( CB::ePjf );
    if ( _type & CB::ePfas ) _type |= ( CB::ePjf | CB::ePjas );
    //  ............................................................................................
    if ( ! ( _type & oflp::CB::ePjf ) )
        goto lab_exit_success;

    _inf->a_prj = Manager::Get()->GetProjectManager()->FindProjectForFile(
            _inf->a_fp      ,
        &   _inf->a_pjf     ,
        false, false        );

    if ( ( ! _inf->a_pjf ) || ( ! _inf->a_prj ) )
    {
        ERG_TKI("%s", wxS("oflp::CB::X_EditorBase_get_infos():call to FindProjectForFile() failed"));
        goto lab_exit_failure;
    }
    _inf->a_rfp =   _inf->a_pjf->relativeFilename;
    _inf->a_inf |=  ( oflp::CB::ePjf | oflp::CB::ePrj);
    //  ............................................................................................
    if ( ! ( _type & oflp::CB::ePjas ) )
        goto lab_exit_success;

    if ( ! oflp::Modules::Instance()->layout()->x_pjas_get( &(_inf->a_pjas), _inf->a_prj) )
    {
        ERG_TKI("%s", wxS("oflp::CB::X_EditorBase_get_infos():call to x_pjas_get() failed"));
        goto lab_exit_failure;
    }
    _inf->a_inf |=  oflp::CB::ePjas;
    //  ............................................................................................
    if ( ! ( _type & oflp::CB::ePfas ) )
        goto lab_exit_success;

    if ( ! _inf->a_pjas->x_pfas_get( &(_inf->a_pfas), _inf->a_fn, _inf->a_rfp) )
    {
        ERG_TKI("%s", wxS("oflp::CB::X_EditorBase_get_infos():call to x_pfas_get() failed"));
        goto lab_exit_failure;
    }
    _inf->a_inf |=  oflp::CB::ePfas;
    //  ............................................................................................
lab_exit_success:
    ERG_TKI("oflp::CB::X_EditorBase_get_infos():time [%s]us", stw.TimeInMicro().ToString().wc_str());
    OFLP_LOG_FUNC_DEC();
    return true;

lab_exit_failure:
    ERG_TKI("oflp::CB::X_EditorBase_get_infos():time [%s]us", stw.TimeInMicro().ToString().wc_str());
    OFLP_LOG_FUNC_DEC();
    return false;
}


}



