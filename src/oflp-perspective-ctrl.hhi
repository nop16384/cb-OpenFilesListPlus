/// ****************************************************************************
//! \class  OpenFilesListPlusPerspectiveCtrl
//!
//! \brief  Widget for OFLP perspectives management
//!
//! \detail
/// ****************************************************************************
class OpenFilesListPlusPerspectiveCtrl  : public wxPanel
{
  private:
    //OFLPPanel           *       aw_parent;                                      //  _GWR_TODO_ can use this->GetParent() instead

    //earlgreycb::HString         a_title;

    wxComboBox          *   dw_combo_box;
    wxSizer             *   dw_sizer;
    wxStaticText        *   dw_current_perspective_name;
    wxButton            *   dw_button_perspective_menu;

    wxMenu              *   dw_menu_main;
    wxMenuItem          *       dw_item_sync_editors_to_perspective;
    wxMenuItem          *       dw_item_separator_01;
    //wxMenuItem        *       perspective items...


  public:

  private:
    //  ------------------------------------------------------------------------
  public:

    //  ------------------------------------------------------------------------
  public:
             OpenFilesListPlusPerspectiveCtrl(wxWindow* _parent);
    virtual ~OpenFilesListPlusPerspectiveCtrl();
};


