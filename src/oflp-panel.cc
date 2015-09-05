#include "sdk.h"                                                                // Code::Blocks SDK
//  ............................................................................
#ifndef CB_PRECOMP
    #include "manager.h"
    #include "configmanager.h"
    #include "configurationpanel.h"
    #include "editormanager.h"
    #include "projectmanager.h"
    #include "logmanager.h"
    #include "editorbase.h"
    #include "cbeditor.h"
    #include "sdk_events.h"
    #include "misctreeitemdata.h"
#endif
//  ............................................................................
#include    "oflp-panel.hh"
#include    "oflp-plugin.hh"
//  ............................................................................
#define GWR_OFLP_SANITY_CHECKS
#define GWR_LOG(FORMAT, ...)    GWRCB_LOG(FORMAT, __VA_ARGS__);
#define GWR_TKI(FORMAT, ...)    GWRCB_TKI(FORMAT, __VA_ARGS__);
#define GWR_TKE(FORMAT, ...)    GWRCB_TKE(FORMAT, __VA_ARGS__);
#define GWR_INF(FORMAT, ...)    GWRCB_INF(FORMAT, __VA_ARGS__);
#define GWR_WNG(FORMAT, ...)    GWRCB_WNG(FORMAT, __VA_ARGS__);
#define GWR_ERR(FORMAT, ...)    GWRCB_ERR(FORMAT, __VA_ARGS__);
//  ############################################################################
#include    "oflp-panel-header.cci"
#include    "oflp-panel-utils.cci"
//  ############################################################################
//
//                          OPENFILESLISTPLUGINPANEL
//
//  ############################################################################
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
    OFLPPanelDataObject     dobj;
    wxTreeItemId            iid     =   _e.GetItem();                           //  _GWR_REM_ always valid ( cf wxdoc )
    wxTreeItemData      *   idata   =   d_tree->GetItemData(iid);
    OFLPPanelItemData   *   pidata  =   NULL;
    EditorBase          *   editor  =   NULL;

    OFLPPanel           *   dpanel  =   NULL;
    //  ........................................................................
    earlgreycb::Log_function_enter(wxS("OFLPPanel::OnDragInit()"));
    //  ........................................................................
    GWR_INF("OnDragInit():object format count [%i]", GWR_SIZE_T_TO_INT( dobj.GetFormatCount()) );
    //  ........................................................................
    //  You can init a drag anywhere inside the wxTreeCtrl, even where there is
    //  nothing ! In this case, iid IsOk() ( why ??? ) , but idata is NULL
    if ( ! iid.IsOk() )                                                         //  _GWR_UNUSEFUL_ should never happen
        goto lab_return;

    if ( ! idata )
        goto lab_return;

    pidata  =   static_cast< OFLPPanelItemData* >( idata );
    editor  =   pidata->GetEditor();
    //  ........................................................................
    //  prepare drag source
    dobj.SetData( plugin()->panel_get_visual_index(this) , 0xab, editor );      //  _GWR_TODO_ iid
    //wxDropSource        dropSource(dobj, d_tree);
    dropSource.SetData(dobj);
    //  ........................................................................
    // do DnD
    dres = dropSource.DoDragDrop( wxDrag_DefaultMove );
    if ( dres != wxDragMove )
    {
        GWR_ERR("           OnDragInit():res[%s], expected wxDragMove", OpenFilesListPlusPanel::stringize_drag_result(dres).c_str() );
        goto lab_return;
    }
    //  ........................................................................
    //  delete our item ; this will cause a SELECTION_CHANGE event with a       //  _GWR_TECH_
    //  NULL data
    editor_del( static_cast< OFLPPanelItemData* >( idata )->GetEditor() );

    dpanel  =   OpenFilesListPlus::Instance()->a_dnd_destination_panel;
    if ( ! dpanel )
    {
        GWR_TKE("%s", wxS("OnDragInit():NULL destination panel"));
        goto lab_return;
    }
    dpanel->editor_add(editor);
    dpanel->editor_select(editor);
    //  ........................................................................
lab_return:
    earlgreycb::Log_function_exit();
    return;
}

void OpenFilesListPlusPanel:: OnDragEnd   (wxTreeEvent& _e)
{
    GWR_INF("%s", _T("OFLPPanel::OnDragEnd()") );
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
    //  let OFLPPanelHeader do his job
    dw_header->title_switch_to_dynamic();

    //  the header's events is connected to this ( OpenFilesListPlusPanel )
    //  only for updating this's layout, because wxStaticText and WxwxTextCtrl
    //  dont have same vertical size
    dw_sizer->Layout();
}

void OpenFilesListPlusPanel:: evh_title_dynamic_TEXT_ENTER        (wxCommandEvent& _e)
{
    GWR_INF("%s", _T("OFLPPanel:: evh_title_dynamic_TEXT_ENTER()"));

    p0_allow_kill_focus_event(false);

    dw_header   ->title_switch_to_static();
    dw_sizer    ->Layout();

    p0_allow_kill_focus_event(true);

    _e.Skip();
}

void OpenFilesListPlusPanel:: evh_title_dynamic_KILL_FOCUS        (wxFocusEvent   & _e)
{
    GWR_INF("%s", _T("OFLPPanel:: evh_title_dynamic_KILL_FOCUS()"));

    if ( ! a_allow_kill_focus_event )
    {
        GWR_INF("%s", _T("  (aborted)"));
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
        wxTreeEventHandler(OpenFilesListPlus::evh_tree_item_activated)      , NULL, a_ofl_plugin);
    d_tree->Connect(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK,
        wxTreeEventHandler(OpenFilesListPlus::evh_tree_item_right_click)    , NULL, a_ofl_plugin);
    d_tree->Connect(wxEVT_COMMAND_TREE_SEL_CHANGED,
        wxTreeEventHandler(OpenFilesListPlus::evh_tree_sel_changed)         , NULL, a_ofl_plugin);

    d_drop_target   =   new OpenFilesListPlusPanelDropTarget(d_tree, this);
    d_tree->SetDropTarget(d_drop_target);
}
//  ============================================================================
OpenFilesListPlusPanel::OpenFilesListPlusPanel(
        OpenFilesListPlus *   _ofl_plugin ,
        wxWindow            *   _parent ,
        wxString                _title  ,
        bool                    _bulk   )
        :   wxPanel                     ( _parent, wxNewId()    )   ,
            a_ofl_plugin                ( _ofl_plugin   )   ,
            a_bulk                      ( _bulk         )   ,
            a_allow_kill_focus_event    ( true          )
{
    GWR_INF("OpenFilesListPlusPanel::OpenFilesListPlusPanel():[%p] plugin[%p]", this, _ofl_plugin);

    SetFont( OpenFilesListPlus::Instance()->gfx()->fnt8() );

    dw_header   =   new OpenFilesListPlusPanelHeader( this, _title );

    dw_header->button_prepend   ( OpenFilesListPlus::eBmpBarOrange  );
    dw_header->button_append    ( OpenFilesListPlus::eBmpBarUp      );
    dw_header->button_append    ( OpenFilesListPlus::eBmpBarDown    );
    dw_header->button_append    ( OpenFilesListPlus::eBmpBarRed     );

    dw_header->button(0)->Connect(                                              //  (mini | maxi) mize
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_mm)  ,
        NULL, _ofl_plugin                                                               );

    dw_header->button(1)->Connect(                                              //  up
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_up)  ,
        NULL, _ofl_plugin                                                               );

    dw_header->button(2)->Connect(                                              //  down
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_down)  ,
        NULL, _ofl_plugin                                                               );

    dw_header->button(3)->Connect(                                              //  del
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
void            OpenFilesListPlusPanel::  reset()
{
}
//  ============================================================================
wxTreeItemId    OpenFilesListPlusPanel::  item_find           (EditorBase* _editor)
{
    wxTreeItemIdValue   cookie  =   0;
    wxTreeItemId        iid     = d_tree->GetFirstChild( d_tree->GetRootItem(), cookie );
    //  ........................................................................
    //D GWR_INF("%s", _T("OpenFilesListPlusPanel::item_find()"));
    //  ........................................................................
    // loop all tree items
    while ( iid )
    {
        OFLPPanelItemData       *   data    = static_cast<OFLPPanelItemData*>(d_tree->GetItemData(iid));
        EditorBase              *   e       = data->GetEditor();
        if ( _editor == e )
        {
            //D GWR_INF("%s", _T("OpenFilesListPlusPanel::item_find():found"));
            return iid;
        }
        iid = d_tree->GetNextChild(d_tree->GetRootItem(), cookie);
    }
    //D GWR_INF("%s", _T("OpenFilesListPlusPanel::item_find():not found"));
    iid.Unset();
    return iid;
}

wxTreeItemId    OpenFilesListPlusPanel::  item_append         (EditorBase* _editor)
{
    wxTreeItemId        iid;
    int                 mod = OpenFilesListPlus::Instance()->gfx()->icon(_editor);
    //  ........................................................................
    //  ........................................................................
    iid = d_tree->AppendItem(
        d_tree->GetRootItem()   ,
        _editor->GetShortName() ,
        mod, mod                ,
        new OFLPPanelItemData(this, _editor)   );

    if ( ! iid.IsOk() )
    {
        GWR_ERR("%s", _T("OpenFilesListPlusPanel::item_append():failed"));
        iid.Unset();
        return iid;
    }

    GWR_INF("OpenFilesListPlusPanel::item_append():success img[%i]", mod);

    d_tree->SortChildren(d_tree->GetRootItem());

    return iid;
}

bool            OpenFilesListPlusPanel::  item_del            (EditorBase* _editor)
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

bool            OpenFilesListPlusPanel::  item_select         (EditorBase* _editor)
{
    wxTreeItemId    iid = item_find(_editor);
    //  ........................................................................
    if ( ! iid.IsOk() )
    {
        return false;
    }

    if ( d_tree->GetSelection() == iid )
        return true;

    d_tree->SelectItem(iid);
    return true;
}

bool            OpenFilesListPlusPanel::  item_selected       (EditorBase* _editor)
{
    wxTreeItemId    iid = d_tree->GetSelection();

    if ( ! iid.IsOk() )
    {
        return false;
    }

    OFLPPanelItemData   *   idata = static_cast< OFLPPanelItemData* >( d_tree->GetItemData(iid) );

    if ( ! idata )
    {
        return false;
    }

    if ( idata->GetEditor() == _editor )
        return true;

    return false;
}

void            OpenFilesListPlusPanel::  items_deselect      ()
{
    d_tree->UnselectAll();
}

void            OpenFilesListPlusPanel::  items_del           ()
{
    d_tree->DeleteChildren(d_tree->GetRootItem());
}
//  ============================================================================
int             OpenFilesListPlusPanel::  editor_index    (EditorBase* _editor)
{
    return a_editors_array.Index(_editor);
}

void            OpenFilesListPlusPanel::  editor_add      (EditorBase* _editor)
{
    wxTreeItemId    iid;
    int             mod =   OpenFilesListPlus::Instance()->gfx()->icon(_editor);
    //  ........................................................................
    GWR_TKI("OFLPPanel::editor_add():[%p][%p]", this, _editor);
    //  ........................................................................
    iid = item_append(_editor);

    if ( ! iid.IsOk() )
    {
        return;
    }

    a_editors_array.Add(_editor);

    //d_tree->SetItemImage(iid, mod, wxTreeItemIcon_Normal);
    //d_tree->SetItemImage(iid, mod, wxTreeItemIcon_Selected);
}

void            OpenFilesListPlusPanel::  editor_del      (EditorBase* _editor)
{
    //  ........................................................................
    GWR_TKI("OFLPPanel::editor_del():[%p][%p]", this, _editor);
    //  ........................................................................
    item_del(_editor);

    a_editors_array.Remove(_editor);
}

void            OpenFilesListPlusPanel::  editor_select   (EditorBase* _editor)
{
    //  ........................................................................
    GWR_TKI("OFLPPanel::editor_select():[%p][%p]", this, _editor);
    //  ........................................................................
    item_select(_editor);
}

bool            OpenFilesListPlusPanel::  editor_selected (EditorBase* _editor)
{
    //  ........................................................................
    GWR_INF("%s", _T("OFLPPanel::editor_selected()"));
    //  ........................................................................
    return item_selected(_editor);
}

void            OpenFilesListPlusPanel::  editor_sync     (EditorBase* _editor)
{
    wxTreeItemId    iid;
    int             mod =   OpenFilesListPlus::Instance()->gfx()->icon(_editor);
    //  ........................................................................
    GWR_INF("%s", _T("OFLPPanel::editor_refresh()") );
    //  ........................................................................
    iid = item_find(_editor);

    if ( ! iid.IsOk() )
    {
        return;
    }

    if ( d_tree->GetItemImage(iid, wxTreeItemIcon_Normal)  != mod )
    {
        d_tree->SetItemImage( iid, mod , wxTreeItemIcon_Normal );
        d_tree->SetItemImage( iid, mod , wxTreeItemIcon_Selected );
    }
    ////if ( d_tree->GetItemText(iid)   != _editor->GetShortName() )
    ////{
        ////d_tree->SetItemText( iid, _editor->GetShortName() );
        ////d_tree->SortChildren( d_tree->GetRootItem() );
    ////}
}

void            OpenFilesListPlusPanel::  editors_del     ()
{
    //  ........................................................................
    GWR_INF("OFLPPanel::editors_del([%p])", this);
    //  ........................................................................
    a_editors_array.Clear();
    items_del();
}

void            OpenFilesListPlusPanel::  editors_deselect()
{
    //  ........................................................................
    GWR_INF("OFLPPanel::editors_deselect([%p])", this);
    //  ........................................................................
    items_deselect();
}
//  ============================================================================
bool            OpenFilesListPlusPanel::  is_minimized    ()
{
    return ! d_tree->IsShown();
}

void            OpenFilesListPlusPanel::  minimize        ()
{
    d_tree->Show(false);

    dw_header->button_show( 1, false);
    dw_header->button_show( 2, false);
}

void            OpenFilesListPlusPanel::  maximize        ()
{
    d_tree->Show(true);

    dw_header->button_show( 1, true);
    dw_header->button_show( 2, true);
}
//  ############################################################################
//
//                          OPENFILESLISTPLUGINPANELBULK
//
//  ############################################################################
OpenFilesListPlusPanelBulk::OpenFilesListPlusPanelBulk(
        OpenFilesListPlus *   _ofl_plugin ,
        wxWindow            *   _parent ,
        wxString                _title  )
        :   OpenFilesListPlusPanel( _ofl_plugin, _parent, _title, true)   ,
            dw_menu_main                            (NULL)  ,
                dw_menu_selection_mode              (NULL)  ,
                    dw_item_selection_standard      (NULL)  ,
                    dw_item_selection_productivity  (NULL)
{
    dw_header   =   new OpenFilesListPlusPanelHeader( this, _title );

    dw_header->button_prepend   ( OpenFilesListPlus::eBmpBarBlue    );
    dw_header->button_append    ( OpenFilesListPlus::eBmpBarGreen   );

    dw_header->button(0)->Connect(                                              //  options
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_opt) ,
        NULL, _ofl_plugin                                                               );

    dw_header->button(1)->Connect(                                              //  add panel
        wxEVT_COMMAND_BUTTON_CLICKED                                                    ,
        wxCommandEventHandler(OpenFilesListPlus::evh_panel_header_button_clicked_add) ,
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
