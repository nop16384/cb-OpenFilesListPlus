/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin.hh"

#include    "oflp-plugin-mod-panels.hh"

#include    "oflp-panel.hh"
//  ................................................................................................
#define GWR_OFLP_SANITY_CHECKS
#define GWR_LOG(FORMAT, ...)    GWRCB_LOG(FORMAT, __VA_ARGS__);
#define GWR_TKI(FORMAT, ...)    GWRCB_TKI(FORMAT, __VA_ARGS__);
#define GWR_TKE(FORMAT, ...)    GWRCB_TKE(FORMAT, __VA_ARGS__);
#define GWR_INF(FORMAT, ...)    GWRCB_INF(FORMAT, __VA_ARGS__);
#define GWR_WNG(FORMAT, ...)    GWRCB_WNG(FORMAT, __VA_ARGS__);
#define GWR_ERR(FORMAT, ...)    GWRCB_ERR(FORMAT, __VA_ARGS__);
//  ################################################################################################
//
//                          OflpPanelItemData
//                          OflpPanelItemInfo
//
//  ################################################################################################
OpenFilesListPlusPanelTreeItemData::OpenFilesListPlusPanelTreeItemData(
        OpenFilesListPlusPanel      *   _panel  ,
        EditorBase                  *   _ed     )
            :   a_editor(_ed), a_panel(_panel)
{
}

OpenFilesListPlusPanelTreeItemEventInfo::OpenFilesListPlusPanelTreeItemEventInfo(wxTreeEvent& _e)
{
    OflpPanelTiData *   tid = nullptr;
    //  ............................................................................................
    //D GWR_INF("%s", wxS("OflpPanelItemInfo::()"));
    //  ............................................................................................
    a_iid   =   _e.GetItem();
    //D GWR_INF("  iid   [%p]", a_iid);
    if ( ! a_iid.IsOk() )   goto lab_failure;

    a_tree  = static_cast< wxTreeCtrl* >( _e.GetEventObject() );
    //D GWR_INF("  tree  [%p]", a_tree);
    if ( ! a_tree )         goto lab_failure;

    //  when DnD ended, OnTreeSelChanged is called on the source ; we land                          // _ERG_TECH_ (001)
    //  here with a valid iid but a NULL data, since the item was removed !
    tid = static_cast< OflpPanelTiData* >( a_tree->GetItemData(a_iid) );
    if ( ! tid )
        goto lab_failure;
    //D GWR_INF("  data  [%p]", data);

    a_panel     =   tid->x_get_panel();
    //D GWR_INF("  panel [%p]", a_panel);
    if ( ! a_panel )        goto lab_failure;

    a_editor    =   tid->x_get_editor();
    //D GWR_INF("  editor[%p]", a_editor);
    if ( ! a_editor )       goto lab_failure;

    #ifdef  GWR_OFLP_SANITY_CHECKS                                                                  //  _GWR_SANITY_CHECK_
    if ( ! a_panel->editor_has(a_editor) )
        goto lab_failure;

    if ( a_panel->tree() != a_tree )
        goto lab_failure;
    #endif

    return;
    //  ............................................................................................
lab_failure:
    //D GWR_ERR("%s", wxS("  failure"));
    return;
}







