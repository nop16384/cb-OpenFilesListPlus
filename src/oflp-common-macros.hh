/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_COMMON_MACROS_HH__
#define __OFLP_COMMON_MACROS_HH__
//  ............................................................................
#define OFLP_EDB_LOG(edb)       edb, edb->GetTitle().wc_str(), edb->GetFilename().wc_str()
#define OFLP_PANEL_LOG(p)       p, p->uid().str().wc_str(), p->title().wc_str()
//  ............................................................................
#define OFLP01(SIZE_T_VARIABLE) ( static_cast< int >( SIZE_T_VARIABLE ) )
//  ............................................................................
#define ERG_LABELS_EXIT_SUCCESS_FAILURE_RTF()                                   \
    lab_exit_success:                                                           \
        OFLP_LOG_FUNC_EXIT();                                                   \
        return true;                                                            \
                                                                                \
    lab_exit_failure:                                                           \
        OFLP_LOG_FUNC_EXIT();                                                   \
        return false;

#define ERG_LABELS_EXIT_SUCCESS_FAILURE_RET()                                   \
    lab_exit_success:                                                           \
        OFLP_LOG_FUNC_EXIT();                                                   \
        return;                                                                 \
                                                                                \
    lab_exit_failure:                                                           \
        OFLP_LOG_FUNC_EXIT();                                                   \
        return;
//  ............................................................................
//! Return immediately on degraded mode
#define OFLP_ON_DEGRADED__RET()                                                 \
    if ( degraded() )                                                           \
        return;
//! Set emergency mode and return on null pointer
#define OFLP_EMERGENCY__NULL_POINTER__RET(P)                                    \
    if ( ! P )                                                                  \
    {                                                                           \
        emergency();                                                            \
        return;                                                                 \
    }
//! Set emergency mode and goto lab_failure on null pointer
#define OFLP_EMERGENCY__NULL_POINTER__FAILURE(P)                                \
    if ( ! P )                                                                  \
    {                                                                           \
        oflp()->emergency();                                                    \
        goto lab_exit_failure;                                                  \
    }
//! Set emergency mode and return if ( CALL() != 0 )
#define OFLP_EMERGENCY__CALL__RET(C)                                            \
    if ( ! (C) )                                                                \
    {                                                                           \
        emergency();                                                            \
        return;                                                                 \
    }
//  ............................................................................
//  for(...) loop on stl containers, Im tired of lines like :
//      for ( EditorArray::const_iterator it = array->begin() ; it != array->end() ; it++)
//  or
//      EditorArray::const_iterator         it;
//  or
//      for ( OflpPanelArray::const_iterator
//          it  =   panels()->array().begin()   ;
//          it  !=  panels()->array().end()     ;
//          it++                                )

//  standard loops
#define OFLP_STL_FOR(C, V, IT)                                                  \
    for ( C::iterator IT = (V).begin() ; IT != (V).end() ; IT++ )

//  const standard loops
#define OFLP_STL_CFOR(C, V, IT)                                                 \
    for ( C::const_iterator IT = (V).begin() ; IT != (V).end() ; IT++ )

//  break stl loop
#define OFLP_STL_RET()                                                          \
        return;

#define OFLP_STL_RETV(RETVAL)                                                  \
        return (RETVAL);

#endif
