/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-common.hh"
#include    "oflp-panel-utils.hh"

#include    "oflp-panel.hh"
//  ................................................................................................
#define ERG_OFLP_SANITY_CHECKS
#define ERG_LOG(FORMAT, ...)    ERGCB_LOG(FORMAT, __VA_ARGS__);
#define ERG_TKI(FORMAT, ...)    ERGCB_TKI(FORMAT, __VA_ARGS__);
#define ERG_TKE(FORMAT, ...)    ERGCB_TKE(FORMAT, __VA_ARGS__);
#define ERG_INF(FORMAT, ...)    ERGCB_INF(FORMAT, __VA_ARGS__);
#define ERG_WNG(FORMAT, ...)    ERGCB_WNG(FORMAT, __VA_ARGS__);
#define ERG_ERR(FORMAT, ...)    ERGCB_ERR(FORMAT, __VA_ARGS__);
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
    //D ERG_INF("%s", wxS("OflpPanelItemInfo::()"));
    //  ............................................................................................
    a_iid   =   _e.GetItem();
    //D ERG_INF("  iid   [%p]", a_iid);
    if ( ! a_iid.IsOk() )   goto lab_failure;

    a_tree  = static_cast< wxTreeCtrl* >( _e.GetEventObject() );
    //D ERG_INF("  tree  [%p]", a_tree);
    if ( ! a_tree )         goto lab_failure;

    //  when DnD ended, OnTreeSelChanged is called on the source ; we land                          // _ERG_TECH_ (001)
    //  here with a valid iid but a NULL data, since the item was removed !
    tid = static_cast< OflpPanelTiData* >( a_tree->GetItemData(a_iid) );
    if ( ! tid )
        goto lab_failure;
    //D ERG_INF("  data  [%p]", data);

    a_panel     =   tid->x_get_panel();
    //D ERG_INF("  panel [%p]", a_panel);
    if ( ! a_panel )        goto lab_failure;

    a_editor    =   tid->x_get_editor();
    //D ERG_INF("  editor[%p]", a_editor);
    if ( ! a_editor )       goto lab_failure;

    #ifdef  ERG_OFLP_SANITY_CHECKS                                                                  //  _ERG_SANITY_CHECK_
    if ( ! a_panel->editor_has(a_editor) )
        goto lab_failure;

    if ( a_panel->tree() != a_tree )
        goto lab_failure;
    #endif

    return;
    //  ............................................................................................
lab_failure:
    //D ERG_ERR("%s", wxS("  failure"));
    return;
}







