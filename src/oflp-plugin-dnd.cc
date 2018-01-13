/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin.hh"

#include    "oflp-panel.hh"
#include    "oflp-plugin-mod-panels.hh"
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
    if ( ! _panel_dst )
    {
        GWRCB_TKE("editor_drag_and_dropped():%s", wxS("_panel_dst is NULL"));
        return;
    }
    if ( ! _panel_src )
    {
        GWRCB_TKE("editor_drag_and_dropped():%s", wxS("_panel_src is NULL"));
        return;
    }
    if ( ! _ed )
    {
        GWRCB_TKE("editor_drag_and_dropped():%s", wxS("_ed is NULL"));
        return;
    }
    //  ............................................................................................
    _panel_src->editor_del( _ed );
    _panel_dst->editor_add( _ed );
    ///layout()->file_assignment_update( _nn_edb, _nn_dst );
    panels()->resize_and_layout();
}


