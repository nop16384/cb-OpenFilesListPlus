/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin.hh"

#include    "oflp-plugin-mod-panels.hh"

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
//                          OflpPanelDataObject
//
//  ############################################################################
OflpPanelDataObject::OflpPanelDataObject()
{
    wxDataFormat fmt;
    fmt.SetId(wxS("c::b/oflpp-item-drag"));

    SetFormat(fmt);

    a_data_size = 0;
}
//  ============================================================================
bool        OflpPanelDataObject::p0_serialize   ()
{
    a_data[0]                           =   a_data_panel_index;
    a_data[1]                           =   a_data_item_index;
    *( p0_serialized_editor_pointer() ) =   a_data_editor;

    a_data_size = 2 * sizeof(unsigned char) + sizeof(EditorBase*);

    return true;
}
bool        OflpPanelDataObject::p0_deserialize ()
{
    a_data_panel_index  =   a_data[0];
    a_data_item_index   =   a_data[1];
    a_data_editor       =   *( p0_serialized_editor_pointer() );
    //  ........................................................................
    //  validity checks
    if ( a_data_panel_index >= OflpPanelDataObject::s_panel_index_max )
        return false;

    if ( a_data_item_index  >= OflpPanelDataObject::s_item_index_max  )
        return false;

    if ( ! a_data_editor )
        return false;

    return true;
}
//  ============================================================================
size_t      OflpPanelDataObject::GetDataSize    ()                     const
{
    GWR_INF("OflpPanelDataObject::GetDataSize():[%i]", oflp::Log_szt2int(a_data_size));
    return a_data_size;
}

bool        OflpPanelDataObject::GetDataHere    (void *buf)            const
{
    GWR_INF("%s", wxS("OflpPanelDataObject::GetDataHere():start") );
    if ( ! a_data_size )
        return false;

    memcpy( buf, (void*)a_data, a_data_size );
    return true;
}

int         OflpPanelDataObject::GetDataPanelIx ()
{
    if ( ! a_data_size )
        return wxNOT_FOUND;

    return static_cast< int >( a_data_panel_index );
}

int         OflpPanelDataObject::GetDataItemIx  ()
{
    if ( ! a_data_size )
        return wxNOT_FOUND;

    return static_cast< int >( a_data_item_index );
}

EditorBase* OflpPanelDataObject::GetDataEditor  ()
{
    if ( ! a_data_size )
        return NULL;

    return a_data_editor;
}
//  ============================================================================
bool        OflpPanelDataObject::SetData        (size_t len, const void *buf)   //  unformated input serializer
{
    GWR_TKI("%s", wxS("OflpPanelDataObject::SetData(1):start") );
    //  ........................................................................
    if ( len > GetDataCapacity() )
        return false;

    memcpy( (void*)a_data, buf, len );

    if ( ! p0_deserialize() )
        return false;

    a_data_size =   len;
    return true;
}

bool        OflpPanelDataObject::SetData        (int _panel_ix, int _item_ix, EditorBase* _editor)  //  formated input serializer
{
    GWR_TKI("%s", wxS("OflpPanelDataObject::SetData(2):start") );
    //  ........................................................................
    //  input validity checks
    if ( ( _panel_ix < 0 ) || ( _panel_ix >= OflpPanelDataObject::s_panel_index_max ) )
        return false;

    if ( ( _item_ix < 0  ) || ( _item_ix >= OflpPanelDataObject::s_item_index_max   ) )
        return false;

    if ( ! _editor )
        return false;
    //  ........................................................................
    //  import data
    a_data_panel_index  =   static_cast< unsigned char >( _panel_ix );
    a_data_item_index   =   static_cast< unsigned char >( _item_ix  );
    a_data_editor       =   _editor;
    //  ........................................................................
    //  now serialize
    return p0_serialize();
}
//  ############################################################################
//
//                          OPENFILESLISTPLUGINPANELITEMDATA
//                          OPENFILESLISTPLUGINPANELITEMINFO
//
//  ############################################################################
OpenFilesListPlusPanelItemData::OpenFilesListPlusPanelItemData(
        OpenFilesListPlusPanel      *   _panel  ,
        EditorBase                  *   _ed     )
            :   a_editor(_ed), a_panel(_panel)
{
}

OpenFilesListPlusPanelItemInfo::OpenFilesListPlusPanelItemInfo(wxTreeEvent& _e)
    :   OpenFilesListPlusPanelItemData      (NULL, NULL )   ,
        a_is_ok                             (false      )
{
    OflpPanelItemData   *   data;
    //  ........................................................................
    //D GWR_INF("%s", wxS("OflpPanelItemInfo::()"));
    //  ........................................................................
    a_iid   =   _e.GetItem();
    //D GWR_INF("  iid   [%p]", a_iid);
    if ( ! a_iid.IsOk() )   goto lab_failure;

    a_tree  = static_cast< wxTreeCtrl* >( _e.GetEventObject() );
    //D GWR_INF("  tree  [%p]", a_tree);
    if ( ! a_tree )         goto lab_failure;

    //  when DnD ended, OnTreeSelChanged is called on the source ; we land
    //  here with a valid iid but a NULL data, since the item was removed !
    data    = static_cast< OflpPanelItemData* >( a_tree->GetItemData(a_iid) );
    //D GWR_INF("  data  [%p]", data);
    if ( ! data )           goto lab_failure;

    a_panel     =   data->GetPanel();
    //D GWR_INF("  panel [%p]", a_panel);
    if ( ! a_panel )        goto lab_failure;

    a_editor    =   data->GetEditor();
    //D GWR_INF("  editor[%p]", a_editor);
    if ( ! a_editor )       goto lab_failure;

    #ifdef  GWR_OFLP_SANITY_CHECKS                                              //  _GWR_SANITY_CHECK_
    if ( ! GetPanel()->editor_has(GetEditor()) )
        goto lab_failure;

    if ( GetPanel()->tree() != a_tree )
        goto lab_failure;
    #endif

    a_is_ok     =   true;
    return;
    //  ........................................................................
lab_failure:
    //D GWR_ERR("%s", wxS("  failure"));
    OpenFilesListPlusPanelItemData:p1_raz();
    OpenFilesListPlusPanelItemInfo:p1_raz();
    return;
}
//  ############################################################################
//
//                          OPENFILESLISTPLUGINPANELDROPTARGET
//
//  ############################################################################
OpenFilesListPlusPanelDropTarget::        OpenFilesListPlusPanelDropTarget(wxTreeCtrl* _owner, OflpPanel* _owner_panel)
{
    a_owner         =   _owner;
    a_owner_panel   =   _owner_panel;
    d_data_object   =   new OflpPanelDataObject();

    SetDataObject( d_data_object );
}

OpenFilesListPlusPanelDropTarget::       ~OpenFilesListPlusPanelDropTarget()
{
    //delete d_data_object;
}

bool OpenFilesListPlusPanelDropTarget::   OnDrop(wxCoord x, wxCoord y)
{
    GWR_INF("%s", wxS("OflpPanelDropTarget::OnDrop():start") );
    return true;
}

wxDragResult
OpenFilesListPlusPanelDropTarget::        OnData(wxCoord x, wxCoord y, wxDragResult defaultDragResult)
{
    wxDragResult        dres;

    EditorBase      *   editor;
    OflpPanel       *   spanel  =   NULL;
    int                 spvix   =   0;
    int                 siix    =   0;

    OflpPanel       *   cpanel  =   NULL;
    int                 cpvix   =   0;
    int                 ciix    =   0;
    wxTreeItemId        ciid;
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OflpPanelDropTarget::OnData()");
    //  ........................................................................
    //  reset destination panel for the end of OnDragInit()
    OpenFilesListPlus::Instance()->dnd_panel_dst_set(NULL);

    //  use predef wxWidgets object copy ; depending on ??? ( answer :
    //  dropSource.DoDragDrop() ) the drag result may differ from what the drop
    //  source has asked for
    dres = wxDropTarget::OnData(x, y, defaultDragResult);

    GWR_TKI("OnData():suggested drag result[%s] returned[%s]"  ,
        OpenFilesListPlusPanel::stringize_drag_result(defaultDragResult).wc_str()   ,
        OpenFilesListPlusPanel::stringize_drag_result(dres).wc_str()                );

    if ( dres != defaultDragResult )
    {
        d_data_object->ClrData();
        goto lab_return;
    }
    //  ........................................................................
    editor  = d_data_object->GetDataEditor();                                   //  get editor from unserialized data
    spvix   = d_data_object->GetDataPanelIx();                                  //  ...src panel visual index
    siix    = d_data_object->GetDataItemIx();                                   //  ...src item visual index
    GWR_INF("  data:spvix[%i] siix[%i] ed[%p]\n", spvix, siix, editor);

    #ifdef  GWR_OFLP_SANITY_CHECKS                                              //  _GWR_SANITY_CHECK_
    cpanel  = OpenFilesListPlus::Instance()->panels()->get(editor);             //  get src panel from editor
    GWR_INF("  panel:from editor[%p]", cpanel);

    cpvix   = OpenFilesListPlus::Instance()->panels()->get_visual_index(cpanel);
    //iid     = dpanel->item_find(ed);

    GWR_TKI("  check:cpvix[%i] ciix[%i]", cpvix, ciix);

    if ( cpvix != spvix )
    {
        GWR_ERR("%s", wxS("  check:panel visual index mismatch") );
        dres = wxDragError;
        goto lab_return;
    }
    #endif                                                                      //  _GWR_TODO_ check item index too

    //  item operations are done in the cbPlugin instance ; here just
    //  memorize destination parameters
    OpenFilesListPlus::Instance()->dnd_panel_dst_set( opanel() );
    //  ........................................................................
lab_return:
    OFLP_FUNC_EXIT_LOG();
    return dres;
}
