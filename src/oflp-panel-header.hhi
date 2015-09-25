/// ****************************************************************************
//! \class  OpenFilesListPlusPanelHeader
//!
//! \brief  wxPanel containing many wxWindow-s in a wxBoxSizer(wxHORIZONTAL)
//!
//! \detail
/// ****************************************************************************
class OpenFilesListPlusPanelHeader          : public wxPanel
{
  private:
    WX_DEFINE_ARRAY( wxWindow   *,  wxWindowArray);
    //  ------------------------------------------------------------------------
  private:
    OFLPPanel                       *   aw_oflp_panel;

  public:
    OFLPPanel                       *   oflp_panel              ()  { return aw_oflp_panel; }
    //  ------------------------------------------------------------------------
  private:
    wxSizer                         *   dw_sizer;
    //  ------------------------------------------------------------------------
  private:
    OFLPPanelHeader::wxWindowArray      a_wx_window_array;

  public:
    int                                 add     (wxWindow* _wx_win)
        {
            a_wx_window_array.Add(_wx_win);

            dw_sizer->Add(_wx_win, 1, wxEXPAND, 0);
            dw_sizer->Layout();

            return a_wx_window_array.Index(_wx_win);
        }
    wxWindow                        *   window  (int _ix)
        {
            size_t  ix  =   static_cast< size_t >( _ix );

            return a_wx_window_array.Item( ix );
        }
    //  ------------------------------------------------------------------------
  public:
                OpenFilesListPlusPanelHeader(wxWindow* _parent);
    virtual    ~OpenFilesListPlusPanelHeader();
};
/// ****************************************************************************
//! \class  OpenFilesListPlusPanelHeader
//!
//! \brief  Header widget for OFLPPanel
//!
//! \detail Some buttons and a title eventually modifiable
/// ****************************************************************************
class OpenFilesListPlusPanelHeaderSimple    :   public wxPanel
{
    friend class OpenFilesListPlusPanel;
    friend class OpenFilesListPlusPanelBulk;

  private:
    WX_DEFINE_ARRAY(wxButton*, ButtonsArray);
    //  ------------------------------------------------------------------------
  private:
    wxPanel             *   dw_panel;
    wxSizer             *   dw_sizer;
    wxWindow            *       aw_txt;
    wxWindow            *       dw_txt_old;
    wxStaticText        *       dw_txt_sta;
    wxTextCtrl          *       dw_txt_dyn;
    //  ------------------------------------------------------------------------
  private:
    OFLPPanel           *   aw_oflp_panel;

  public:
    OFLPPanel           *   oflp_panel              ()  { return aw_oflp_panel; }
    //  ------------------------------------------------------------------------
  private:
    earlgreycb::HString     a_title;

  private:
    void                    title_switch_to_dynamic ();
    void                    title_switch_to_static  ();
    bool                    p0_title_ctrl_replace   (wxWindow* _wnew);

  public:
    wxString    const   &   title                   ()                  const   { return a_title.get(); }
    //  ------------------------------------------------------------------------
  private:
    ButtonsArray            a_buttons_array;

    wxButton            *   button                  (int _ix);
    void                    button_prepend          (int _bitmap_id);
    void                    button_append           (int _bitmap_id);
    void                    button_show             (int _ix, bool);
    //  ------------------------------------------------------------------------
  public:
             OpenFilesListPlusPanelHeaderSimple(
                wxWindow    *   _parent                 ,
                OFLPPanel   *   _oflp_panel             ,
                wxString        _title                  ,
                bool            _title_modifiable = TRUE);
    virtual ~OpenFilesListPlusPanelHeaderSimple();
};
