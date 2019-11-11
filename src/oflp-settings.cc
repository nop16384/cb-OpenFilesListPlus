/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-common.hh"

#include    "oflp-plugin.hh"
#include    "oflp-plugin-mod-settings.hh"
#include    "oflp-plugin-mod-gfx.hh"

#include    "oflp-settings.hh"
//  ................................................................................................
//  specific to this file oflp defines
#define ERG_OFLP_SANITY_CHECKS



//  ################################################################################################
//
//                          OPENFILESLISTPLUSSETTINGS
//
//  ################################################################################################
void            OpenFilesListPlusSettings:: opt_log__sync_widgets_activity()
{
    if ( ! oflp::Modules::Instance()->settings()->emb_menu__log() )
        return;

    if ( dw_log_cb_enable->GetValue() )
    {
        dw_log_cb_ew    ->Enable(true);
        dw_log_cb_ec    ->Enable(true);
    }
    else
    {
        dw_log_cb_ew    ->Enable(false);
        dw_log_cb_ec    ->Enable(false);
    }
}
void            OpenFilesListPlusSettings:: opt_log__sync_from_var          (OflpOptLog & _o)
{
    if ( ! oflp::Modules::Instance()->settings()->emb_menu__log() )
        return;

    a_opt_log   =   _o;
    opt_log__sync_from_internals();
}
void            OpenFilesListPlusSettings:: opt_log__sync_from_widgets      ()
{
    if ( ! oflp::Modules::Instance()->settings()->emb_menu__log() )
        return;

    a_opt_log.a_enabled =   dw_log_cb_enable->GetValue();
    a_opt_log.a_window  =   dw_log_cb_ew->GetValue();
    a_opt_log.a_console =   dw_log_cb_ec->GetValue();
}
void            OpenFilesListPlusSettings:: opt_log__sync_from_internals    ()
{
    if ( ! oflp::Modules::Instance()->settings()->emb_menu__log() )
        return;

    dw_log_cb_enable    ->SetValue(a_opt_log.a_enabled);
    dw_log_cb_ew        ->SetValue(a_opt_log.a_window);
    dw_log_cb_ec        ->SetValue(a_opt_log.a_console);

    opt_log__sync_widgets_activity();
}

void            OpenFilesListPlusSettings:: opt_sel__sync_from_var          (OflpOptSel & _o)
{
    a_opt_sel   =   _o;
    opt_sel__sync_from_internals();
}
void            OpenFilesListPlusSettings:: opt_sel__sync_from_widgets      ()
{
    a_opt_sel.sclick( dw_sel_cb_single->GetValue() );
    a_opt_sel.dclick( dw_sel_cb_double->GetValue() );
}
void            OpenFilesListPlusSettings:: opt_sel__sync_from_internals    ()
{
    dw_sel_cb_single->SetValue(a_opt_sel.sclick());
    dw_sel_cb_double->SetValue(a_opt_sel.dclick());
}

void            OpenFilesListPlusSettings:: opt_div_tt__sync_from_var       (OflpOptDivTt & _o)
{
    a_opt_div_tt    =   _o;
    opt_div_tt__sync_from_internals();
}
void            OpenFilesListPlusSettings:: opt_div_tt__sync_from_widgets   ()
{
    a_opt_div_tt.a_show = dw_cb_div_enable_tooltips->GetValue();
}
void            OpenFilesListPlusSettings:: opt_div_tt__sync_from_internals ()
{
    dw_cb_div_enable_tooltips->SetValue(a_opt_div_tt.a_show);
}

void            OpenFilesListPlusSettings:: opt_app_col__sync_from_widgets()
{
   a_opt_colors.a_bg_p.Set( dw_app_tc__color_bg_panels->GetLineText(0) );
   a_opt_colors.a_bg_h.Set( dw_app_tc__color_bg_headers->GetLineText(0) );
}


void            OpenFilesListPlusSettings:: xport(OflpOptLog& _olog, OflpOptSel& _osel, OflpOptDivTt& _odivtt, OflpOptColors& _ocolors)
{
    opt_log__sync_from_widgets();
    opt_sel__sync_from_widgets();
    opt_div_tt__sync_from_widgets();
    opt_app_col__sync_from_widgets();

    _olog   =   a_opt_log;
    _osel   =   a_opt_sel;
    _odivtt =   a_opt_div_tt;
    _ocolors=   a_opt_colors;
}
//  ################################################################################################
void            OpenFilesListPlusSettings:: evh_check_box_clicked       (wxCommandEvent & _e)
{
    //ERG_INF("ClientData[%p] ClientObject[%p] EventObject[%p]", _e.GetClientData(), _e.GetClientObject(), _e.GetEventObject());

    if ( oflp::Modules::Instance()->settings()->emb_menu__log() )
    {

    if  (   ( _e.GetEventObject() == dw_log_cb_enable       )   ||
            ( _e.GetEventObject() == dw_log_cb_ew           )   ||
            ( _e.GetEventObject() == dw_log_cb_ec           )   )
    {
        opt_log__sync_widgets_activity();

        opt_log__sync_from_widgets();
    }

    }

    if  (   ( _e.GetEventObject() == dw_sel_cb_single   )   )
    {
        dw_sel_cb_double->SetValue( ! dw_sel_cb_single->GetValue() );                               //  sync the other checkbox
        opt_sel__sync_from_widgets();
    }

    if  (   ( _e.GetEventObject() == dw_sel_cb_double   )   )
    {
        dw_sel_cb_single->SetValue( ! dw_sel_cb_double->GetValue() );                               //  sync the other checkbox
        opt_sel__sync_from_widgets();
    }

    if  (   ( _e.GetEventObject() == dw_cb_div_enable_tooltips )   )
    {
        opt_div_tt__sync_from_widgets();
    }
}

void            OpenFilesListPlusSettings:: evh_button_clicked_dbg01    (wxCommandEvent& _i_evt)
{
    //  ........................................................................
    //OFLP_LOG_FUNC_ENTER("OFLP::Settings::action()");
    //  ........................................................................
    //  ........................................................................
    ERGCB_LOG("%s", "OpenFilesListPlusSettings::evh_button_clicked_dbg01()");

    //OFLP_LOG_FUNC_EXIT();
}
//  ################################################################################################
void            OpenFilesListPlusSettings:: z_build_widgets_01(
    OflpOptLog      &   _opt_log    ,
    OflpOptSel      &   _opt_sel    ,
    OflpOptDivTt    &   _opt_div_tt ,
    OflpOptColors   &   _opt_colors )
{
    //  ............................................................................................
    //  reserve ids for checkboxes
    wxWindowID idb                      =   wxID_HIGHEST + 1;
    //  ............................................................................................
    dw_bs_main                          =   new wxBoxSizer( wxVERTICAL );
    //  ............................................................................................
    //  log options
    if ( oflp::Modules::Instance()->settings()->emb_menu__log() )
    {

    wxPanel         *pn1, *pn2, *pn3, *pn4;
    wxSizer         *bs0, *bs1, *bs2, *bs4;

    dw_log_cp                           =   new wxCollapsiblePane( this, wxID_ANY, wxS("Logs"));
    aw_log_cp_wn                        =   dw_log_cp->GetPane();
    aw_log_cp_wn->SetFont( oflp::Modules::Instance()->gfx()->fnt8() );

    bs0                                 =   new wxBoxSizer(wxVERTICAL);

      pn1                               =   new wxPanel(aw_log_cp_wn, wxID_ANY);
      bs1                               =   new wxBoxSizer(wxVERTICAL);

        dw_log_cb_enable                =   new wxCheckBox(pn1, idb + 0, wxS("Enable logs"), wxDefaultPosition, wxDefaultSize, 0);

        pn2                             =   new wxPanel(pn1, wxNewId());
        bs2                             =   new wxBoxSizer( wxHORIZONTAL );

          pn3                           =   new wxPanel(pn2, wxNewId());

          pn4                           =   new wxPanel(pn2, wxNewId());
          bs4                           =   new wxBoxSizer( wxVERTICAL );

            dw_log_cb_ew                =   new wxCheckBox(pn4, idb + 1, wxS("window"));
            dw_log_cb_ec                =   new wxCheckBox(pn4, idb + 2, wxS("console"));

          bs4               ->Add(dw_log_cb_ew ,        1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
          bs4               ->Add(dw_log_cb_ec ,        1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
          pn4               ->SetSizer(bs4);

        bs2                 ->Add(pn3 ,                 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
        bs2                 ->Add(pn4 ,                 3, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
        pn2                 ->SetSizer(bs2);
      bs1                   ->Add(dw_log_cb_enable ,    1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);    //  1 and 2 propoertions
      bs1                   ->Add(pn2 ,                 2, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);    //  are ideal, dont change them
      pn1                   ->SetSizer(bs1);

    bs0                     ->Add(pn1 ,                 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    aw_log_cp_wn            ->SetSizer(bs0);
    bs0                     ->SetSizeHints(aw_log_cp_wn);

    }
    //  ............................................................................................
    //  file selection mode option
    {

    dw_sel_cp                           = new wxCollapsiblePane( this, wxID_ANY, wxS("Selection"));
    aw_sel_pn1                          = dw_sel_cp->GetPane();
    aw_sel_pn1->SetFont( oflp::Modules::Instance()->gfx()->fnt8() );

    wxBoxSizer  *   s = new wxBoxSizer(wxVERTICAL);
    wxPanel     *   p = new wxPanel(aw_sel_pn1, wxID_ANY);

      dw_sel_sz1                        = new wxBoxSizer(wxVERTICAL);
        dw_sel_cb_double                = new wxCheckBox(p, idb + 3, wxS("double click"));
        dw_sel_cb_single                = new wxCheckBox(p, idb + 4, wxS("simple click"));

    dw_sel_sz1->Add( dw_sel_cb_double, 1, wxGROW | wxALL, 0);
    dw_sel_sz1->Add( dw_sel_cb_single, 1, wxGROW | wxALL, 0);

    p->SetSizer(dw_sel_sz1);

    s->Add(p, 1, wxGROW | wxALL, 0);
    aw_sel_pn1->SetSizer(s);
    s->SetSizeHints(aw_sel_pn1);

    }
    //  ............................................................................................
    //  divers actions
    {

    wxSizer     *   sz1;

    dw_div_cp                           = new wxCollapsiblePane( this, wxID_ANY, wxS("Divers"));
    aw_div_cp_wn                        = dw_div_cp->GetPane();
    dw_div_cp->GetPane()->SetFont( oflp::Modules::Instance()->gfx()->fnt8() );

    sz1                                 = new wxBoxSizer(wxVERTICAL);
      dw_cb_div_enable_tooltips         = new wxCheckBox(aw_div_cp_wn, idb + 5, wxS("enable tooltips"));

    sz1                     ->Add( dw_cb_div_enable_tooltips, 1, wxGROW|wxALL, 0);
    aw_div_cp_wn            ->SetSizer(sz1);
    sz1                     ->SetSizeHints(aw_div_cp_wn);

    }
    //  ............................................................................................
    //  debug actions
    if ( oflp::Modules::Instance()->settings()->emb_menu__dbg() )
    {

    wxSizer     *   sz1;

    dw_dbg_cp                           =   new wxCollapsiblePane( this, wxID_ANY, wxS("Debug"));
    aw_dbg_cp_wn                        =   dw_dbg_cp->GetPane();
    aw_dbg_cp_wn->SetFont( oflp::Modules::Instance()->gfx()->fnt8() );

    sz1                                 =   new wxBoxSizer(wxVERTICAL);

      dw_dbg_bt_chk01                   =   new wxButton(aw_dbg_cp_wn, idb + 6, wxS("panel vs layout"));

    sz1                     ->Add( dw_dbg_bt_chk01, 1, wxGROW|wxALL, 0);
    aw_dbg_cp_wn            ->SetSizer(sz1);
    sz1                     ->SetSizeHints(aw_dbg_cp_wn);

    }
    //  ............................................................................................
    //  appearance
    {

    wxSizer             *sz1, *sz2;
    wxStaticBoxSizer    *sbs;

    dw_app_cp                           = new wxCollapsiblePane( this, wxID_ANY, wxS("Appearance"));
    aw_app_cp_wn                        = dw_app_cp->GetPane();
    aw_app_cp_wn->SetFont( oflp::Modules::Instance()->gfx()->fnt8() );

    sbs                                 =   new wxStaticBoxSizer(wxVERTICAL, aw_app_cp_wn, wxS("bg colors"));

      sz1                               =   new wxBoxSizer(wxHORIZONTAL);
      dw_app_st__color_bg_panels        =   new wxStaticText(aw_app_cp_wn, wxID_ANY, wxS("Panels"));
      dw_app_tc__color_bg_panels        =   new wxTextCtrl  (aw_app_cp_wn, wxID_ANY, _opt_colors.a_bg_p.GetAsString(wxC2S_HTML_SYNTAX).wc_str() );
      sz1                               ->Add( dw_app_st__color_bg_panels, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
      sz1                               ->Add( dw_app_tc__color_bg_panels, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);

      sz2                               =   new wxBoxSizer(wxHORIZONTAL);
      dw_app_st__color_bg_headers       =   new wxStaticText(aw_app_cp_wn, wxID_ANY, wxS("Headers"));
      dw_app_tc__color_bg_headers       =   new wxTextCtrl  (aw_app_cp_wn, wxID_ANY, _opt_colors.a_bg_h.GetAsString(wxC2S_HTML_SYNTAX).wc_str() );
      sz2                               ->Add( dw_app_st__color_bg_headers, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
      sz2                               ->Add( dw_app_tc__color_bg_headers, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);

    sbs ->Add(sz1, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    sbs ->Add(sz2, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    aw_app_cp_wn                        ->SetSizer(sbs);

    }
    //  ............................................................................................
    if ( oflp::Modules::Instance()->settings()->emb_menu__log() )
    {
    dw_bs_main              ->Add(dw_log_cp                 , 0, wxGROW|wxALL, 0);
    }

    dw_bs_main              ->Add(dw_sel_cp                 , 0, wxGROW|wxALL, 0);
    dw_bs_main              ->Add(dw_div_cp                 , 0, wxGROW|wxALL, 0);

    if ( oflp::Modules::Instance()->settings()->emb_menu__dbg() )
    {
    dw_bs_main              ->Add(dw_dbg_cp                 , 0, wxGROW|wxALL, 0);
    }

    dw_bs_main              ->Add(dw_app_cp                 , 0, wxGROW|wxALL, 0);
    //  ............................................................................................
    opt_log__sync_from_var(_opt_log);
    opt_sel__sync_from_var(_opt_sel);
    opt_div_tt__sync_from_var(_opt_div_tt);
    //opt_app_col__sync_from_internals(_opt_colors);                                                //  already done above

    //  ............................................................................................
    //  the check events are propagated to the miniframe, so we can connect with
    //  a range of ids, so only one Connect() call is needed in the code.
    this->Connect(
        idb + 0, idb + 5                                                        ,
        wxEVT_COMMAND_CHECKBOX_CLICKED                                          ,
        wxCommandEventHandler(OpenFilesListPlusSettings::evh_check_box_clicked) ,
        NULL, this                                                              );

    this->Bind(
        wxEVT_COMMAND_BUTTON_CLICKED                                                ,
        wxCommandEventHandler(OpenFilesListPlusSettings::evh_button_clicked_dbg01)  ,
        this                                                                        ,
        idb + 6, idb + 6                                                            ,
        nullptr                                                                     );
}

                OpenFilesListPlusSettings:: ~OpenFilesListPlusSettings()
{
    // dont delete child widgets else double free-s
}
//  ================================================================================================
//  constructor specialized stuff -> microsoft
//  ================================================================================================
#ifdef  OFLP_MS
                OpenFilesListPlusSettings:: OpenFilesListPlusSettings(
        wxWindow    *   _parent     ,
        wxPoint         _scr_pos    ,
        wxSize          _scr_size   ,
        OflpOptLog      _opt_log    ,
        OflpOptSel      _opt_sel    ,
        OflpOptColors   _opt_colors )
            :   wxFrame (   _parent             , wxNewId()     ,   wxT("title")    ,
                            wxDefaultPosition   , wxDefaultSize ,
                            (wxBORDER_NONE)                         )   ,
                a_parent    (_parent)
{
    //  --------------------------------------------------------------------------------------------
    //  child widgets, Options import, signals
    p0_ctor_01(_opt_log, _opt_sel, _opt_colors);
    //  ............................................................................................
    //  flags etc...
    p0_ctor_02(_scr_pos, _scr_size);
}

void            OpenFilesListPlusSettings:: z_build_widgets_02  (wxPoint& _scr_pos, wxSize& _scr_size)
{
    this->SetSizer(dw_bs_main);
    this->SetBackgroundColour( wxColour(200,200,200) );                         //  required under ms
    this->SetPosition( _scr_pos );                                              //  required under ms
    dw_bs_main->Fit(this);
    dw_bs_main->Layout();
}
#endif
//  ================================================================================================
//  constructor specialized stuff -> linux
//  ================================================================================================
#ifdef  OFLP_LX
//  ------------------------------------------------------------------------------------------------
//  constructor specialized stuff -> linux -> wx-3.0.?
//  ------------------------------------------------------------------------------------------------
#if ( wxMAJOR_VERSION == 3 ) && ( wxMINOR_VERSION == 0 )
                OpenFilesListPlusSettings:: OpenFilesListPlusSettings(
        wxWindow    *   _parent     ,
        wxPoint         _scr_pos    ,
        wxSize          _scr_size   ,
        OflpOptLog      _opt_log    ,
        OflpOptSel      _opt_sel    ,
        OflpOptDivTt    _opt_div_tt ,
        OflpOptColors   _opt_colors )
            :   wxFrame (   _parent             , wxNewId()     ,   wxT("title")    ,
                            wxDefaultPosition   , wxDefaultSize ,
                            (wxBORDER_NONE)                                         )   ,
                a_parent    (_parent)                                                   ,
                dw_log_cp   (nullptr)                                                   ,
                dw_sel_cp   (nullptr)                                                   ,
                dw_app_cp   (nullptr)                                                   ,
                dw_div_cp   (nullptr)                                                   ,
                dw_dbg_cp   (nullptr)
{

    this->SetFont(oflp::Modules::Instance()->gfx()->fnt8());
    //  --------------------------------------------------------------------------------------------
    //  child widgets, Options import, signals
    z_build_widgets_01(_opt_log, _opt_sel, _opt_div_tt, _opt_colors);
    //  ............................................................................................
    //  flags etc...
    z_build_widgets_02(_scr_pos, _scr_size);
}

void            OpenFilesListPlusSettings:: z_build_widgets_02   (wxPoint& _scr_pos, wxSize& _scr_size)
{
    this->SetSizer(dw_bs_main);
    this->SetPosition( _scr_pos );                                              //  required under ms, keep on linux ( why not )
    dw_bs_main->Fit(this);
    dw_bs_main->Layout();
}
#endif
#endif
