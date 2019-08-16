/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    <wx/tooltip.h>

#include    "oflp-common.hh"

#include    "oflp-util-hstring.hh"

//#include    "oflp-plugin.hh"
#include    "oflp-plugin-module.hh"
#include    "oflp-plugin-mod-tooltips.hh"
#include    "oflp-plugin-mod-gfx.hh"

#include    "oflp-panel-header.hh"
#include    "oflp-panel.hh"
//  ................................................................................................
#define     ERG_OFLP_SANITY_CHECKS
#include    "generated/oflp-panel-header--log-defines.cci"
//  ################################################################################################
//
//                          OPENFILESLISTPLUSPANELHEADER
//
//  ################################################################################################
bool OpenFilesListPlusPanelHeader::   z_title_ctrl_replace      (wxWindow* _vnew)
{
//  ................................................................................................
    ERG_INF("OflpPanel::p0_title_ctrl_replace():old[%p] act[%p] new[%p]", dw_txt_old, aw_txt, _vnew);
//  ................................................................................................
    if ( dw_txt_old )
        delete dw_txt_old;                                                      //  here old aw_txt is not unused anymore

    if ( ! dw_sizer->Replace( aw_txt, _vnew ) )
    {
        ERG_TKE("%s", wxS(" wxSizer::Replace() failed"));
        return false;
    }

    //  mandatory ? yes else the ctrl is still shwon until we delete it
    aw_txt->Hide();                                                             //  calling Hide() makes KILL_FOCUS to be processed !

    dw_sizer->Layout();

    dw_txt_old  =   aw_txt;                                                     //  dont delete old aw_txt here else segfault
    aw_txt      =   _vnew;

    return true;
}

void OpenFilesListPlusPanelHeader::   title_switch_to_dynamic   ()
{
    ERG_INF("%s", wxS("OflpPanelHeader::title_switch_to_dynamic()"));
//  ................................................................................................
    dw_txt_dyn = new wxTextCtrl( this, wxNewId(), a_title.get() );
    dw_txt_dyn->SetWindowStyle(wxTE_PROCESS_ENTER);                             //  allow wxEVT_COMMAND_TEXT_ENTER generation

    ERG_INF(" new ctrl[%p]", dw_txt_dyn);

    if ( ! z_title_ctrl_replace(dw_txt_dyn) )
    {
        delete dw_txt_dyn;
        return;
    }

    dw_txt_dyn->Connect(
        wxEVT_COMMAND_TEXT_ENTER                                        ,
        wxCommandEventHandler(OflpPanel::evh_title_dynamic_TEXT_ENTER)  ,
        NULL, aw_parent                                                 );      //  callback is on OflpPanel

    dw_txt_dyn->Connect(                                                        //  loosing the focus is like enter
        wxEVT_KILL_FOCUS                                                ,
        wxFocusEventHandler(OflpPanel::evh_title_dynamic_KILL_FOCUS)    ,
        NULL, aw_parent                                                 );      //  callback is on OflpPanel

    dw_txt_dyn->SetFocus();                                                     //  so the user can input text immediately
}

void OpenFilesListPlusPanelHeader::   title_switch_to_static    ()
{
    ERG_INF("%s", wxS("OflpPanelHeader::title_switch_to_static()"));
//  ................................................................................................
    a_title     = dw_txt_dyn->GetLineText(0);
    dw_txt_sta  = new wxStaticText( this, wxNewId(), a_title.get() );

    ERG_INF(" new ctrl[%p]", dw_txt_sta);

    if ( ! z_title_ctrl_replace(dw_txt_sta) )
    {
        delete dw_txt_sta;
        return;
    }

    dw_txt_sta->Connect(
        wxEVT_LEFT_DOWN                                             ,
        wxMouseEventHandler(OflpPanel::evh_title_static_LEFT_DOWN)  ,
        NULL,aw_parent                                              );          //  callback is on OflpPanel
}
//  ================================================================================================
wxButton*
OpenFilesListPlusPanelHeader::        button(int _ix)
{
    if ( _ix < 0 )
        return NULL;

    if ( _ix >= a_buttons_array.size() )
        return NULL;

    return a_buttons_array[ (size_t)_ix ];
}

void OpenFilesListPlusPanelHeader::   button_prepend  (int _i_bitmap_id, char const * _i_tooltip_text)
{
    wxSize          sz(24,16);
    wxButton    *   bt  =   NULL;
//  ................................................................................................
//  ................................................................................................
    bt  = new wxBitmapButton(this, wxNewId()                                ,
        OflpModGfx::Bitmap(_i_bitmap_id)                                    ,
        wxDefaultPosition, wxDefaultSize, wxBORDER_NONE |  wxBU_EXACTFIT    );

    if ( _i_tooltip_text)
        oflp::Modules::Instance()->tooltips()->x_add(a_trid, bt, _i_tooltip_text);

    //  wxEventHandler's ClientData is a easy way to pass extra data
    //  with the event
    bt->SetClientData( (void*)aw_parent);

    bt->SetMaxSize(sz);
    bt->SetMinSize(sz);

    a_buttons_array.insert( a_buttons_array.begin(), bt );
    dw_sizer->Prepend( bt, 0, wxEXPAND, 0 );
}

void OpenFilesListPlusPanelHeader::   button_append   (int _i_bitmap_id, char const * _i_tooltip_text)
{
    wxSize          sz(24,16);
    wxButton    *   bt  =   NULL;
    wxToolTip   *   tt  =   NULL;
//  ................................................................................................
//  ................................................................................................
    bt  = new wxBitmapButton(this, wxNewId()                                ,
        OflpModGfx::Bitmap(_i_bitmap_id)                                    ,
        wxDefaultPosition, wxDefaultSize, wxBORDER_NONE |  wxBU_EXACTFIT    );

    if ( _i_tooltip_text)
        oflp::Modules::Instance()->tooltips()->x_add(a_trid, bt, _i_tooltip_text);

    //  wxEventHandler's ClientData is a easy way to pass extra data
    //  with the event
    bt->SetClientData( (void*)aw_parent);

    bt->SetMaxSize(sz);
    bt->SetMinSize(sz);

    a_buttons_array.push_back( bt );
    dw_sizer->Add( bt, 0, wxEXPAND, 0 );
}

void OpenFilesListPlusPanelHeader::     button_show     (int _ix, bool _b)
{
    wxButton    *   bt  =   button(_ix);

    if ( ! bt )
        return;

    bt->Show(_b);
}

void OpenFilesListPlusPanelHeader::     z_set_col_bg  (wxColour& _c)
{
    this->SetBackgroundColour(_c);
}
//  ================================================================================================
OpenFilesListPlusPanelHeader::        OpenFilesListPlusPanelHeader(
        OflpPanel   *   _parent ,
        wxString        _title  )
            :   wxPanel     (_parent, wxNewId())    ,
                aw_parent   (_parent)
{
//  ................................................................................................
    a_title.set(_title);

    dw_sizer        =   new wxBoxSizer(wxHORIZONTAL);

    dw_txt_sta      =   new wxStaticText( this, wxNewId(), _title );                                //  at fist display, static text displayed
    dw_txt_dyn      =   NULL;

    aw_txt          =   dw_txt_sta;
    dw_txt_old      =   NULL;                                                                       //  no txt has been replaced yet

    dw_sizer->Add( dw_txt_sta, 1, wxEXPAND, 0);
    //dw_bs_main              ->Add(dw_sb_div                 , 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);


    this->SetSizer(dw_sizer);

    if ( ! aw_parent->is_bulk() )                                                                   //  bulk's label is not editable
        dw_txt_sta->Connect(
            wxEVT_LEFT_DOWN                                             ,
            wxMouseEventHandler(OflpPanel::evh_title_static_LEFT_DOWN)  ,
            NULL,aw_parent                                              );

    a_trid = oflp::Modules::Instance()->tooltips()->x_slot_add();
}

OpenFilesListPlusPanelHeader::       ~OpenFilesListPlusPanelHeader()
{
    oflp::Modules::Instance()->tooltips()->x_slot_sub(a_trid);
}
