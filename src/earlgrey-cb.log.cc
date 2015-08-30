#include    "earlgrey-cb.log.hh"
#include    <wx/window.h>
#include    <wx/frame.h>
#include    <wx/font.h>
namespace   earlgreycb
{

class   LogWindow : public wxLogWindow
{
  public:
    virtual bool OnFrameClose 	(wxFrame* _frame)
        {
            return false;
        }

  public:
    LogWindow(wxWindow* _parent, const wxString& _title, bool _show=true, bool _passToOld=true)
        :   wxLogWindow(_parent, _title, _show, _passToOld)
        {
        }
};
//  ............................................................................
bool                A_log_console   =   false;
bool                A_log_window    =   false;

LogWindow       *   dw_log_window   =   NULL;
//  ............................................................................
void    Log_window_show(wxWindow* _parent)
{
    wxFrame     *   f   =   NULL;
    //  ........................................................................
    if ( dw_log_window )
    {
        return;
    }

    wxFont fnt(8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxString::FromUTF8("monospace") );
    wxLog::EnableLogging(true);

    dw_log_window   = new LogWindow( _parent, _T(" OpenFilesList"), true, false );
    f               = dw_log_window->GetFrame();
    f->SetSize(20,30,600,300);

    wxWindow        *   www = NULL;
    wxWindowList        wl  = f->GetChildren();
    for ( wxWindowList::iterator it = wl.begin() ; it != wl.end() ; it++ )
    {
        www=*it;
        www->SetFont(fnt);
    }
}

void    Log_window_hide()
{
    if ( ! dw_log_window )
    {
        return;
    }

    delete dw_log_window;
    dw_log_window = NULL;
}
//  ............................................................................
void        Log_console (wxString& _wxs)
{
    printf("%s\n", _wxs.mb_str(wxConvUTF8).data());

}

void        Log_window  (wxString& _wxs)
{
    if ( dw_log_window )
    {

        wxLog   *   old = wxLog::GetActiveTarget();
        wxLog::SetActiveTarget(dw_log_window);
        wxLogMessage(_wxs);
        wxLog::SetActiveTarget(old);
    }
}

}




