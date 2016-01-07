/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_COMMON_MACROS_HH__
#define __OFLP_COMMON_MACROS_HH__
//  ............................................................................
#define GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF()                                   \
    lab_exit_success:                                                           \
        OFLP_FUNC_EXIT_LOG();                                                   \
        return true;                                                            \
                                                                                \
    lab_exit_failure:                                                           \
        OFLP_FUNC_EXIT_LOG();                                                   \
        return false;

#define GWR_LABELS_EXIT_SUCCESS_FAILURE_RET()                                   \
    lab_exit_success:                                                           \
        OFLP_FUNC_EXIT_LOG();                                                   \
        return;                                                                 \
                                                                                \
    lab_exit_failure:                                                           \
        OFLP_FUNC_EXIT_LOG();                                                   \
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
//  for(...) loop on stl containers, tired of lines like
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
#define OFLP_STL_RET(V)                                                         \
        return;

#define OFLP_STL_RETV(V,RETVAL)                                                 \
        return (RETVAL);

#endif
