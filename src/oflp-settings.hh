/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_SETTINGS_HH__
#define __OFLP_SETTINGS_HH__
//  ............................................................................
#include    "oflp-common.hh"

#include    "oflp-plugin-module.hh"

#include    "oflp-settings-options.hh"
//  ............................................................................
#include    <wx/sizer.h>
#include    <wx/checkbox.h>
#include    <wx/minifram.h>
/// ****************************************************************************
//! \class  OpenFilesListPlusSettings
//!
//! \brief  Settings widget
/// ****************************************************************************
class OpenFilesListPlusSettings             : public wxFrame
{
  private:
    static  wxSize                      sButtonsSize;
            //  ................................................................
            wxWindow                *   a_parent;
            //  ................................................................
            //wsScrolledWindow      *   dw_scr_win;                             //  no need for this for instant but keep in mind
            wxBoxSizer              *   dw_bs_main;
            //  ................................................................
            wxStaticBoxSizer        *   dw_sb_log;
                wxCheckBox          *   dw_cb_log_enable;
                wxBoxSizer          *   dw_bs_log_wc1;
                    wxPanel         *   dw_pl_log_wc;
                    wxBoxSizer      *   dw_bs_log_wc2;
                        wxCheckBox  *   dw_cb_log_enable_window;
                        wxCheckBox  *   dw_cb_log_enable_console;
            //  ................................................................
            wxStaticBoxSizer        *   dw_sb_sel;
                    wxCheckBox      *   dw_cb_sel_double;
                    wxCheckBox      *   dw_cb_sel_single;
    //  ------------------------------------------------------------------------
  private:
    OflpOptLog      a_opt_log;                                                  //  internal variable
    OflpOptSel      a_opt_sel;                                                  //  internal variable

    void    import_log              (OflpOptLog&);
    void    export_log              ();
    void    sync_log                ();
    void    import_sel              (OflpOptSel&);
    void    export_sel_from_single  ();
    void    export_sel_from_double  ();
    void    sync_sel                ();

  public:
    void    output      (OflpOptLog&, OflpOptSel&);
    //  ------------------------------------------------------------------------
  private:
    void    evh_check_box_clicked(wxCommandEvent&);
    //  ------------------------------------------------------------------------
  public:
    wxRect      parent_screen_rect()
        {
            return a_parent->GetScreenRect();
        }
    //  ------------------------------------------------------------------------
  private:
    void        p0_ctor_01                  (OflpOptLog&, OflpOptSel&);

    void        p0_ctor_02                  (wxPoint& _scr_pos, wxSize& _scr_size);

  public:
                OpenFilesListPlusSettings   (wxWindow* _parent, wxPoint, wxSize, OflpOptLog, OflpOptSel);
    virtual    ~OpenFilesListPlusSettings   ();


};

#endif                                                                          //  __OFLP_SETTINGS_HH__
