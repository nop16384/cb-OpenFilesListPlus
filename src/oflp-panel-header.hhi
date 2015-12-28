/*
/// ****************************************************************************
//! \class  OpenFilesListPlusHeader
//!
//! \brief  Basic header widget with a title and buttons
//!
//! \detail Header owns title and buttons, but does not create buttons by
//!         itself.
/// ****************************************************************************
class OpenFilesListPlusHeader               : public wxPanel
{
  protected:
    WX_DEFINE_ARRAY(wxButton*, ButtonsArray);

  protected:
    earlgreycb::HString         a_title;

  public:
    wxString    const   &       title()
        {
            return a_title.get();
        }

  protected:
    static  wxSize              sButtonsSize;
            wxPanel     *       dw_panel;
            wxSizer     *       dw_sizer;
            wxStaticText*       dw_txt_sta;

  protected:
    ButtonsArray                a_buttons_array;

  public:
    wxButton            *       button          (int _ix);
    void                        button_prepend  (int _bitmap_id , void* _client_data);
    void                        button_append   (int _bitmap_id , void* _client_data);
    void                        button_show     (int _ix        , bool);
    //  ------------------------------------------------------------------------
  public:
                OpenFilesListPlusHeader(wxWindow* _parent, wxString _title);
    virtual    ~OpenFilesListPlusHeader();
};
*/
/// ****************************************************************************
//! \class  OpenFilesListPlusPanelHeader
//!
//! \brief  Header widget
//!
//! \detail Header owns title and buttons, but does not create buttons by
//!     itself.
/// ****************************************************************************
class OpenFilesListPlusPanelHeader          : public wxPanel
{
    friend class OpenFilesListPlusPanel;

  private:
    WX_DEFINE_ARRAY(wxButton*, ButtonsArray);

  private:
    OFLPPanel           *       aw_parent;                                      //  _GWR_WHYNOT_ use this->GetParent() instead

    earlgreycb::HString         a_title;

    wxPanel             *       dw_panel;
    wxSizer             *       dw_sizer;
    wxWindow            *           aw_txt;
    wxWindow            *           dw_txt_old;
    wxStaticText        *           dw_txt_sta;
    wxTextCtrl          *           dw_txt_dyn;

    ButtonsArray                    a_buttons_array;

  public:
    wxString    const   &           title()
        {
            return a_title.get();
        }

  private:
    bool                    p0_title_ctrl_replace   (wxWindow* _wnew);
    //  ------------------------------------------------------------------------
  public:
    void                    title_switch_to_dynamic   ();
    void                    title_switch_to_static    ();

    wxButton            *   button          (int _ix);
    void                    button_prepend  (int _bitmap_id);
    void                    button_append   (int _bitmap_id);
    void                    button_show     (int _ix, bool);

    //  ------------------------------------------------------------------------
  public:
                OpenFilesListPlusPanelHeader(OFLPPanel* _parent, wxString _title);
    virtual    ~OpenFilesListPlusPanelHeader();
};


