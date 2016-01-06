/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin.hh"
#include    "oflp-plugin-mod-panels.hh"
#include    "oflp-plugin-mod-gfx.hh"

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
//                          OPENFILESLISTPLUGINPANEL
//
//  ############################################################################
void OpenFilesListPlusPanel::dump()
{
    OFLP_FUNC_ENTER_MARK("OflpPanel::dump()");

    EditorBase              *   edb     =   NULL;
    oflp::HString::tHash        hash    =   0;
    oflp::HString::tHash        hash2   =   0;

    GWR_TKI("panel[%p][%s] editors[%i] hashs[%u]", this, title().wc_str(), a_editors_array.size(), a_harray.size());

    for ( size_t
        i  =   0                        ;
        i  !=  a_editors_array.size()   ;
        i++                             )
    {
        edb     =   a_editors_array.Item(i);
        hash    =   a_harray.at(i);

        hash2   =   oflp::HString::Hash( edb->GetFilename() );

        GWR_TKI("Editor[%u]:[%p][%s]hash[%lu][%lu]" ,
            i                           ,
            edb                         ,
            edb->GetTitle().wc_str()    ,
            hash                        ,
            hash2                       )
    }
}

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
    wxDropSource            dropSource( d_tree);

    wxDragResult            dres;
    OflpPanelDataObject     dobj;
    wxTreeItemId            iid     =   _e.GetItem();                           //  _GWR_REM_ always valid ( cf wxdoc )
    wxTreeItemData      *   idata   =   d_tree->GetItemData(iid);
    OflpPanelItemData   *   pidata  =   NULL;
    EditorBase          *   editor  =   NULL;

    OflpPanel           *   dpanel  =   NULL;
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OflpPanel::OnDragInit()");
    //  ........................................................................
    GWR_INF("OnDragInit():object format count [%i]", oflp::Log_szt2int( dobj.GetFormatCount()) );
    //  ........................................................................
    //  You can init a drag anywhere inside the wxTreeCtrl, even where there is
    //  nothing ! In this case, iid IsOk() ( why ??? ) , but idata is NULL
    if ( ! iid.IsOk() )                                                         //  _GWR_UNUSEFUL_ should never happen
        goto lab_return;

    if ( ! idata )
        goto lab_return;

    pidata  =   static_cast< OflpPanelItemData* >( idata );
    editor  =   pidata->GetEditor();
    //  ........................................................................
    //  prepare drag source
    dobj.SetData( OpenFilesListPlus::Instance()->panels()->get_visual_index(this) , 0xab, editor );      //  _GWR_TODO_ iid
    //wxDropSource        dropSource(dobj, d_tree);
    dropSource.SetData(dobj);
    //  ........................................................................
    // do DnD
    dres = dropSource.DoDragDrop( wxDrag_DefaultMove );
    if ( dres != wxDragMove )
    {
        GWR_ERR("           OnDragInit():res[%s], expected wxDragMove", OpenFilesListPlusPanel::stringize_drag_result(dres).wc_str() );
        goto lab_return;
    }
    //  ........................................................................
    //  delete our item ; this will cause a SELECTION_CHANGE event with a       //  _GWR_TECH_
    //  NULL data
    OpenFilesListPlus::Instance()->dnd_panel_src_set(this);
    OpenFilesListPlus::Instance()->dnd_editor_set   (static_cast< OflpPanelItemData* >( idata )->GetEditor());

    OpenFilesListPlus::Instance()->editor_drag_and_dropped();
    //  ........................................................................
lab_return:
    OFLP_FUNC_EXIT_LOG();
    return;
}

void OpenFilesListPlusPanel:: OnDragEnd   (wxTreeEvent& _e)
{
    GWR_INF("%s", wxS("OflpPanel::OnDragEnd()") );
}
//  ============================================================================
void OpenFilesListPlusPanel:: p0_allow_kill_focus_event           (bool _b)
{
    //  When user enters a new panel name, TEXT_ENTER _AND_ KILL_FOCUS are
    //  called ( in this order, but whatever, the two are called ).
    //  Thats why we need some kind of flag, avoiding to delete the same object
    //  a second time when evh_title_dynamic_KILL_FOCUS handler
    //  calls p0_title_ctrl_replace()
    a_allow_kill_focus_event = _b;
}

void OpenFilesListPlusPanel:: evh_title_static_LEFT_DOWN          (wxMouseEvent& _e)
{
    //  let OflpPanelHeader do his job
    dw_header->title_switch_to_dynamic();

    //  the header's events is connected to this ( OpenFilesListPlusPanel )
    //  only for updating this's layout, because wxStaticText and WxwxTextCtrl
    //  dont have same vertical size
    dw_sizer->Layout();
}

void OpenFilesListPlusPanel:: evh_title_dynamic_TEXT_ENTER        (wxCommandEvent& _e)
{
    GWR_INF("%s", wxS("OflpPanel:: evh_title_dynamic_TEXT_ENTER()"));

    p0_allow_kill_focus_event(false);

    dw_header   ->title_switch_to_static();
    dw_sizer    ->Layout();

    p0_allow_kill_focus_event(true);

    _e.Skip();
}

void OpenFilesListPlusPanel:: evh_title_dynamic_KILL_FOCUS        (wxFocusEvent   & _e)
{
    GWR_INF("%s", wxS("OflpPanel:: evh_title_dynamic_KILL_FOCUS()"));

    if ( ! a_allow_kill_focus_event )
    {
        GWR_INF("%s", wxS("  (aborted)"));
        return;
    }

    dw_header   ->title_switch_to_static();
    dw_sizer    ->Layout();

    _e.Skip();
}
//  ============================================================================
void OpenFilesListPlusPanel:: p0_create_tree()
{
    d_tree          =   new wxTreeCtrl( this, wxNewId(), wxDefaultPosition, wxSize(150, 100),
        wxTR_HAS_BUTTONS | wxNO_BORDER | wxTR_HIDE_ROOT );
    d_tree->AddRoot( wxString::FromUTF8("ROOT") );

    d_tree->SetImageList(const_cast< wxImageList* >( OpenFilesListPlus::Instance()->gfx()->image_list() ));

    d_tree->Connect(wxEVT_COMMAND_TREE_BEGIN_DRAG,
        wxTreeEventHandler(OpenFilesListPlusPanel::OnDragInit), NULL, this);

    d_tree->Connect(wxEVT_COMMAND_TREE_END_DRAG,
        wxTreeEventHandler(OpenFilesListPlusPanel::OnDragEnd), NULL, this);

    d_tree->Connect(wxEVT_COMMAND_TREE_ITEM_ACTIVATED,
        wxTreeEventHandler(OpenFilesListPlus::evh_tree_item_activated)      , NULL, OpenFilesListPlus::Instance());
    d_tree->Connect(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK,
        wxTreeEventHandler(OpenFilesListPlus::evh_tree_item_right_click)    , NULL, OpenFilesListPlus::Instance());
    d_tree->Connect(wxEVT_COMMAND_TREE_SEL_CHANGED,
        wxTreeEventHandler(OpenFilesListPlus::evh_tree_sel_changed)         , NULL, OpenFilesListPlus::Instance());

    d_drop_target   =   new OpenFilesListPlusPanelDropTarget(d_tree, this);
    d_tree->SetDropTarget(d_drop_target);
}
//  ============================================================================
OpenFilesListPlusPanel::OpenFilesListPlusPanel(
        OpenFilesListPlus   *   _ofl_plugin ,
        wxWindow            *   _parent     ,
        wxString                _title      ,
        bool                    _bulk       )
        :   wxPanel                     ( _parent, wxNewId())   ,
            a_bulk                      ( _bulk             )   ,
            a_allow_kill_focus_event    ( true              )
{
    GWR_INF("OpenFilesListPlusPanel::OpenFilesListPlusPanel():[%p] plugin[%p]", this, _ofl_plugin);

    if ( is_bulk() )
        return;

    SetFont( OpenFilesListPlus::Instance()->gfx()->fnt8() );

    dw_header   =   new OpenFilesListPlusPanelHeader( this, _title );

    dw_header->button_prepend   ( OflpModGfx::eBmpBarUp      );          //  2
    dw_header->button_prepend   ( OflpModGfx::eBmpBarDown    );          //  1
    dw_header->button_prepend   ( OflpModGfx::eBmpBarOrange  );          //  0
    dw_header->button_append    ( OflpModGfx::eBmpBarRed     );          //  3

    //  set disabled bitmap to padding will pseudo-hide the button
    bitmap_button(1)->SetBitmapDisabled( OflpModGfx::Bitmap(OflpModGfx::eBmpBarPadding) );
    bitmap_button(2)->SetBitmapDisabled( OflpModGfx::Bitmap(OflpModGfx::eBmpBarPadding) );

    button(0)->Connect(                                                         //  (mini | maxi) mize
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_mm)  ,
        NULL, _ofl_plugin                                                               );

    button(2)->Connect(                                                         //  up
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_up)  ,
        NULL, _ofl_plugin                                                               );

    button(1)->Connect(                                                         //  down
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_down)  ,
        NULL, _ofl_plugin                                                               );

    button(3)->Connect(                                                         //  del
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_del)   ,
        NULL, _ofl_plugin                                                               );

    p0_create_tree();

    dw_sizer    =   new wxBoxSizer(wxVERTICAL);

    dw_sizer->Add( dw_header , 0, wxEXPAND, 0);
    dw_sizer->Add( d_tree    , 1, wxEXPAND, 0);

    this->SetSizer(dw_sizer);
}

OpenFilesListPlusPanel::~OpenFilesListPlusPanel()
{
    //delete  dw_header;

    //delete  d_tree;

    //delete  d_drop_target;

    //delete  dw_sizer;
}
//  ============================================================================
void            OpenFilesListPlusPanel::    reset()
{
}
//  ============================================================================
wxTreeItemId    OpenFilesListPlusPanel::    item_find           (EditorBase* _editor)
{
    wxTreeItemIdValue   cookie  =   0;
    wxTreeItemId        iid     = d_tree->GetFirstChild( d_tree->GetRootItem(), cookie );
    //  ........................................................................
    //D GWR_INF("%s", wxS("OpenFilesListPlusPanel::item_find()"));
    //  ........................................................................
    // loop all tree items
    while ( iid )
    {
        OflpPanelItemData       *   data    = static_cast< OflpPanelItemData* >(d_tree->GetItemData(iid));
        EditorBase              *   e       = data->GetEditor();
        if ( _editor == e )
        {
            //D GWR_INF("%s", wxS("OpenFilesListPlusPanel::item_find():found"));
            return iid;
        }
        iid = d_tree->GetNextChild(d_tree->GetRootItem(), cookie);
    }
    //D GWR_INF("%s", wxS("OpenFilesListPlusPanel::item_find():not found"));
    iid.Unset();
    return iid;
}

wxTreeItemId    OpenFilesListPlusPanel::    item_append         (EditorBase* _editor)
{
    wxTreeItemId        iid;
    int                 mod = OflpModGfx::Icon(_editor);
    //  ........................................................................
    //  ........................................................................
    iid = d_tree->AppendItem(
        d_tree->GetRootItem()   ,
        _editor->GetShortName() ,
        mod, mod                ,
        new OflpPanelItemData(this, _editor)   );

    if ( ! iid.IsOk() )
    {
        GWR_TKE("%s", wxS("OpenFilesListPlusPanel::item_append():failed"));
        iid.Unset();
        return iid;
    }

    //D GWR_INF("OflpPanel::item_append():success img[%i]", mod);

    d_tree->SortChildren(d_tree->GetRootItem());

    return iid;
}

bool            OpenFilesListPlusPanel::    item_del            (EditorBase* _editor)
{
    wxTreeItemId    iid = item_find(_editor);
    //  ........................................................................
    if ( ! iid.IsOk() )
    {
        return false;
    }

    //  delete d_tree->GetItemData( iid );                                      //  wxTreeCtrl does it by itself

    d_tree->Delete(iid);
    return true;
}

bool            OpenFilesListPlusPanel::    item_select         (EditorBase* _editor)
{
    wxTreeItemId    iid = item_find(_editor);
    //  ........................................................................
    if ( ! iid.IsOk() )
    {
        return false;
    }

    //if ( d_tree->GetSelection() == iid )                                      //  buggy
        //return true;

    d_tree->SelectItem(iid);
    return true;
}

bool            OpenFilesListPlusPanel::    item_selected       (EditorBase* _editor)
{
    wxTreeItemId    iid = item_find(_editor);
    //  ........................................................................
    if ( ! iid.IsOk() )
    {
        return false;                                                           //  _GWR_TODO_ no error propagation from here !
    }

    return ( d_tree->IsSelected(iid) );
    /*
    wxTreeItemId    iid = d_tree->GetSelection();

    if ( ! iid.IsOk() )
    {
        return false;
    }

    OflpPanelItemData   *   idata = static_cast< OflpPanelItemData* >( d_tree->GetItemData(iid) );

    if ( ! idata )
    {
        return false;
    }

    if ( idata->GetEditor() == _editor )
        return true;

    return false;
    */
}

void            OpenFilesListPlusPanel::    items_deselect      ()
{
    d_tree->UnselectAll();
}

void            OpenFilesListPlusPanel::    items_del           ()
{
    d_tree->DeleteChildren(d_tree->GetRootItem());
}
//  ============================================================================
EditorBase*     OpenFilesListPlusPanel::    editor_from_absolute_filepath (wxString const & _absolute_filepath)
{
    OFLP_FUNC_ENTER_MARK("editor_from_absolute_filepath");
    oflp::HString::tHash            hash    =   oflp::HString::Hash(_absolute_filepath);
    EditorBase                  *   ed      =   NULL;
    //  ........................................................................
    //D dump();

    for ( size_t
        i   =   0               ;
        i   !=  a_harray.size() ;
        i++                     )
    {
        //D GWR_TKI("[%lu] ? [%lu][%s]", hash, a_harray.at(i), a_editors_array.Item(i)->GetFilename().wc_str());
        if ( a_harray.at(i) == hash )
        {
            if ( a_editors_array.Item(i)->GetFilename().Cmp(_absolute_filepath) == 0 )
            {
                ed      =   a_editors_array.Item(i);
                break;
            }
        }
    }

    return ed;
}

bool            OpenFilesListPlusPanel::    editor_has          (EditorBase* _editor)
{
    return ( a_editors_array.Index(_editor) != wxNOT_FOUND );
}

bool            OpenFilesListPlusPanel::    editor_add          (EditorBase* _editor)
{
    wxTreeItemId    iid;
    int             mod =   OflpModGfx::Icon(_editor);
    //  ........................................................................
    GWR_TKI("OflpPanel::editor_add():[%p][%s][%p]", this, title().wc_str(), _editor);
    //  ........................................................................
    iid = item_append(_editor);

    if ( ! iid.IsOk() )
    {
        return false;
    }

    a_editors_array.Add(_editor);
    a_harray.push_back( oflp::HString::Hash(_editor->GetFilename()) );
    return true;
}

bool            OpenFilesListPlusPanel::    editor_del          (EditorBase* _editor)
{
    //  ........................................................................
    GWR_TKI("OflpPanel::editor_del():[%p][%s][%p]", this, title().wc_str(), _editor);
    //  ........................................................................
    if ( ! item_del(_editor) )
        return false;

    a_harray.erase( a_harray.begin() + a_editors_array.Index(_editor) );
    a_editors_array.Remove(_editor);

    return true;
}

void            OpenFilesListPlusPanel::    editor_select       (EditorBase* _editor)
{
    //  ........................................................................
    GWR_TKI("OflpPanel::editor_select():[%p][%p]", this, _editor);
    //  ........................................................................
    item_select(_editor);
}

bool            OpenFilesListPlusPanel::    editor_selected     (EditorBase* _editor)
{
    //  ........................................................................
    GWR_INF("%s", wxS("OflpPanel::editor_selected()"));
    //  ........................................................................
    bool b = item_selected(_editor);

    GWR_INF("OflpPanel::editor_selected():[%s][%i]",
            _editor->GetShortName().wc_str()    ,
            b                                   );

    return b;
}

void            OpenFilesListPlusPanel::    editor_sync         (EditorBase* _editor)
{
    wxTreeItemId    iid;
    int             mod =   OflpModGfx::Icon(_editor);
    //  ........................................................................
    GWR_INF("%s", wxS("OflpPanel::editor_sync()") );
    //  ........................................................................
    iid = item_find(_editor);

    if ( ! iid.IsOk() )
    {
        return;
    }

    if ( d_tree->GetItemImage(iid, wxTreeItemIcon_Normal)  != mod )
    {
        GWR_INF("%s", wxS("editor_sync():editor icon changed"));

        d_tree->SetItemImage( iid, mod , wxTreeItemIcon_Normal );
        d_tree->SetItemImage( iid, mod , wxTreeItemIcon_Selected );
    }

    if ( d_tree->GetItemText(iid).Cmp( _editor->GetShortName() ) )
    {
        GWR_INF("editor_sync():editor name changed [%s] -> [%s]",
            d_tree->GetItemText(iid).wc_str()   ,
             _editor->GetShortName().wc_str()   );

        d_tree->SetItemText( iid, _editor->GetShortName() );
        d_tree->SortChildren( d_tree->GetRootItem() );
    }
}

void            OpenFilesListPlusPanel::    editors_del         ()
{
    //  ........................................................................
    GWR_INF("OflpPanel::editors_del([%p])", this);
    //  ........................................................................
    a_editors_array.Clear();
    a_harray.clear();
    items_del();
}

void            OpenFilesListPlusPanel::    editors_deselect    ()
{
    //  ........................................................................
    GWR_INF("OflpPanel::editors_deselect([%p])", this);
    //  ........................................................................
    items_deselect();
}
//  ============================================================================
wxButton        *   OpenFilesListPlusPanel::    button          (int _ix)
{
    return dw_header->button(_ix);
}

wxBitmapButton  *   OpenFilesListPlusPanel::    bitmap_button   (int _ix)
{
    return reinterpret_cast< wxBitmapButton* >( button(_ix) );
}

bool                OpenFilesListPlusPanel::    is_minimized    ()
{
    return ! d_tree->IsShown();
}

void                OpenFilesListPlusPanel::    minimize        ()
{
    d_tree->Show(false);

    button(1)->Enable(false);
    button(2)->Enable(false);

    dw_sizer->Layout();
}

void                OpenFilesListPlusPanel::    maximize        ()
{
    d_tree->Show(true);

    button(1)->Enable();
    button(2)->Enable();

    dw_sizer->Layout();
}

void                OpenFilesListPlusPanel::    set_bg          (wxColour& _c)
{
    this->d_tree->SetBackgroundColour(_c);

}
//  ############################################################################
//
//                          OPENFILESLISTPLUGINPANELBULK
//
//  ############################################################################
OpenFilesListPlusPanelBulk::OpenFilesListPlusPanelBulk(
        OpenFilesListPlus   *   _ofl_plugin ,
        wxWindow            *   _parent     ,
        wxString                _title      )
        :   OpenFilesListPlusPanel( _ofl_plugin, _parent, _title, true)
{
    dw_header   =   new OpenFilesListPlusPanelHeader( this, _title );

    dw_header->button_prepend   ( OflpModGfx::eBmpBarPadding );
    dw_header->button_prepend   ( OflpModGfx::eBmpBarPadding );
    dw_header->button_prepend   ( OflpModGfx::eBmpBarBlue    );
    dw_header->button_append    ( OflpModGfx::eBmpBarGreen   );

    button(1)->Enable(false);
    button(2)->Enable(false);

    button(0)->Connect(                                                         //  options
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_opt)   ,
        NULL, _ofl_plugin                                                               );

    button(3)->Connect(                                                         //  add panel
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_add)   ,
        NULL, _ofl_plugin                                                               );

    p0_create_tree();

    dw_sizer    =   new wxBoxSizer(wxVERTICAL);

    dw_sizer->Add( dw_header , 0, wxEXPAND, 0);
    dw_sizer->Add( d_tree    , 1, wxEXPAND, 0);

    this->SetSizer(dw_sizer);
}

OpenFilesListPlusPanelBulk::~OpenFilesListPlusPanelBulk()
{
}
