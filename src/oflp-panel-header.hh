/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PANEL_HEADER_HH__
#define __OFLP_PANEL_HEADER_HH__

/// ************************************************************************************************
//! \class  OpenFilesListPlusPanelHeader
//!
//! \brief  Header widget
//!
//! \detail Header owns title and buttons, but does not create buttons by
//!     itself.
/// ************************************************************************************************
class OpenFilesListPlusPanelHeader          : public wxPanel
{
    friend class    OpenFilesListPlus;
    friend class    OpenFilesListPlusPanel;
    friend class    OflpModPanels;

  private:
    WX_DEFINE_ARRAY(wxButton*, ButtonsArray);

  private:
    OflpPanel           *       aw_parent;                                      //  _GWR_WHYNOT_ use this->GetParent() instead

    oflp::HString               a_title;

    wxPanel             *       dw_panel;
    wxSizer             *       dw_sizer;
    wxWindow            *           aw_txt;
    wxWindow            *           dw_txt_old;
    wxStaticText        *           dw_txt_sta;
    wxTextCtrl          *           dw_txt_dyn;

    ButtonsArray                    a_buttons_array;

    size_t                      a_trid;                                                             // tooltip registrant id

  public:
    wxString    const   &           title()
        {
            return a_title.get();
        }

  private:
    bool                    z_title_ctrl_replace   (wxWindow* _wnew);
    //  --------------------------------------------------------------------------------------------
  public:
    void                    title_switch_to_dynamic   ();
    void                    title_switch_to_static    ();

    wxButton            *   button          (int _ix);
    void                    button_prepend  (int _i_bitmap_id, const char* _i_tooltip_text = nullptr);
    void                    button_append   (int _i_bitmap_id, const char* _i_tooltip_text = nullptr);
    void                    button_show     (int _ix, bool);
    //  --------------------------------------------------------------------------------------------
  private:
    void                    z_set_col_bg                    (wxColour& _c);
    //  --------------------------------------------------------------------------------------------
  public:
                OpenFilesListPlusPanelHeader(OflpPanel* _parent, wxString _title);
    virtual    ~OpenFilesListPlusPanelHeader();
};


#endif
