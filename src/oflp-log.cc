/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-log.hh"
//  ****************************************************************************
//  logs enabled
//  ****************************************************************************
#ifdef  OFLP_LOG

#include    <wx/frame.h>
#include    <wx/font.h>

#include    <wx/textctrl.h>
#include    <wx/sizer.h>

namespace   oflp
{

/// ****************************************************************************
//! \class  Spacer
//!
//! \brief  For log indentations.
/// ****************************************************************************
class Spacer
{
    private:
    wxString            a_spaces;

    public:
    const   wxChar  *   wcstr() { return a_spaces.wc_str();  }
    void                inc()   { a_spaces.append( wxString::FromUTF8("    ")); }
    void                dec()
        {
            if ( ! a_spaces.length() )
            {
                return;
            }
            a_spaces = a_spaces.Right( a_spaces.length() - 4 );
        }

    public:
    Spacer()
    {
    }
    ~Spacer()
    {
    }
};
/// ****************************************************************************
//! \class  LogFrame
//!
//! \brief  Window for outputing logs into.
//!
//! \detail Because using wxLogWindow involves using wxLog, and I get other C::B
//!         logs in it.
/// ****************************************************************************
class   LogFrame : public wxFrame
{
  private:
    wxSizer         *   dw_log_sizer;
    wxTextCtrl      *   dw_log_txt_ctrl;
    wxTextAttr          a_text_attr;

  public:
            LogFrame(wxWindow* _parent)
                :   wxFrame(_parent, wxNewId(), wxString::FromUTF8("OpenFilesListPlus"), wxDefaultPosition, wxSize(900,800))
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
  private:
    void    print_header()
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

    void    evh_close_window(wxCloseEvent& _e)
        {
            //  avoid too complicated synchronization between LogFrame and
            //  OFLP plugin menu : LogFrame closing has to be done in the
            //  plugin menu, and thats all
            _e.Veto();
        }

    void    evh_key_up(wxKeyEvent& _e)
        {
            if ( _e.GetModifiers() & wxMOD_CONTROL )
                if ( _e.GetKeyCode() == 'E' )
                {
                    dw_log_txt_ctrl->Clear();
                    print_header();
                }

        }

  public:
    void    log(wxString& _wxs)
        {
            dw_log_txt_ctrl->AppendText( _wxs + wxString::FromUTF8("\n") );
        }

    //wxTextAttr(const wxColour& colText, const wxColour& colBack = wxNullColour, const wxFont& font = wxNullFont, wxTextAttrAlignment alignment = wxTEXT_ALIGNMENT_DEFAULT)

    void    set_text_col_fg(const wxColour& _wx_col)
        {
            dw_log_txt_ctrl->SetDefaultStyle( wxTextAttr(_wx_col) );
        }
};
//  ############################################################################
wxColour    A_colour_log_inf    (  38 , 167 ,  38 );
wxColour    A_colour_log_wng    ( 197 , 126 ,   0 );
wxColour    A_colour_log_err    ( 255 ,   0 ,   0 );
wxColour    A_colour_log_tki    ( 167 , 167 , 167 );
wxColour    A_colour_log_tkw    ( 197 , 126 ,   0 );
wxColour    A_colour_log_tke    ( 255 ,   0 ,   0 );

Spacer              a_spacer;
LogFrame        *   dw_log_frame    =   NULL;
//  ............................................................................
bool                A_log_console   =   false;
bool                A_log_window    =   false;

const wxChar*
        Log_spc_wxc()
{
    return a_spacer.wcstr();
}

void    Log_spc_inc()
{
    a_spacer.inc();
}

void    Log_spc_dec()
{
    a_spacer.dec();
}

void    Log_function_mark   (const wxChar* _funcname)
{
    GWRCB_TCS("%s%s%s", wxS("----------  "), _funcname, wxS("  ---------- ") );
}

void    Log_function_enter  (const wxChar* _funcname)
{
    oflp::Log_spc_inc();

    GWRCB_TCS("%s", wxS("***********************************************************")  );
    GWRCB_TCS("%s",  _funcname);
    GWRCB_TCS("%s", wxS("***********************************************************")  );
}

void    Log_function_exit   ()
{
    oflp::Log_spc_dec();
}

int     Log_szt2int         (size_t _szt)
{
    return static_cast< int >( _szt );
}
//  ............................................................................
void    Log_window_open     (wxWindow* _parent)
{
    if ( dw_log_frame )
    {
        return;
    }

    dw_log_frame    =   new LogFrame( _parent );
    dw_log_frame->Show();
}

void    Log_window_close    ()
{
    if ( ! dw_log_frame )
    {
        return;
    }

    delete dw_log_frame;
    dw_log_frame = NULL;
}
//  ............................................................................
void    Log_console                 (wxString& _wxs)
{
    printf("%s\n", _wxs.mb_str(wxConvUTF8).data());
}

void    Log_window                  (wxString& _wxs)
{
    if ( dw_log_frame )
    {
        dw_log_frame->log( _wxs );
    }
}

void    Log_window_set_text_col_fg  (wxColour& _wx_col)
{
    if ( dw_log_frame )
    {
        dw_log_frame->set_text_col_fg(_wx_col);
    }
}

}

#endif                                                                          //  #ifdef OFLP_LOG
//  ****************************************************************************
//  logs disabled
//  ****************************************************************************
#ifndef OFLP_LOG

//  instead of just defining 'class wxWindow', we have to #include wxWidgets stuff
//  else the Log_window_open(wxWindow*) wont be included in the shared library
#include    <wx/window.h>

namespace oflp
{

bool                A_log_console   =   false;
bool                A_log_window    =   false;

void                Log_window_open             (wxWindow*) {}
void                Log_window_close            ()          {}

}

#endif



