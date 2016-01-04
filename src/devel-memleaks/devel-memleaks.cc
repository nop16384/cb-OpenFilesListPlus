#include    "sdk.h"
//  ============================================================================
        Manager         *   s_manager           =   NULL;
static  ProjectManager  *   s_project_manager   =   NULL;
static  EditorManager   *   s_editor_manager    =   NULL;
static  wxWindow        *   s_app_window        =   NULL;
//  ============================================================================
void    NotImplemented(wxString _wx_str)
{
}
void    NotifyMissingFile(wxString _wx_str)
{
}
//  ============================================================================
// Return @c str as a proper unicode-compatible string
wxString            cbC2U(const char* str)
{
#if wxUSE_UNICODE
    return wxString(str, wxConvUTF8);
#else
    return wxString(str);
#endif
}
// Return multibyte (C string) representation of the string
const wxWX2MBbuf    cbU2C(const wxString& str)
{
    if (platform::unicode)
        return str.mb_str(wxConvUTF8);
    else
        return str.mb_str();
}
//  ############################################################################
//
//                              Manager
//
//  ############################################################################
ProjectManager  *   Manager::GetProjectManager()                        const
{
    return  s_project_manager;
}
EditorManager   *   Manager::GetEditorManager()                         const
{
    return  s_editor_manager;
}
wxWindow        *   Manager::GetAppWindow()
{
    return  s_app_window;
}
//  ============================================================================
//EditorManager::
//  ============================================================================
int main(int argc, char** argv)
{
    return 0;
}
