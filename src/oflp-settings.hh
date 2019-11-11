/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_SETTINGS_HH__
#define __OFLP_SETTINGS_HH__
//  ................................................................................................
#include    "oflp-plugin-module.hh"

#include    "oflp-settings-options.hh"
//  ................................................................................................
#include    <wx/sizer.h>
#include    <wx/checkbox.h>
#include    <wx/minifram.h>
#include    <wx/collpane.h>
/// ************************************************************************************************
//! \class  OpenFilesListPlusSettings
//!
//! \brief  Settings widget
/// ************************************************************************************************
class OpenFilesListPlusSettings             : public wxFrame
{
  private:
    static  wxSize                      sButtonsSize;
            //  ....................................................................................
            wxWindow                *   a_parent;
            //  ....................................................................................
            //wsScrolledWindow      *   dw_scr_win;                             //  no need for this for instant but keep in mind
            wxBoxSizer              *   dw_bs_main;
            //  ....................................................................................
            //  logs
            wxCollapsiblePane       *   dw_log_cp;
            wxWindow                *   aw_log_cp_wn;
              wxCheckBox            *   dw_log_cb_enable;
                wxCheckBox          *   dw_log_cb_ew;
                wxCheckBox          *   dw_log_cb_ec;
            //  ....................................................................................
            //  selection mode
            wxCollapsiblePane       *   dw_sel_cp;
            wxWindow                *   aw_sel_pn1;
              wxSizer               *   dw_sel_sz1;
                wxCheckBox          *   dw_sel_cb_double;
                wxCheckBox          *   dw_sel_cb_single;
            //  ....................................................................................
            //  appearance
            wxCollapsiblePane       *   dw_app_cp;
            wxWindow                *   aw_app_cp_wn;
              wxStaticText          *   dw_app_st__color_bg_panels;
              wxTextCtrl            *   dw_app_tc__color_bg_panels;
              wxStaticText          *   dw_app_st__color_bg_headers;
              wxTextCtrl            *   dw_app_tc__color_bg_headers;
            //  ....................................................................................
            //  divers
            wxCollapsiblePane       *   dw_div_cp;
            wxWindow                *   aw_div_cp_wn;
              wxCheckBox            *   dw_cb_div_enable_tooltips;
            //  ....................................................................................
            //  debug
            wxCollapsiblePane       *   dw_dbg_cp;
            wxWindow                *   aw_dbg_cp_wn;
              wxButton              *   dw_dbg_bt_chk01;
    //  --------------------------------------------------------------------------------------------
  private:
    OflpOptLog          a_opt_log;
    OflpOptSel          a_opt_sel;
    OflpOptDivTt        a_opt_div_tt;
    OflpOptColors       a_opt_colors;

    void    opt_log__sync_widgets_activity  ();
    void    opt_log__sync_from_widgets      ();
    void    opt_log__sync_from_internals    ();
    void    opt_log__sync_from_var          (OflpOptLog&);

    void    opt_sel__sync_from_widgets      ();
    void    opt_sel__sync_from_internals    ();
    void    opt_sel__sync_from_var          (OflpOptSel&);

    void    opt_div_tt__sync_from_widgets   ();
    void    opt_div_tt__sync_from_internals ();
    void    opt_div_tt__sync_from_var       (OflpOptDivTt&);

    void    opt_app_col__sync_from_widgets  ();
    void    opt_app_col__sync_from_internals();
    void    opt_app_col__sync_from_var      (OflpOptColors&);

  public:
    void    xport                           (OflpOptLog&, OflpOptSel&, OflpOptDivTt&, OflpOptColors&);
    //  --------------------------------------------------------------------------------------------
  private:
    void    evh_check_box_clicked       (wxCommandEvent&);
    void    evh_button_clicked_dbg01    (wxCommandEvent&);
    //  --------------------------------------------------------------------------------------------
  public:
    wxRect      parent_screen_rect()
        {
            return a_parent->GetScreenRect();
        }
    //  --------------------------------------------------------------------------------------------
  private:
    void        z_build_widgets_01          (OflpOptLog&, OflpOptSel&, OflpOptDivTt&, OflpOptColors&);

    void        z_build_widgets_02          (wxPoint& _scr_pos, wxSize& _scr_size);

  public:
                OpenFilesListPlusSettings   (wxWindow* _parent, wxPoint, wxSize, OflpOptLog, OflpOptSel, OflpOptDivTt, OflpOptColors);
    virtual    ~OpenFilesListPlusSettings   ();


};

#endif                                                                                              //  __OFLP_SETTINGS_HH__
