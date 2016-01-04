/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin.hh"

#include    "oflp-plugin-mod-panels.hh"
#include    "oflp-plugin-mod-editors.hh"
//  ############################################################################
//
//                          EDITOR EVENTS
//
//  ############################################################################
void    OpenFilesListPlus:: editor_drag_and_dropped()
{
    if ( ! a_dnd_panel_dst )
    {
        GWRCB_TKE("editor_drag_and_dropped():%s", wxS("a_dnd_panel_dst is NULL"));
        return;
    }
    if ( ! a_dnd_panel_src )
    {
        GWRCB_TKE("editor_drag_and_dropped():%s", wxS("a_dnd_panel_src is NULL"));
        return;
    }
    if ( ! a_dnd_editor_base )
    {
        GWRCB_TKE("editor_drag_and_dropped():%s", wxS("a_dnd_editor_base is NULL"));
        return;
    }
    //  ................................................................
    editors()->mov( a_dnd_panel_dst, a_dnd_panel_src, a_dnd_editor_base );

    panels()->resize_and_layout();
}


