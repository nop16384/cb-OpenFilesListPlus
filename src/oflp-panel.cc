/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-common.hh"

#include    "oflp-plugin.hh"

#include    "oflp-plugin-module.hh"
#include    "oflp-plugin-mod-panels.hh"
#include    "oflp-plugin-mod-gfx.hh"

#include    "oflp-panel.hh"
#include    "oflp-panel-utils.hh"
#include    "oflp-panel-utils-dnd.hh"

#include    "oflp-panel-header.hh"
//  ................................................................................................
#define     ERG_OFLP_SANITY_CHECKS
#include    "generated/oflp-panel--log-defines.cci"
//  ................................................................................................
namespace oflp
{
    wxString            UID::NullStr    ( wxString::FromUTF8("000000000000000") );
    UID         const   UID::Null       ( UID::NullStr );
}
//  ################################################################################################
//
//                          OpenFilesListPlusPanel
//
//  ################################################################################################
wxTreeItemId    OflpPanel::A_last_wxTreeItemId_dragged_and_dropped  =   0;

void OpenFilesListPlusPanel:: dump()
{
    OFLP_FUNC_ENTER_MARK("OflpPanel::dump()");
    //  ............................................................................................
    EditorBase              *   edb     =   NULL;
    //  ............................................................................................
    ERG_TKI("panel[%p][%s] editors[%lu]", this, title().wc_str(), a_editors_array.size());

    for ( size_t
        i  =   0                        ;
        i  !=  a_editors_array.size()   ;
        i++                             )
    {
        edb     =   a_editors_array.Item(i);

        ERG_TKI("Editor[%lu]:[%p][%s]"  ,
            i                           ,
            edb                         ,
            edb->GetTitle().wc_str()    );
    }
}
//  ================================================================================================
void OpenFilesListPlusPanel:: z_event_allow__kill_focus         (bool _b)
{
    //  When user enters a new panel name, TEXT_ENTER _AND_ KILL_FOCUS are
    //  called ( in this order, but whatever, the two are called ).
    //  Thats why we need some kind of flag, avoiding to delete the same object
    //  a second time when evh_title_dynamic_KILL_FOCUS handler
    //  calls p0_title_ctrl_replace()
    a_allow_kill_focus_event = _b;
}
void OpenFilesListPlusPanel:: Z_set_last_wxTreeItemId_dragged_and_dropped   (wxTreeItemId _tid)
{
    A_last_wxTreeItemId_dragged_and_dropped = _tid;
}

void OpenFilesListPlusPanel:: evh_title_static_LEFT_DOWN        (wxMouseEvent& _e)
{
    //  let OflpPanelHeader do his job
    dw_header->title_switch_to_dynamic();

    //  the header's events is connected to this ( OpenFilesListPlusPanel )
    //  only for updating this's layout, because wxStaticText and WxwxTextCtrl
    //  dont have same vertical size
    dw_sizer->Layout();
}

void OpenFilesListPlusPanel:: evh_title_dynamic_TEXT_ENTER      (wxCommandEvent& _e)
{
    ERG_INF("%s", wxS("OflpPanel:: evh_title_dynamic_TEXT_ENTER()"));

    z_event_allow__kill_focus(false);

    dw_header   ->title_switch_to_static();
    dw_sizer    ->Layout();

    z_event_allow__kill_focus(true);

    _e.Skip();
}

void OpenFilesListPlusPanel:: evh_title_dynamic_KILL_FOCUS      (wxFocusEvent   & _e)
{
    ERG_INF("%s", wxS("OflpPanel:: evh_title_dynamic_KILL_FOCUS()"));

    if ( ! a_allow_kill_focus_event )
    {
        ERG_INF("%s", wxS("  (aborted)"));
        return;
    }

    dw_header   ->title_switch_to_static();
    dw_sizer    ->Layout();

    _e.Skip();
}
//  ================================================================================================
void OpenFilesListPlusPanel:: z_create_tree                     ()
{
    d_tree          =   new wxTreeCtrl( this, wxNewId(), wxDefaultPosition, wxSize(150, 100),
        wxTR_HAS_BUTTONS | wxNO_BORDER | wxTR_HIDE_ROOT );
    d_tree->AddRoot( wxString::FromUTF8("ROOT") );

    d_tree->SetImageList(const_cast< wxImageList* >( oflp::Modules::Instance()->gfx()->image_list() ));

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
//  ================================================================================================
wxTreeItemId    OpenFilesListPlusPanel::    item_find           (EditorBase* _editor)
{
    OflpPanelTiData     *   tid     =   nullptr;
    EditorBase          *   ed      =   nullptr;
    wxTreeItemIdValue       cookie  =   0;
    wxTreeItemId            iid     =   d_tree->GetFirstChild( d_tree->GetRootItem(), cookie );
    //  ............................................................................................
    //D ERG_INF("%s", wxS("OpenFilesListPlusPanel::item_find()"));
    //  ............................................................................................
    // loop all tree items
    while ( iid )
    {
        tid =   static_cast< OflpPanelTiData* >(d_tree->GetItemData(iid));
        ed  =   tid->x_get_editor();
        if ( _editor == ed )
        {
            //D ERG_INF("%s", wxS("OpenFilesListPlusPanel::item_find():found"));
            return iid;
        }
        iid = d_tree->GetNextChild(d_tree->GetRootItem(), cookie);
    }
    //D ERG_INF("%s", wxS("OpenFilesListPlusPanel::item_find():not found"));
    iid.Unset();
    return iid;
}

wxTreeItemId    OpenFilesListPlusPanel::    item_append         (EditorBase* _editor)
{
    wxTreeItemId        iid;
    int                 mod = OflpModGfx::Icon(_editor);
    //  ............................................................................................
    iid = d_tree->AppendItem(
        d_tree->GetRootItem()   ,
        _editor->GetShortName() ,
        mod, mod                ,
        new OflpPanelTiData(this, _editor)   );

    if ( ! iid.IsOk() )
    {
        ERG_TKE("%s", wxS("OpenFilesListPlusPanel::item_append():failed"));
        iid.Unset();
        return iid;
    }

    //D ERG_INF("OflpPanel::item_append():success img[%i]", mod);

    d_tree->SortChildren(d_tree->GetRootItem());

    return iid;
}

bool            OpenFilesListPlusPanel::    item_del            (EditorBase* _editor)
{
    wxTreeItemId    iid = item_find(_editor);
    //  ............................................................................................
    if ( ! iid.IsOk() )
    {
        ERG_TKE("%s", wxS("OpenFilesListPlusPanel::item_del():failed"));
        return false;
    }

    //  delete d_tree->GetItemData( iid );                                      //  wxTreeCtrl does it by itself

    d_tree->Delete(iid);

    return true;
}

bool            OpenFilesListPlusPanel::    item_select         (EditorBase* _editor)
{
    wxTreeItemId    iid = item_find(_editor);
    //  ............................................................................................
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
    //  ............................................................................................
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
//  ================================================================================================
bool            OpenFilesListPlusPanel::    editor_has          (EditorBase* _editor)
{
    //  ............................................................................................
    ERG_TKI("OflpPanel::editor_has():[%p][%s][%p]", this, title().wc_str(), _editor);
    //  ............................................................................................
    dump();

    int ix = a_editors_array.Index(_editor);

    return ( a_editors_array.Index(_editor) != wxNOT_FOUND );
}

bool            OpenFilesListPlusPanel::    editor_add          (EditorBase* _editor)
{
    wxTreeItemId    iid;
    int             mod =   OflpModGfx::Icon(_editor);
    //  ............................................................................................
    ERG_TKI("OflpPanel::editor_add():[%p][%s][%p]", this, title().wc_str(), _editor);
    //  ............................................................................................
    iid = item_append(_editor);

    if ( ! iid.IsOk() )
    {
        return false;
    }

    a_editors_array.Add(_editor);

    dump();

    return true;
}

bool            OpenFilesListPlusPanel::    editor_del          (EditorBase* _editor)
{
    //  ............................................................................................
    ERG_TKI("OflpPanel::editor_del():[%p][%s][%p]", this, title().wc_str(), _editor);
    //  ............................................................................................
    if ( ! item_del(_editor) )
        return false;

    dump();

    a_editors_array.Remove(_editor);

    dump();

    return true;
}

void            OpenFilesListPlusPanel::    editor_select       (EditorBase* _editor)
{
    //  ............................................................................................
    ERG_TKI("OflpPanel::editor_select():[%p][%p]", this, _editor);
    //  ............................................................................................
    item_select(_editor);
}

bool            OpenFilesListPlusPanel::    editor_selected     (EditorBase* _editor)
{
    //  ............................................................................................
    ERG_INF("%s", wxS("OflpPanel::editor_selected()"));
    //  ............................................................................................
    bool b = item_selected(_editor);

    ERG_INF("OflpPanel::editor_selected():[%s][%i]",
            _editor->GetShortName().wc_str()    ,
            b                                   );

    return b;
}

void            OpenFilesListPlusPanel::    editor_sync         (EditorBase* _editor)
{
    wxTreeItemId    iid;
    int             mod =   OflpModGfx::Icon(_editor);
    //  ............................................................................................
    ERG_INF("%s", wxS("OflpPanel::editor_sync()") );
    //  ............................................................................................
    iid = item_find(_editor);

    if ( ! iid.IsOk() )
    {
        return;
    }

    if ( d_tree->GetItemImage(iid, wxTreeItemIcon_Normal)  != mod )
    {
        ERG_INF("%s", wxS("editor_sync():editor icon changed"));

        d_tree->SetItemImage( iid, mod , wxTreeItemIcon_Normal );
        d_tree->SetItemImage( iid, mod , wxTreeItemIcon_Selected );
    }

    if ( d_tree->GetItemText(iid).Cmp( _editor->GetShortName() ) )
    {
        ERG_INF("editor_sync():editor name changed [%s] -> [%s]",
            d_tree->GetItemText(iid).wc_str()   ,
             _editor->GetShortName().wc_str()   );

        d_tree->SetItemText( iid, _editor->GetShortName() );
        d_tree->SortChildren( d_tree->GetRootItem() );
    }
}

void            OpenFilesListPlusPanel::    editors_del         ()
{
    //  ............................................................................................
    ERG_INF("OflpPanel::editors_del([%p])", this);
    //  ............................................................................................
    a_editors_array.Clear();
    items_del();
}

void            OpenFilesListPlusPanel::    editors_deselect    ()
{
    //  ............................................................................................
    ERG_INF("OflpPanel::editors_deselect([%p])", this);
    //  ............................................................................................
    items_deselect();
}
//  ================================================================================================
wxString const  &   OpenFilesListPlusPanel::title()                                         const
{
    return dw_header->title();
}

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

void                OpenFilesListPlusPanel::    z_set_col_bg  (wxColour& _c)
{
    this->d_tree->SetBackgroundColour(_c);
}
//  ================================================================================================
OpenFilesListPlusPanel::OpenFilesListPlusPanel(
        wxWindow            *   _parent     ,
        wxString                _title      ,
        oflp::UID               _uid        ,
        bool                    _bulk       )
        :   wxPanel                         ( _parent, wxNewId())   ,
            a_uid                           ( _uid              )   ,
            a_bulk                          ( _bulk             )   ,
            a_allow_kill_focus_event        ( true              )
{
    ERG_TKI("OpenFilesListPlusPanel::OpenFilesListPlusPanel():[%p]", this);

    if ( is_bulk() )
        return;

    if ( a_uid.is_null() )
        a_uid.renew();

    SetFont( oflp::Modules::Instance()->gfx()->fnt8() );

    dw_header   =   new OpenFilesListPlusPanelHeader( this, _title );

    dw_header->button_prepend   ( OflpModGfx::eBmpBarUp      ,  "Move up"   );                      //  2
    dw_header->button_prepend   ( OflpModGfx::eBmpBarDown    ,  "MoveDown"  );                      //  1
    dw_header->button_prepend   ( OflpModGfx::eBmpBarOrange  ,  "Minimize"  );                      //  0
    dw_header->button_append    ( OflpModGfx::eBmpBarRed     ,  "Delete"    );                      //  3

    //  setting "disabled bitmap" to padding will pseudo-hide the button
    bitmap_button(1)->SetBitmapDisabled( OflpModGfx::Bitmap(OflpModGfx::eBmpBarPadding) );
    bitmap_button(2)->SetBitmapDisabled( OflpModGfx::Bitmap(OflpModGfx::eBmpBarPadding) );

    button(0)->Connect(                                                         //  (mini | maxi) mize
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_mm)  ,
        NULL, OpenFilesListPlus::Instance()                                             );

    button(2)->Connect(                                                         //  up
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_up)  ,
        NULL, OpenFilesListPlus::Instance()                                             );

    button(1)->Connect(                                                         //  down
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_down)  ,
        NULL, OpenFilesListPlus::Instance()                                             );

    button(3)->Connect(                                                         //  del
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_del)   ,
        NULL, OpenFilesListPlus::Instance()                                             );

    z_create_tree();

    dw_sizer    =   new wxBoxSizer(wxVERTICAL);

    dw_sizer->Add( dw_header , 0, wxEXPAND, 0);
    dw_sizer->Add( d_tree    , 1, wxEXPAND, 0);

    this->SetSizer(dw_sizer);
}

OpenFilesListPlusPanel::~OpenFilesListPlusPanel()
{
}
//  ################################################################################################
//
//                          OpenFilesListPlusPanelBulk
//
//  ################################################################################################
OpenFilesListPlusPanelBulk::OpenFilesListPlusPanelBulk(
        wxWindow            *   _parent     ,
        wxString                _title      )
        :   OpenFilesListPlusPanel(_parent, _title, oflp::UID::Null, true)
{
    dw_header   =   new OpenFilesListPlusPanelHeader( this, _title );

    dw_header->button_prepend   ( OflpModGfx::eBmpBarPadding);
    dw_header->button_prepend   ( OflpModGfx::eBmpBarPadding);
    dw_header->button_prepend   ( OflpModGfx::eBmpBarBlue    ,  "Options"   );
    dw_header->button_append    ( OflpModGfx::eBmpBarGreen   ,  "New panel" );

    button(1)->Enable(false);
    button(2)->Enable(false);

    button(0)->Connect(                                                         //  options
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_opt)   ,
        NULL, OpenFilesListPlus::Instance()                                             );

    button(3)->Connect(                                                         //  add panel
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_add)   ,
        NULL, OpenFilesListPlus::Instance()                                             );

    z_create_tree();

    dw_sizer    =   new wxBoxSizer(wxVERTICAL);

    dw_sizer->Add( dw_header , 0, wxEXPAND, 0);
    dw_sizer->Add( d_tree    , 1, wxEXPAND, 0);

    this->SetSizer(dw_sizer);
}

OpenFilesListPlusPanelBulk::~OpenFilesListPlusPanelBulk()
{
}
