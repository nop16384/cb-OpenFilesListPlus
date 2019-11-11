/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_LOG_FRAME_HH__
#define __OFLP_LOG_FRAME_HH__

#include    "oflp-common.hh"
#include    "oflp-log.hh"

#include    <wx/frame.h>
#include    <wx/font.h>

#include    <wx/textctrl.h>
#include    <wx/sizer.h>

namespace   oflp
{

/// ************************************************************************************************
//! \class  Spacer
//!
//! \brief  For log indentations.
/// ************************************************************************************************
class Spacer
{
    private:
    wxString            a_spaces;

    public:
    const   wxChar  *   wcstr();
    void                inc();
    void                dec();

    public:
    Spacer()
    {
    }
    ~Spacer()
    {
    }
};
/// ************************************************************************************************
//! \class  LogFrame
//!
//! \brief  Window for outputing logs into.
//!
//! \detail Because using wxLogWindow involves using wxLog, and I get other C::B
//!         logs in it.
/// ************************************************************************************************
class   LogFrame : public wxFrame
{
  private:
    wxSizer         *   dw_log_sizer;
    wxTextCtrl      *   dw_log_txt_ctrl;
    wxTextAttr          a_text_attr;

  public:
            LogFrame(wxWindow* _parent);
            ~LogFrame();

  private:
    void    print_header();

    void    evh_close_window(wxCloseEvent& _e);
    void    evh_key_up      (wxKeyEvent& _e);

  public:
    void    log             (wxString& _wxs);
    void    set_text_col_fg (const wxColour& _wx_col);
    void    set_text_attr   (const wxTextAttr& _wx_att);
};

}

#endif                                                                                              //  __OFLP_LOG_FRAME_HH__



