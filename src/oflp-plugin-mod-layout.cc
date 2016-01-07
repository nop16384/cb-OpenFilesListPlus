/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin-mod-layout.hh"

#include    "oflp-plugin.hh"

#include    "oflp-plugin-mod-panels.hh"

#include    "oflp-panel.hh"
//  ............................................................................
//  specific to this file oflp defines
#define GWR_OFLP_SANITY_CHECKS
#define GWR_LOG(FORMAT, ...)    GWRCB_LOG(FORMAT, __VA_ARGS__);
#define GWR_TKI(FORMAT, ...)    GWRCB_TKI(FORMAT, __VA_ARGS__);
#define GWR_TKW(FORMAT, ...)    GWRCB_TKW(FORMAT, __VA_ARGS__);
#define GWR_TKE(FORMAT, ...)    GWRCB_TKE(FORMAT, __VA_ARGS__);
#define GWR_INF(FORMAT, ...)    GWRCB_INF(FORMAT, __VA_ARGS__);
#define GWR_WNG(FORMAT, ...)    GWRCB_WNG(FORMAT, __VA_ARGS__);
#define GWR_ERR(FORMAT, ...)    GWRCB_ERR(FORMAT, __VA_ARGS__);
//  ############################################################################
//
//                      CB WORKSPACES & PROJECTS LAYOUTS
//
//  ############################################################################
void    OflpModLayout::     Filename_cb_to_oflp(
    wxString        _in_filename    ,
    wxString    &   _out_filename   ,
    int             _type           )
{
    wxString            sfx     =   wxString::FromUTF8(".oflp.layout");
    //  ........................................................................
    _out_filename   =   _in_filename;
    _out_filename.Append(sfx);
}
//  ############################################################################
//                              XML stuff ( boring )
//  ############################################################################
bool    OflpModLayout::     xml_load_document       (wxString& _filename, TiXmlDocument* _doc)
{
    GWR_TKI("OflpModLayout::xml_load_document():[%s]", _filename.wc_str());

    if ( ! TinyXML::LoadDocument(_filename, _doc) )
        return false;

    return true;
}

bool    OflpModLayout::     xml_load_workspace      (cbWorkspace* _wsp, TiXmlDocument* _doc)
{
    wxString    wspfn;
    //  ........................................................................
    //  build oflp workspace layout filename
    Filename_cb_to_oflp( _wsp->GetFilename(), wspfn, 1 );

    //  load the xml file & return it
    return xml_load_document(wspfn, _doc);
}

bool    OflpModLayout::     xml_load_project        (cbProject* _pro, TiXmlDocument* _doc)
{
    wxString    profn;
    //  ........................................................................
    //  build oflp workspace layout filename
    Filename_cb_to_oflp( _pro->GetFilename(), profn, 2 );
    //  ........................................................................
    //  load the xml file & return it
    return xml_load_document(profn, _doc);
}

bool    OflpModLayout::     xml_save_workspace      ()
{
    //  - no TiXml constructore are available in plugins ( why : ??? ). So      //  _GWR_KNO_
    //    every TiXml object has to be new-allocated
    //  ........................................................................
    bool                    bret    =   TRUE;

    cbWorkspace         *   wsp     =   Manager::Get()->GetProjectManager()->GetWorkspace();
    OflpPanel   const   *   panel   =   NULL;

    const   char        *   ROOT_TAG    =  "CodeBlocks_workspace_oflp_file";
            wxString        wspfn;
            wxString        psix;

    TiXmlDocument       *   doc         =   NULL;
    TiXmlDeclaration    *   docdec      =   NULL;
    TiXmlElement        *   rootnode    =   NULL;
    TiXmlElement        *   panelsnode  =   NULL;
    TiXmlElement        *   panelnode   =   NULL;
    TiXmlElement        *   xe1         =   NULL;
    TiXmlElement        *   xe2         =   NULL;
    TiXmlElement        *   xe3         =   NULL;
    TiXmlElement        *   xe4         =   NULL;
    //  ........................................................................
    //  build oflp workspace layout filename
    Filename_cb_to_oflp( wsp->GetFilename(), wspfn, 1 );
    //  ........................................................................
    //  xml stuff - pre
    doc     =   new TiXmlDocument();
    docdec  =   new TiXmlDeclaration("1.0", "UTF-8", "yes");

    doc->SetCondenseWhiteSpace(false);
    doc->InsertEndChild(*docdec);

    xe1         =   new TiXmlElement(ROOT_TAG);
    rootnode    =   static_cast<TiXmlElement*>(doc->InsertEndChild(*xe1));
    if ( ! rootnode )
        goto lab_failure;

    xe2         =   new TiXmlElement("FileVersion");
    xe2->SetAttribute("major", OFLP_FILE_VERSION_MAJOR_STR);
    xe2->SetAttribute("minor", OFLP_FILE_VERSION_MINOR_STR);
    if ( ! rootnode->InsertEndChild(*xe2) )
        goto lab_failure;

    xe3         =   new TiXmlElement("Panels");
    panelsnode  =   static_cast<TiXmlElement*>( rootnode->InsertEndChild(*xe3) );
    if ( ! panelsnode )
        goto lab_failure;
    //  ........................................................................
    //  xml stuff - layout
    for ( size_t i = 0 ; i != panels()->size() ; i++ )
    {
        panel = panels()->panel(i);

        if ( panel->is_bulk() )
            continue;

        xe4         =   new TiXmlElement("Panel");
        panelnode   =   static_cast< TiXmlElement* >( panelsnode->InsertEndChild(*xe4) );
        if ( ! panelnode )
            goto lab_failure;

        psix.Clear();
        psix << panels()->get_visual_index(panel);

        panelnode->SetAttribute( "name"  , cbU2C(panel->title() ));
        panelnode->SetAttribute( "index" , cbU2C(psix           ));

        delete xe4;
    }
    //  ........................................................................
    //  write file
    bret = cbSaveTinyXMLDocument(doc, wspfn);
    //  ........................................................................
    if ( xe1 )  delete xe1;
    if ( xe2 )  delete xe2;
    if ( xe3 )  delete xe3;

    return bret;
    //  ........................................................................
lab_failure:
    if ( doc )  delete doc;
    if ( xe1 )  delete xe1;
    if ( xe2 )  delete xe2;
    if ( xe3 )  delete xe3;
    if ( xe4 )  delete xe4;

    return FALSE;
}

bool    OflpModLayout::     xml_save_project        (cbProject* _pro)
{
    //  - no TiXml constructore are available in plugins ( why : ??? ). So      //  _GWR_KNO_
    //    every TiXml object has to be new-allocated
    //  ........................................................................
    bool                        bret        =   TRUE;
    ProjectManager          *   promgr      =   Manager::Get()->GetProjectManager();
    int                         i,c;
    EditorBase              *   edb         =   NULL;
    ProjectFile             *   pjf         =   NULL;
    wxString                    pjfn;
    cbProject               *   pro         =   NULL;
    OflpPanel       const   *   panel       =   NULL;

    char            const   *   ROOT_TAG    = "CodeBlocks_project_oflp_file";
    wxString                    profn;

    TiXmlDocument   *   doc         =   NULL;
    TiXmlDeclaration*   docdec      =   NULL;
    TiXmlElement    *   rootnode    =   NULL;
    TiXmlElement    *   filenode    =   NULL;
    TiXmlElement    *   xe1         =   NULL;
    TiXmlElement    *   xe2         =   NULL;
    TiXmlElement    *   xe3         =   NULL;
    //  ........................................................................
    //  build oflp workspace layout filename
    Filename_cb_to_oflp( _pro->GetFilename(), profn, 2 );
    //  ........................................................................
    //  xml stuff - pre
    doc     =   new TiXmlDocument();
    docdec  =   new TiXmlDeclaration("1.0", "UTF-8", "yes");

    doc->SetCondenseWhiteSpace(false);
    doc->InsertEndChild(*docdec);

    xe1         =   new TiXmlElement(ROOT_TAG);
    rootnode    =   static_cast<TiXmlElement*>(doc->InsertEndChild(*xe1));
    if ( ! rootnode )
        goto lab_failure;

    xe2         =   new TiXmlElement("FileVersion");
    xe2->SetAttribute("major", OFLP_FILE_VERSION_MAJOR_STR);
    xe2->SetAttribute("minor", OFLP_FILE_VERSION_MINOR_STR);
    if ( ! rootnode->InsertEndChild(*xe2) )
        goto lab_failure;
    //  ........................................................................
    //  xml stuff - layout
    //  FindProjectForFile() inconsistency on project closing -> djb2 on complete filename
    //  ........................................................................
    //  xml stuff - layout
    c = _pro->GetFilesCount();

    for ( i = 0 ; i != c ; i++ )
    {
        pjf     =   _pro->GetFile(i);

        if ( pjf->editorOpen )
        {
            pjfn    =   pjf->file.GetFullPath();
            panel   =   panels()->get_from_abs_path( pjfn );

            if ( panel )
            {
                if ( ! panel->is_bulk() )
                {
                    GWR_TKI("xml_save_project():+[%s] -> [%s]",
                        pjf->relativeFilename.wc_str()  ,
                        panel->title().wc_str()         );

                    xe3 =   new TiXmlElement("File");
                    filenode = static_cast< TiXmlElement* >( rootnode->InsertEndChild(*xe3) );
                    if ( ! filenode )
                        goto lab_failure;

                    filenode->SetAttribute( "name"  , cbU2C(pjf->relativeFilename   ));
                    filenode->SetAttribute( "panel" , cbU2C(panel->title()          ));

                    delete xe3;
                }
            }
        }
        else
        {
            GWR_TKI("xml_save_project(): [%s] not found in any panel",
                pjf->relativeFilename.wc_str()  );
        }
    }
    //  ........................................................................
    //  write file
    bret    =   cbSaveTinyXMLDocument(doc, profn);
    //  ........................................................................
lab_exit:
    if ( doc )  delete doc;
    if ( xe1 )  delete xe1;
    if ( xe2 )  delete xe2;

    return bret;
    //  ........................................................................
lab_failure:
    if ( doc )  delete doc;
    if ( xe1 )  delete xe1;
    if ( xe2 )  delete xe2;
    if ( xe3 )  delete xe3;

    return FALSE;
}

bool    OflpModLayout::     xml_parse_workspace     (TiXmlDocument* _doc)
{
    TiXmlElement*   root    =   _doc->FirstChildElement("CodeBlocks_workspace_oflp_file");
    if ( ! root )
    {
        //GetpMsg()->DebugLog(_T("Not a valid Code::Blocks workspace file..."));
        return false;
    }

    TiXmlElement*   panels  =   root->FirstChildElement("Panels");
    if ( ! panels )
    {
        //GetpMsg()->DebugLog(_T("No 'Panels' element in file..."));
        return false;
    }

    TiXmlElement* panel     =   panels->FirstChildElement("Panel");
    while ( panel )
    {
        long        u       =   0;
        wxString    name    =   cbC2U( panel->Attribute("name") );
        wxString    index   =   cbC2U( panel->Attribute("index") );

        if ( ! index.ToLong(&u) )
        {
            GWR_TKE("xml_parse_workspace():panel[%s] index[%s] - index could not be transformed to int",
                name.wc_str(), index.wc_str());
            return false;
        }
        GWR_TKI("xml_parse_workspace():panel[%s] index[%s]", name.wc_str(), index.wc_str());

        a_panel_assignment_array.Add( new PanelAssignment(name, static_cast< int >( u )) );

                    panel   =   panel->NextSiblingElement("Panel");
    }

    return true;
}

OflpModLayout::ProjectAssignments*
        OflpModLayout::     xml_parse_project       (cbProject* _project, TiXmlDocument* _doc)
{
    ProjectAssignments  *   pa  =   NULL;
    //  ------------------------------------------------------------------------
    //  first see if the project has not already been parsed :
    //  - reloading of project
    if ( p0_project_assignments_get_from_cbProject( _project, pa ) )
    {
        GWR_TKI("xml_parse_project():project[%s] already parsed", _project->GetTitle().wc_str());
        return pa;
    }
    //  ------------------------------------------------------------------------
    //  parse
    TiXmlElement*   root    =   _doc->FirstChildElement("CodeBlocks_project_oflp_file");
    if ( ! root )
    {
        //GetpMsg()->DebugLog(_T("Not a valid Code::Blocks workspace file..."));
        return NULL;
    }

    pa = new ProjectAssignments(_project);

    TiXmlElement*   file    =   root->FirstChildElement("File");
    while ( file )
    {
        wxString    name    =   cbC2U( file->Attribute("name")  );
        wxString    panel   =   cbC2U( file->Attribute("panel") );

        wxString    wxfnstr(_project->GetBasePath());
        wxfnstr.Append(wxS("/"));
        wxfnstr.Append(name);
        wxFileName  wxfn(wxfnstr);

        GWR_TKI("xml_parse_project():file[%s] -> panel[%s]", name.wc_str(), panel.wc_str());

        pa->add( wxfn, name, panel );

                    file    =   file->NextSiblingElement("File");
    }

    return pa;
}
//  ############################################################################
//                              Assignments
//  ############################################################################
bool    OflpModLayout::ProjectAssignments::fas_get_from_rel_fpath(
    wxString        const   &   _rel_path   ,
    FileAssignment          *&  _fas        )
{
    FileAssignment                  *   fas     =   NULL;
    oflp::HString::tHash                hash    =   oflp::HString::Hash(_rel_path);
    //  ........................................................................
    OFLP_STL_CFOR( FileAssignmentArray, a_assignments, it )
    {
        fas = (*it);

        //D GWR_TKI("              ...comparing [%lu] with FileAssignment[%lu][%s]", hash, fas->hrfp(), fas->afp().wc_str());

        if ( fas->hrfp() != hash )
            continue;

        if ( fas->rfp().Cmp( _rel_path ) )
            continue;

        //  here we are
        _fas = fas;
        return true;
    }

    _fas = NULL;
    return FALSE;
}
//  ============================================================================
void        OflpModLayout:: p0_reset_assignments                        ()
{
    OFLP_STL_CFOR( ProjectAssignmentsArray, a_project_assignments_array, it )
    {
        delete (*it);
    }

    a_project_assignments_array.Clear();

    OFLP_STL_CFOR( PanelAssignmentArray, a_panel_assignment_array, it )
    {
        delete (*it);
    }

    a_panel_assignment_array.Clear();
}

bool        OflpModLayout:: p0_project_assignments_get_from_cbProject   (cbProject* _pro, ProjectAssignments*& _pra)
{
    OFLP_STL_CFOR( ProjectAssignmentsArray, a_project_assignments_array, it )
    {
        if ( (*it)->project() == _pro )                                         //  until we find the one that points on the cbProject
        {
            _pra = (*it);
            return true;
        }
    }

    _pra = NULL;
    return FALSE;
}

void        OflpModLayout:: p0_project_assignments_add                  (ProjectAssignments* _pra)
{
    a_project_assignments_array.Add(_pra);
}

void        OflpModLayout:: p0_project_assignments_sub                  (cbProject* _pro)
{
    ProjectAssignments  *   pa  =   NULL;
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::Layout::project_assignments_sub()");
    //  ........................................................................
    OFLP_STL_CFOR( ProjectAssignmentsArray, a_project_assignments_array, it )
    {
        pa  =   (*it);

        if ( pa->project() == _pro )
        {
            a_project_assignments_array.Remove(pa);
            delete pa;
            GWR_TKI("              ...FileAssignment-s removed for project[%s]", _pro->GetTitle().wc_str());
            goto lab_exit;
        }
    }
    //  ........................................................................
lab_exit:
    OFLP_FUNC_EXIT_LOG();
}

bool        OflpModLayout:: p0_project_assignments_get_from_editor_base (
    EditorBase              *   _in_edb                     ,
    ProjectAssignments      *&  _out_project_assignments    ,
    ProjectFile             *&  _out_project_file           )
{
    cbProject                   *       pro         =   NULL;

    ProjectAssignments          *       pa          =   NULL;
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::Layout::p0_project_assignments_get_from_editor_base()");
    //  ........................................................................
    //  init outputs
    _out_project_assignments    =   NULL;
    _out_project_file           =   NULL;
    //  ........................................................................
    //  get the project the file belongs too ( if any )
    if ( ! oflp()->FindCbProjectForFile(_in_edb->GetFilename(), &pro, &_out_project_file) )
    {
        GWR_TKI("              ...no cbProject contains file[%s]", _in_edb->GetFilename().wc_str());
        goto lab_exit_failure;
    }
    //  ........................................................................
    if ( ! p0_project_assignments_get_from_cbProject(pro, _out_project_assignments) )
    {
        GWR_TKW("              ...no ProjectAssignments for cbProject[%s]", pro->GetTitle().wc_str());
        goto lab_exit_failure;
    }
    //  ........................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

bool        OflpModLayout:: p0_file_assignment_get_from_editor_base     (
    EditorBase              *   _in_edb                     ,
    FileAssignment          *&  _out_file_assignment        )
{
    cbProject                   *       pro         =   NULL;
    ProjectFile                 *       pjf         =   NULL;
    ProjectAssignments          *       pas         =   NULL;
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::Layout::p0_file_assignment_get_from_editor_base()");
    //  ........................................................................
    //  init output
    _out_file_assignment        =   NULL;
    //  ........................................................................
    //  get the project the file belongs too ( if any )
    if ( ! oflp()->FindCbProjectForFile(_in_edb->GetFilename(), &pro, &pjf) )
    {
        GWR_TKI("              ...no cbProject contains file[%s]", _in_edb->GetFilename().wc_str());
        goto lab_exit_failure;
    }
    GWR_TKI("              ...cbProject[%p][%s] contains file[%s]", pro, pro->GetTitle().wc_str(), _in_edb->GetFilename().wc_str());

    if ( ! pjf )
    {
        GWR_TKE("%s", wxS("              ...no ProjectFile found"));
        goto lab_exit_failure;
    }
    //  ........................................................................
    //  get the ProjectAssignments from the cbProject
    if ( ! p0_project_assignments_get_from_cbProject(pro, pas) )
    {
        GWR_TKW("              ...no ProjectAssignments for cbProject[%s]", pro->GetTitle().wc_str());
        goto lab_exit_failure;
    }
    //  ........................................................................
    //  get the FileAssignment from ProjectAssignments
    if ( ! pas->fas_get_from_rel_fpath( pjf->relativeFilename, _out_file_assignment ) )
    {
        GWR_TKW("              ...no FileAssignment found for ProjectAssignments[%s]", pro->GetTitle().wc_str());
        goto lab_exit_failure;
    }
    //  ........................................................................
lab_exit_success:
    OFLP_FUNC_EXIT_LOG();
    return TRUE;
    //  ........................................................................
lab_exit_failure:
    OFLP_FUNC_EXIT_LOG();
    return FALSE;
}

OflpModLayout::FileAssignment   const   *
            OflpModLayout:: file_assignment_find                        (
    EditorBase          *   _nn_edb)
{
    FileAssignment      *   fa          =   NULL;

    OflpPanel           *   panel       =   NULL;
    //  ........................................................................
    OFLP_FUNC_ENTER_MARK("OFLP::Layout::file_assignment_find()");

    if ( p0_file_assignment_get_from_editor_base( _nn_edb, fa ) )
        return fa;

    return NULL;
}

void        OflpModLayout:: file_assignment_update                      (
    EditorBase  *   _nn_edb         ,
    OflpPanel   *   _nn_dst_panel   )
{
    ProjectAssignments  *   pas         =   NULL;
    FileAssignment      *   fas         =   NULL;
    ProjectFile         *   pjf         =   NULL;

    wxFileName              awxfn;
    wxString                rfp;
    OflpPanel const     *   panel       =   NULL;
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::Layout::file_assignment_update()");
    //  ........................................................................
    //  ProjectAssignments found
    if ( p0_project_assignments_get_from_editor_base(_nn_edb, pas, pjf) )
    {
        //  ProjectAssignments found, FileAssignment found
        if ( pas->fas_get_from_rel_fpath( pjf->relativeFilename, fas ) )
        {
            //  ................................................................
            //  find current assigned panel
            panel   =   panels()->get_by_name( fas->pname() );

            if ( ! panel )
            {
                GWR_TKI("              ...assignment found, but OflpPanel not found[%s]",fas->pname().wc_str());
                goto lab_exit;
            }

            if ( panel == _nn_dst_panel )
            {
                GWR_TKI("              ...assignment found, but dst == src [%s], dropping !", panel->title().wc_str());
                goto lab_exit;
            }
            //  ................................................................
            //  remove old assignment
            pas->sub(fas);
            //  ................................................................
            //  set vars for adding new assignment
            awxfn   =   fas->awxfn();
            rfp     =   fas->rfp();
            //  ................................................................
            //  add
            goto lab_add;
        }
        //  ProjectAssignments found, FileAssignment not found
        else
        {
            //  ................................................................
            //  set vars for adding new assignment
            awxfn   =   wxFileName(_nn_edb->GetFilename());
            rfp     =   pjf->relativeFilename;
            //  ................................................................
            //  add
            goto lab_add;
        }
    }
    //  ........................................................................
    //  ProjectAssignments not found
    else
    {
        GWR_WNG("              ...no project assignment found for file[%s]", _nn_edb->GetFilename().wc_str());
        goto lab_exit;
    }
    //  ........................................................................
lab_add:
    GWR_TKI("              ...adding new assignment [%s]->[%s]", rfp.wc_str(), _nn_dst_panel->title().wc_str());
    pas->add( awxfn, rfp, _nn_dst_panel->title() );
    //  ........................................................................
lab_exit:
    OFLP_FUNC_EXIT_LOG();
}
//  ############################################################################
//                              Actions
//  ############################################################################
void    OflpModLayout::     reset                       ()
{
    p0_reset_assignments();
}
//  ############################################################################
//                              Events
//  ############################################################################
bool    OflpModLayout::     workspace_load              ()
{
    OFLP_FUNC_ENTER_LOG("Layout::workspace_load()");
    //  ........................................................................
    ProjectManager      *   pjm     =   NULL;
    cbWorkspace         *   wsp     =   NULL;
    ProjectsArray       *   pja     =   NULL;
    cbProject           *   pro     =   NULL;
    TiXmlDocument       *   doc_wsp =   NULL;
    TiXmlDocument       *   doc_pro =   NULL;

    ProjectAssignments  *   pas     =   NULL;
    //  ........................................................................
    //  get current workspace & projects
    pjm = Manager::Get()->GetProjectManager();
    wsp = pjm->GetWorkspace();
    pja = pjm->GetProjects();
    //  ........................................................................
    GWR_TKI("workspace_load():wks[%s]", wsp->GetTitle().wc_str());
    //  load the oflp workspace layout
    doc_wsp =   new TiXmlDocument();

    if ( ! xml_load_workspace(wsp, doc_wsp) )
    {
        GWR_TKE("workspace_load():%s[%s]", wxS("oflp workspace xml layout _NOT_ loaded"), wsp->GetTitle().wc_str());
        goto lab_exit_failure;
    }
    GWR_TKI("workspace_load():%s[%s]", wxS("oflp workspace xml layout loaded"), wsp->GetTitle().wc_str());
    //  ........................................................................
    //  parse for workspace
    if ( ! xml_parse_workspace(doc_wsp) )                                       //  add asignments for current workspace
        goto lab_exit_failure;
    //  ........................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

bool    OflpModLayout::     workspace_close             ()
{
    OFLP_FUNC_ENTER_LOG("Layout::workspace_close()");

    bool bret = workspace_save();

    OFLP_FUNC_EXIT_LOG();

    return bret;
}

bool    OflpModLayout::     workspace_save              ()
{
    OFLP_FUNC_ENTER_LOG("Layout::workspace_save()");

    bool bret = xml_save_workspace();

    OFLP_FUNC_EXIT_LOG();

    return bret;
}

bool    OflpModLayout::     project_load                (cbProject* _pro)
{
    ProjectAssignments  *   pa  =   NULL;
    TiXmlDocument       *   doc =   NULL;
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("Layout::project_load()");
    GWR_INF("project title[%s]", _pro->GetTitle().wc_str());
    //  ........................................................................
    //  load the project's oflp-layout
    doc =   new TiXmlDocument();

    if ( ! xml_load_project(_pro, doc) )
    {
        GWR_TKE("project_load():%s[%s]", wxS("oflp project xml layout _NOT_ loaded"), _pro->GetFilename().wc_str());
        delete doc; doc = NULL;
        goto lab_exit_failure;
    }
    GWR_TKI("project_load():%s[%s]", wxS("oflp project xml layout loaded"), _pro->GetFilename().wc_str());
    //  ........................................................................
    //  parse the layout, convert it to FileAssignments and add it to
    //  ProjectAssignments
    pa = xml_parse_project(_pro, doc);
    if ( pa )
        p0_project_assignments_add(pa);
    else
        goto lab_exit_failure;
    //  ........................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

bool    OflpModLayout::     project_close               (cbProject* _pro)
{
    OFLP_FUNC_ENTER_LOG("Layout::project_close()");

    project_save(_pro);                                                         //  _GWR_TODO_

    p0_project_assignments_sub(_pro);
    //  ........................................................................
lab_exit:
    OFLP_FUNC_EXIT_LOG();
    return true;
}

bool    OflpModLayout::     project_save                (cbProject* _pro)
{
    OFLP_FUNC_ENTER_LOG("Layout::project_save()");

    GWR_INF("project title[%s]", _pro->GetTitle().wc_str());

    if ( ! xml_save_project(_pro) )
        goto lab_exit_failure;
    //  ........................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}


