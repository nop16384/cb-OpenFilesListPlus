/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    <wx/frame.h>
#include    <wx/font.h>

#include    <wx/textctrl.h>
#include    <wx/sizer.h>

#include    "oflp-common.hh"
#include    "oflp-log-frame.hh"

#ifdef  OFLP_EMBED__LOG

namespace   oflp
{



//  ================================================================================================
//  Spacer
//  ================================================================================================
wxChar  const   *   Spacer::wcstr()
{
    return a_spaces.wc_str();
}
void                Spacer::inc()
{
    a_spaces.append( wxString::FromUTF8("    "));
}
void                Spacer::dec()
{
    if ( ! a_spaces.length() )
    {
        return;
    }
    a_spaces = a_spaces.Right( a_spaces.length() - 4 );
}
//  ================================================================================================
//  Log Frame
//  ================================================================================================
LogFrame::LogFrame(wxWindow* _parent)
    :   wxFrame(_parent, wxNewId(), wxString::FromUTF8("OpenFilesListPlus logs"), wxDefaultPosition, wxSize(1200,800))
{
    dw_log_txt_ctrl     =   new wxTextCtrl(
        this            , wxNewId()                             ,
        wxEmptyString   , wxDefaultPosition , wxDefaultSize     ,
        wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL | wxTE_RICH  );

    //dw_log_txt_ctrl->SetWindowStyle( wxTE_MULTILINE );        //  GWR_TECH_ causes crash
    wxFont fnt(8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxString::FromUTF8("monospace") );
    dw_log_txt_ctrl->SetFont(fnt);

    dw_log_txt_ctrl->SetForegroundColour( wxColour(255,255,255) );
    dw_log_txt_ctrl->SetBackgroundColour( wxColour(0,0,0) );

    dw_log_sizer        =   new wxBoxSizer(wxVERTICAL);
    dw_log_sizer->Add( dw_log_txt_ctrl, 1, wxEXPAND, 0);
    this->SetSizer( dw_log_sizer );

    print_header();

    dw_log_txt_ctrl->Connect( wxEVT_KEY_UP, wxKeyEventHandler(LogFrame::evh_key_up), NULL, this);

    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler(LogFrame::evh_close_window), NULL, this);
}
LogFrame::~LogFrame()
{
    //  plugin c++ object may be already invalid
    //OpenFilesListPlus::Instance()->flog()->x_log("LogFrame::~LogFrame()\n");
}

void                        LogFrame::print_header()
{
    wxString    wxstr;

    wxstr.assign(wxS("----------------------------------------"));
    log(wxstr);

    wxstr.assign(wxS("Close window:use OpenFilesListPlus menu"));
    log(wxstr);
    wxstr.assign(wxS("Clear text  :Ctrl-E"));
    log(wxstr);

    wxstr.assign(wxS("----------------------------------------"));
    log(wxstr);
}

void                        LogFrame::evh_close_window(wxCloseEvent& _e)
{
    //  avoid too complicated synchronization between LogFrame and
    //  OFLP plugin menu : LogFrame closing has to be done in the
    //  plugin menu, and thats all
    _e.Veto();
}

void                        LogFrame::evh_key_up(wxKeyEvent& _e)
{
    if ( _e.GetModifiers() & wxMOD_CONTROL )
        if ( _e.GetKeyCode() == 'E' )
        {
            dw_log_txt_ctrl->Clear();
            print_header();
        }
}

void                        LogFrame::log(wxString& _wxs)
{
    dw_log_txt_ctrl->AppendText( _wxs + wxString::FromUTF8("\n") );
}

void                        LogFrame::set_text_col_fg(const wxColour& _wx_col)
{
    dw_log_txt_ctrl->SetDefaultStyle( wxTextAttr(_wx_col) );
}

void                        LogFrame::set_text_attr(const wxTextAttr& _wx_att)
{
    dw_log_txt_ctrl->SetDefaultStyle( _wx_att );
}



}

#endif                                                                                              //  OFLP_EMBED__LOG
