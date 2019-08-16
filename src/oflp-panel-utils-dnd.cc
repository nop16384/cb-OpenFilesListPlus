/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-common.hh"

#include    "oflp-plugin.hh"

#include    "oflp-plugin-module.hh"
#include    "oflp-plugin-mod-panels.hh"

#include    "oflp-panel.hh"
#include    "oflp-panel-utils.hh"
#include    "oflp-panel-utils-dnd.hh"
//  ................................................................................................
#define     ERG_OFLP_SANITY_CHECKS
#include    "generated/oflp-dnd--log-defines.cci"
//  ################################################################################################
//
//                          OflpPanelDataObject
//
//  ################################################################################################
OflpPanelDndDataObject::OflpPanelDndDataObject()
{
    wxDataFormat fmt;
    fmt.SetId(wxS("c::b/oflp-item-drag"));

    SetFormat(fmt);

    a_data_p1   =   (int*)          ( a_data );
    a_data_p2   =   (int*)          ( a_data + 1 * sizeof(int) );
    a_data_p3   =   (EditorBase**)  ( a_data + 2 * sizeof(int) );
    a_data_size = 0;
}
//  ================================================================================================
bool        OflpPanelDndDataObject::z_serialize     ()
{
    *a_data_p1          =   a_data_panel_visual_index;
    *a_data_p2          =   a_data_item_visual_index;
    *a_data_p3          =   a_data_editor;
    a_data_size         =   sizeof(a_data_p1) + sizeof(a_data_p2) + sizeof(a_data_p3);
    return true;
}
bool        OflpPanelDndDataObject::z_deserialize   ()
{
    a_data_panel_visual_index   =   *a_data_p1;
    a_data_item_visual_index    =   *a_data_p2;
    a_data_editor               =   *a_data_p3;
    //  ............................................................................................
    //  validity checks
    if ( a_data_panel_visual_index < 0 )
        return false;
    if ( a_data_panel_visual_index >= OflpPanelDndDataObject::s_panel_index_max )
        return false;

    if ( a_data_item_visual_index  < 0 )
        return false;
    if ( a_data_item_visual_index  >= OflpPanelDndDataObject::s_item_index_max  )
        return false;

    if ( ! a_data_editor )
        return false;

    return true;
}
//  ================================================================================================
/*
int         OflpPanelDataObject::x_data_panel_vix()
{
    if ( ! a_data_size )
        return wxNOT_FOUND;

    return a_data_panel_visual_index;
}

int         OflpPanelDataObject::x_data_item_vix()
{
    if ( ! a_data_size )
        return wxNOT_FOUND;

    return a_data_item_visual_index;
}

EditorBase* OflpPanelDataObject::x_data_editor()
{
    if ( ! a_data_size )
        return NULL;

    return a_data_editor;
}
*/
//  ================================================================================================
size_t      OflpPanelDndDataObject::GetDataSize     ()                     const
{
    //ERG_TKI("OflpPanelDataObject::GetDataSize():[%i]", oflp::Log_szt2int(a_data_size));
    return a_data_size;
}

bool        OflpPanelDndDataObject::GetDataHere     (void *buf)            const
{
    //ERG_TKI("OflpPanelDataObject::GetDataHere():this[%p], buf[%p] data sz[%lu]", this, buf, a_data_size);
    if ( ! a_data_size )
        return false;

    memcpy( buf, (void*)a_data, a_data_size );
    return true;
}

bool        OflpPanelDndDataObject::SetData         (size_t _len, const void* _buf)
{
    ERG_TKI("OflpPanelDataObject::SetData([%lu], [%p])", _len, _buf);
    //  ............................................................................................
    if ( _len > x_data_capacity() )
        return false;

    memcpy( (void*)a_data, _buf, _len );

    if ( ! z_deserialize() )
        return false;

    a_data_size =   _len;
    return true;
}

bool        OflpPanelDndDataObject::SetData         (int _panel_vix, int _item_vix, EditorBase* _editor)
{
    ERG_TKI("OflpPanelDataObject::SetData([%i], [%i], [%p])", _panel_vix, _item_vix, _editor);
    //  ............................................................................................
    //  input validity checks
    if ( ( _panel_vix < 0 ) || ( _panel_vix >= OflpPanelDndDataObject::s_panel_index_max ) )
        return false;

    if ( ( _item_vix < 0  ) || ( _item_vix >= OflpPanelDndDataObject::s_item_index_max   ) )
        return false;

    if ( ! _editor )
        return false;

    a_data_panel_visual_index   =   _panel_vix;
    a_data_item_visual_index    =   _item_vix;
    a_data_editor               =   _editor;

    return z_serialize();
}
//  ################################################################################################
//
//                          OflpPanelDropTarget
//
//  ################################################################################################
OpenFilesListPlusPanelDropTarget::        OpenFilesListPlusPanelDropTarget(wxTreeCtrl* _owner, OflpPanel* _owner_panel)
{
    a_owner_tree    =   _owner;
    a_owner_panel   =   _owner_panel;
    d_data_object   =   new OflpPanelDndDataObject();

    SetDataObject( d_data_object );
}

OpenFilesListPlusPanelDropTarget::       ~OpenFilesListPlusPanelDropTarget()
{
    //delete d_data_object;
}

bool OpenFilesListPlusPanelDropTarget::   OnDrop(wxCoord x, wxCoord y)
{
    OFLP_LOG_FUNC_ENTER("OflpPanelDropTarget::OnDrop()");
    OFLP_LOG_FUNC_EXIT();
    return true;
}

wxDragResult
OpenFilesListPlusPanelDropTarget::        OnData(wxCoord x, wxCoord y, wxDragResult defaultDragResult)
{
    wxDragResult        dres;

    EditorBase      *   editor;
    OflpPanel       *   spanel  =   NULL;
    int                 spvix   =   0;
    int                 sivix   =   -1;

    int                 cpvix   =   0;
    int                 civix   =   0;
    wxTreeItemId        ciid;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OflpPanelDropTarget::OnData()");
    //  ............................................................................................
    //  use predef wxWidgets object copy ; depending on ??? ( answer :
    //  dropSource.DoDragDrop() ) the drag result may differ from what the drop
    //  source has asked for
    //dres = wxDropTarget::OnData(x, y, defaultDragResult);                                         //  see bug#
    dres = defaultDragResult;

    ERG_TKI("OnData():suggested drag result[%s] returned[%s]"  ,
        OpenFilesListPlusPanel::stringize_drag_result(defaultDragResult).wc_str()   ,
        OpenFilesListPlusPanel::stringize_drag_result(dres).wc_str()                );

    if ( dres != defaultDragResult )
    {
        //d_data_object->ClrData();
        goto lab_return;
    }
    //  ............................................................................................
    GetData();                                                                                      // copy src wxDataObject into dst wxDataObject
    //  ............................................................................................
    editor  = d_data_object->x_data_editor();                                                       //  get editor from unserialized data
    spvix   = d_data_object->x_data_panel_vix();                                                    //  ...src panel visual index
    sivix   = d_data_object->x_data_item_vix();                                                     //  ...src item visual index
    ERG_INF("  data :spvix[%i] sivix[%i] ed[%p]", spvix, sivix, editor);
    spanel  = oflp::Modules::Instance()->panels()->x_get_from_editor(editor);                       // get src panel from editor

    #ifdef  ERG_OFLP_SANITY_CHECKS                                                                  //  _ERG_SANITY_CHECK_
    cpvix   = oflp::Modules::Instance()->panels()->x_get_vix(spanel);                               // get panel visual index
    //iid     = dpanel->item_find(ed);                                                              // _ERG_TODO_ get item visual index too

    ERG_INF("  s-chk:spvix[%i] sivix[%i] ed[%p]", spvix, sivix, editor);

    if ( spvix != cpvix )
    {
        ERG_ERR("%s", wxS("  s-chk:panel visual index mismatch") );
        dres = wxDragError;
        goto lab_return;
    }
    #endif
    //  ............................................................................................
    OpenFilesListPlus::Instance()->editor_drag_and_dropped(a_owner_panel, spanel, editor);          // _ERG_TECH_ (001)
    //  ............................................................................................
lab_return:
    OFLP_LOG_FUNC_EXIT();
    return dres;
}
//  ################################################################################################
//
//                          OflpPanel DnD
//
//  ################################################################################################
wxString
OpenFilesListPlusPanel::      stringize_drag_result(wxDragResult _dres)
{
    wxString    s = wxString::FromUTF8("invalid");

    switch ( _dres )
    {
        //  Error prevented the D&D operation from completing.
        case    wxDragError :	s = wxString::FromUTF8("wxDragError"); break;

        //  Drag target didn't accept the data.
        case    wxDragNone 	:   s = wxString::FromUTF8("wxDragNone"); break;

        //  The data was successfully copied.
        case    wxDragCopy  :   s = wxString::FromUTF8("wxDragCopy"); break;

        //  The data was successfully moved (MSW only).
        case    wxDragMove  : 	s = wxString::FromUTF8("wxDragMove"); break;

        //  Operation is a drag-link.
        case    wxDragLink  : 	s = wxString::FromUTF8("wxDragLink"); break;

        //  The operation was cancelled by user (not an error).
        case    wxDragCancel:   s = wxString::FromUTF8("wxDragCancel"); break;
    }

    return s;
}

void OpenFilesListPlusPanel:: OnDragInit  (wxTreeEvent& _e)
{
    wxDropSource                dropSource(d_tree);

    wxDragResult                dres;
    OflpPanelDndDataObject      dobj;
    wxTreeItemId                iid     =   _e.GetItem();                                           //  _GWR_REM_ always valid ( cf wxdoc )
    OflpPanelTiData         *   tid     =   NULL;
    EditorBase              *   editor  =   NULL;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OflpPanel::OnDragInit()");
    //  ............................................................................................
    ERG_INF("  object format count [%i]", oflp::Log_szt2int( dobj.GetFormatCount()) );
    //  ............................................................................................
    //  You can init a drag anywhere inside the wxTreeCtrl, even where there is
    //  nothing ! In this case, iid IsOk() ( why ??? ) , but idata is NULL
    if ( ! iid.IsOk() )                                                                             //  _GWR_UNUSEFUL_ should never happen
    {
        ERG_TKE("%s", wxS("  invalid wxTreeItemId") );
        goto lab_return;
    }

    tid = static_cast< OflpPanelTiData* >( d_tree->GetItemData(iid) );
    if ( ! tid )
    {
        ERG_TKI("%s", wxS("  NULL data (maybe dragged from empty blank space ?)") );
        goto lab_return;
    }

    editor  =   tid->x_get_editor();
    //  ............................................................................................
    //  prepare
    dobj.SetData( oflp::Modules::Instance()->panels()->x_get_vix(this) , 9999, editor );            //  drag source _ERG_TODO_ visual item index
    dropSource.SetData(dobj);
    //  ............................................................................................
    // do DnD
    dres = dropSource.DoDragDrop( wxDrag_DefaultMove );                                             //  blocking call, until dnd has finished

    if ( dres != wxDragMove )
    {
        ERG_TKE("  res[%s], expected wxDragMove", OpenFilesListPlusPanel::stringize_drag_result(dres).wc_str() );
        goto lab_return;
    }
    //  ............................................................................................
lab_return:
    OFLP_LOG_FUNC_EXIT();
    return;
}

void OpenFilesListPlusPanel:: OnDragEnd   (wxTreeEvent& _e)
{
    ERG_INF("%s", wxS("OflpPanel::OnDragEnd()") );
}
