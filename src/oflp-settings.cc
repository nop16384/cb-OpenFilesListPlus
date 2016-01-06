/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-settings.hh"
//  ............................................................................
//  specific to this file oflp defines
#define GWR_OFLP_SANITY_CHECKS
#define GWR_LOG(FORMAT, ...)    GWRCB_LOG(FORMAT, __VA_ARGS__);
#define GWR_TKI(FORMAT, ...)    GWRCB_TKI(FORMAT, __VA_ARGS__);
#define GWR_TKE(FORMAT, ...)    GWRCB_TKE(FORMAT, __VA_ARGS__);
#define GWR_INF(FORMAT, ...)    GWRCB_INF(FORMAT, __VA_ARGS__);
#define GWR_WNG(FORMAT, ...)    GWRCB_WNG(FORMAT, __VA_ARGS__);
#define GWR_ERR(FORMAT, ...)    GWRCB_ERR(FORMAT, __VA_ARGS__);
//  ############################################################################
//
//                          OPENFILESLISTPLUSSETTINGS
//
//  ############################################################################
void            OpenFilesListPlusSettings:: import_log  (OflpOptLog & _o)
{
    a_opt_log   =   _o;
    sync_log();
}
void            OpenFilesListPlusSettings:: export_log  ()
{
    a_opt_log.enabled   =   dw_cb_log_enable->GetValue();
    a_opt_log.window    =   dw_cb_log_enable_window->GetValue();
    a_opt_log.console   =   dw_cb_log_enable_console->GetValue();
}
void            OpenFilesListPlusSettings:: sync_log    ()
{
    dw_cb_log_enable            ->SetValue(a_opt_log.enabled);
    dw_cb_log_enable_window     ->SetValue(a_opt_log.window);
    dw_cb_log_enable_console    ->SetValue(a_opt_log.console);

    if ( dw_cb_log_enable->GetValue() )
    {
        dw_cb_log_enable_window     ->Enable(true);
        dw_cb_log_enable_console    ->Enable(true);
    }
    else
    {
        dw_cb_log_enable_window     ->Enable(false);
        dw_cb_log_enable_console    ->Enable(false);
    }
}

void            OpenFilesListPlusSettings:: import_sel              (OflpOptSel & _o)
{
    a_opt_sel   =   _o;
    sync_sel();
}
void            OpenFilesListPlusSettings:: export_sel_from_single  ()
{
    a_opt_sel.set_sclick( dw_cb_sel_single->GetValue() );
}
void            OpenFilesListPlusSettings:: export_sel_from_double  ()
{
    a_opt_sel.set_dclick( dw_cb_sel_double->GetValue() );
}
void            OpenFilesListPlusSettings:: sync_sel                ()
{
    dw_cb_sel_single->SetValue(a_opt_sel.sclick());
    dw_cb_sel_double->SetValue(a_opt_sel.dclick());
}

void            OpenFilesListPlusSettings:: output      (OflpOptLog& _olog, OflpOptSel& _osel)
{
    export_log();
    export_sel_from_single();                                                   //  or _from_double(), whatever

    _olog   =   a_opt_log;
    _osel   =   a_opt_sel;
}
//  ============================================================================
void            OpenFilesListPlusSettings:: evh_check_box_clicked(wxCommandEvent & _e)
{
    //GWR_INF("ClientData[%p] ClientObject[%p] EventObject[%p]", _e.GetClientData(), _e.GetClientObject(), _e.GetEventObject());


    if  (   ( _e.GetEventObject() == dw_cb_log_enable           )   ||
            ( _e.GetEventObject() == dw_cb_log_enable_window    )   ||
            ( _e.GetEventObject() == dw_cb_log_enable_console   )   )
    {
        export_log();
        import_log(a_opt_log);
    }

    if  (   ( _e.GetEventObject() == dw_cb_sel_single   )   )
    {
        export_sel_from_single();
        import_sel(a_opt_sel);
    }

    if  (   ( _e.GetEventObject() == dw_cb_sel_double   )   )
    {
        export_sel_from_double();
        import_sel(a_opt_sel);
    }
}
//  ============================================================================
                OpenFilesListPlusSettings:: OpenFilesListPlusSettings(
        wxWindow    *   _parent     ,
        wxPoint         _scr_pos    ,
        wxSize          _scr_size   ,
        OflpOptLog      _opt_log    ,
        OflpOptSel      _opt_sel    )
            :   wxFrame (   _parent     , wxNewId() ,   wxT("title")        ,
                            _scr_pos    , _scr_size                         ,
                            ~( wxCAPTION)                               )   ,
                a_parent    (_parent)
{
    //  ........................................................................
    //  reserve ids for checkboxes
    wxWindowID idb                      =   wxID_HIGHEST + 1;
    //  ........................................................................
    dw_bs_main                          =   new wxBoxSizer( wxVERTICAL );
    //  ........................................................................
    //  log options
    dw_sb_log                           =   new wxStaticBoxSizer( wxVERTICAL, this, wxS(" Logs ") );
        dw_cb_log_enable                =   new wxCheckBox(this, idb + 0, wxS("Enable logs"), wxDefaultPosition, wxDefaultSize, 0);
        dw_bs_log_wc1                   =   new wxBoxSizer( wxHORIZONTAL );
            dw_pl_log_wc                =   new wxPanel(this, wxNewId());
            dw_bs_log_wc2               =   new wxBoxSizer( wxVERTICAL );
                dw_cb_log_enable_window =   new wxCheckBox(this, idb + 1, wxS("window"));
                dw_cb_log_enable_console=   new wxCheckBox(this, idb + 2, wxS("console"));

            dw_bs_log_wc2   ->Add(dw_cb_log_enable_window   , 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
            dw_bs_log_wc2   ->Add(dw_cb_log_enable_console  , 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);

        dw_bs_log_wc1       ->Add(dw_pl_log_wc              , 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
        dw_bs_log_wc1       ->Add(dw_bs_log_wc2             , 3, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);

    dw_sb_log               ->Add(dw_cb_log_enable          , 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    dw_sb_log               ->Add(dw_bs_log_wc1             , 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    //  ........................................................................
    //  file selection mode option
    dw_sb_sel                           =   new wxStaticBoxSizer( wxVERTICAL, this, wxS(" Selection ") );
            dw_cb_sel_double            =   new wxCheckBox(this, idb + 3, wxS("double click"));
            dw_cb_sel_single            =   new wxCheckBox(this, idb + 4, wxS("simple click"));

    dw_sb_sel               ->Add(dw_cb_sel_double          , 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    dw_sb_sel               ->Add(dw_cb_sel_single          , 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
    //  ........................................................................
    dw_bs_main              ->Add(dw_sb_log                 , 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    dw_bs_main              ->Add(dw_sb_sel                 , 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    //  if no logs, just hide the wxStaticBoxSizer here instead of not creating
    //  it, which would lead to spread #ifndef OFLP_LOG checks everywhere
    #ifndef OFLP_LOG
    dw_bs_main->Show((size_t)0, false);
    #endif
    //  ........................................................................
    import_log(_opt_log);
    import_sel(_opt_sel);
    //  ........................................................................
    //  the check events are propagated to the miniframe, so we can connect with
    //  a range of ids, so only one Connect() call is needed in the code.
    this->Connect(
        idb + 0, idb + 4,
        wxEVT_COMMAND_CHECKBOX_CLICKED                                          ,
        wxCommandEventHandler(OpenFilesListPlusSettings::evh_check_box_clicked) ,
        NULL, this                                                              );
    //  ........................................................................
    this->SetSizer(dw_bs_main);
    dw_bs_main->Fit(this);
    dw_bs_main->SetSizeHints(this);
}

                OpenFilesListPlusSettings:: ~OpenFilesListPlusSettings()
{
    // dont delete child widgets else double free-s
}
