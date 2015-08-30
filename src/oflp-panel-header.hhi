/// ****************************************************************************
//! \class  OpenFilesListPlusPanelHeader
//!
//! \brief  Header widget
//!
//! \detail Header owns title and buttons, but does not create buttons by
//!     itself.
/// ****************************************************************************
class OpenFilesListPlusPanelHeader        : public wxPanel
{
    friend class OpenFilesListPlusPanel;

  private:
    WX_DEFINE_ARRAY(wxButton*, ButtonsArray);

  private:
    OFLPPanel           *       aw_parent;                                      //  _GWR_TODO_ can use this->GetParent() instead

    wxString                    a_title;

    wxPanel             *       dw_panel;
    wxSizer             *       dw_sizer;
    wxWindow            *           aw_txt;
    wxWindow            *           dw_txt_old;
    wxStaticText        *           dw_txt_sta;
    wxTextCtrl          *           dw_txt_dyn;

    ButtonsArray                    a_buttons_array;

  public:
    wxString    const   &           get_title()                 const   { return a_title; }

  private:
    void                    p0_create_buttons       ();
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
    OpenFilesListPlusPanelHeader();
    OpenFilesListPlusPanelHeader(OFLPPanel* _parent, wxString _title);
    virtual ~OpenFilesListPlusPanelHeader();
};


