/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-common.hh"

#include    "oflp-plugin.hh"

#include    "oflp-cb.hh"

#include    "oflp-panel.hh"
#include    "oflp-plugin-mod-panels.hh"
//  ................................................................................................
#define     ERG_OFLP_SANITY_CHECKS
#include    "generated/oflp-dnd--log-defines.cci"
//  ################################################################################################
//
//                          EDITOR EVENTS
//
//  ################################################################################################
void    OpenFilesListPlus:: editor_drag_and_dropped(
    OflpPanel   *   _panel_dst  ,
    OflpPanel   *   _panel_src  ,
    EditorBase  *   _ed         )
{
    oflp::CB::EdbInfos                  inf;
    OflpModLayout::PjAs             *   pjas        =   nullptr;
    OflpModLayout::PfAs     const   *   s_pfas      =   nullptr;
    //  ............................................................................................
    if ( ! _panel_dst )
    {
        ERG_TKE("editor_drag_and_dropped():%s", wxS("_panel_dst is NULL"));
        return;
    }
    if ( ! _panel_src )
    {
        ERG_TKE("editor_drag_and_dropped():%s", wxS("_panel_src is NULL"));
        return;
    }
    if ( ! _ed )
    {
        ERG_TKE("editor_drag_and_dropped():%s", wxS("_ed is NULL"));
        return;
    }
    //  ............................................................................................
    //  if src is not bulk, get the PfAs ( should exist )
    if ( ! _panel_src->is_bulk() )
    {
        if ( ! oflp::CB::X_EditorBase_get_infos( &inf, oflp::CB::ePfas, _ed ) )
        {
            ERG_TKE("editor_drag_and_dropped():src != bulk:%s", wxS("could not get PfAs from EditorBase"));
            return;
        }

          pjas  =   inf.pjas();
        s_pfas  =   inf.pfas();
    }
    else
    {
        if ( ! oflp::CB::X_EditorBase_get_infos( &inf, oflp::CB::ePjas, _ed ) )
        {
            ERG_TKE("editor_drag_and_dropped():src == bulk:%s", wxS("could not get PjAs from EditorBase"));
            return;
        }

          pjas  =   inf.pjas();
    }
    //  ............................................................................................
    _panel_src->editor_del( _ed );
    _panel_dst->editor_add( _ed );

    if ( ! _panel_src->is_bulk() )
        pjas->x_pfas_sub(s_pfas);

    if ( ! _panel_dst->is_bulk() )
        pjas->x_pfas_add(inf.a_rfp, _panel_dst->uid());

    oflp::modPanels()->x_resize_and_layout();
}












