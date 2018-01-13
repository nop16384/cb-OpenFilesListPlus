/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin-mod-layout.hh"

#include    "oflp-plugin.hh"

#include    "oflp-plugin-mod-panels.hh"

#include    "oflp-panel.hh"
//  ................................................................................................
//  specific to this file oflp defines
#define GWR_OFLP_SANITY_CHECKS
#define GWR_LOG(FORMAT, ...)    GWRCB_LOG(FORMAT, __VA_ARGS__);
#ifdef  OFLP_DEBUG_LAYOUT
    #define GWR_TKI(FORMAT, ...)    GWRCB_TKI(FORMAT, __VA_ARGS__);
    #define GWR_TKW(FORMAT, ...)    GWRCB_TKW(FORMAT, __VA_ARGS__);
    #define GWR_TKE(FORMAT, ...)    GWRCB_TKE(FORMAT, __VA_ARGS__);
#endif
#define GWR_INF(FORMAT, ...)    GWRCB_INF(FORMAT, __VA_ARGS__);
#define GWR_WNG(FORMAT, ...)    GWRCB_WNG(FORMAT, __VA_ARGS__);
#define GWR_ERR(FORMAT, ...)    GWRCB_ERR(FORMAT, __VA_ARGS__);
//  ################################################################################################
//
//                      CB WORKSPACES & PROJECTS LAYOUTS
//
//  ################################################################################################
void    OflpModLayout::     Cb_filename_to_oflp_filename(
    wxString        _in_filename    ,
    wxString    &   _out_filename   ,
    int             _type           )
{
    wxString            sfx     =   wxString::FromUTF8(".oflp-layout");
    //  ............................................................................................
    _out_filename   =   _in_filename;
    _out_filename.Append(sfx);
}
//  ################################################################################################
//                              XML stuff ( boring )
//  ################################################################################################


bool    OflpModLayout::     x_xml_save_workspace    ()
{
    //  - no TiXml constructore are available in plugins ( why : ??? ). So      //  _GWR_KNO_
    //    every TiXml object has to be new-allocated - NOT in new CB versions
    //  ............................................................................................
    bool                    bret    =   TRUE;

    cbWorkspace         *   wsp     =   Manager::Get()->GetProjectManager()->GetWorkspace();
    OflpPanel   const   *   panel   =   NULL;

    const   char        *   ROOT_TAG    =  "CodeBlocks_workspace_oflp_file";
            wxString        wspfn;
            wxString        psix;

    TiXmlDocument           doc;
    TiXmlDeclaration    *   docdec      =   NULL;
    TiXmlElement        *   rootnode    =   NULL;
    TiXmlElement        *   panelsnode  =   NULL;
    TiXmlElement        *   panelnode   =   NULL;
    TiXmlElement        *   xe1         =   NULL;
    TiXmlElement        *   xe2         =   NULL;
    TiXmlElement        *   xe3         =   NULL;
    TiXmlElement        *   xe4         =   NULL;
    //  ............................................................................................
    //  build oflp workspace layout filename
    Cb_filename_to_oflp_filename( wsp->GetFilename(), wspfn, 1 );
    //  ............................................................................................
    //  xml stuff - pre
    doc.SetCondenseWhiteSpace(false);

    docdec  =   new TiXmlDeclaration("1.0", "UTF-8", "yes");
    doc.InsertEndChild(*docdec);

    delete docdec;

    xe1         =   new TiXmlElement(ROOT_TAG);

    rootnode    =   static_cast<TiXmlElement*>(doc.InsertEndChild(*xe1));
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
    //  ............................................................................................
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

        panelnode->SetAttribute( "uid"   , panel->uid().str()                           );
        panelnode->SetAttribute( "name"  , cbU2C(panel->title() )                       );
        panelnode->SetAttribute( "index" , cbU2C(psix           )                       );

        delete xe4;
    }
    //  ............................................................................................
    //  write file
    bret = cbSaveTinyXMLDocument(&doc, wspfn);
    //  ............................................................................................
    if ( xe1 )  delete xe1;
    if ( xe2 )  delete xe2;
    if ( xe3 )  delete xe3;

    return bret;
    //  ............................................................................................
lab_failure:
    if ( xe1 )  delete xe1;
    if ( xe2 )  delete xe2;
    if ( xe3 )  delete xe3;
    if ( xe4 )  delete xe4;

    return FALSE;
}

bool    OflpModLayout::     x_xml_save_project      (cbProject* _pro)
{
    //  - no TiXml constructore are available in plugins ( why : ??? ). So      //  _GWR_KNO_
    //    every TiXml object has to be new-allocated
    //  ............................................................................................
    bool                        bret        =   TRUE;
    ProjectManager          *   promgr      =   Manager::Get()->GetProjectManager();
    int                         i,c;
    cbEditor                *   ed          =   NULL;
    ProjectFile             *   pjf         =   NULL;
    OflpPanel       const   *   panel       =   NULL;

    char            const   *   ROOT_TAG    = "CodeBlocks_project_oflp_file";
    wxString                    profn;

    TiXmlDocument       doc;
    TiXmlDeclaration*   docdec      =   NULL;
    TiXmlElement    *   rootnode    =   NULL;
    TiXmlElement    *   filenode    =   NULL;
    TiXmlElement    *   xe1         =   NULL;
    TiXmlElement    *   xe2         =   NULL;
    TiXmlElement    *   xe3         =   NULL;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("Layout::z_xml_save_project()");
    //  ............................................................................................
    //  build oflp workspace layout filename
    Cb_filename_to_oflp_filename( _pro->GetFilename(), profn, 2 );
    //  ............................................................................................
    //  xml stuff - pre
    docdec  =   new TiXmlDeclaration("1.0", "UTF-8", "yes");

    doc.SetCondenseWhiteSpace(false);
    doc.InsertEndChild(*docdec);

    xe1         =   new TiXmlElement(ROOT_TAG);
    rootnode    =   static_cast<TiXmlElement*>(doc.InsertEndChild(*xe1));
    if ( ! rootnode )
        goto lab_failure;

    xe2         =   new TiXmlElement("FileVersion");
    xe2->SetAttribute("major", OFLP_FILE_VERSION_MAJOR_STR);
    xe2->SetAttribute("minor", OFLP_FILE_VERSION_MINOR_STR);
    if ( ! rootnode->InsertEndChild(*xe2) )
        goto lab_failure;
    //  ............................................................................................
    //  xml stuff - layout
    //  FindProjectForFile() inconsistency on project closing -> djb2 on complete filename
    //  ............................................................................................
    //  xml stuff - layout
    c = _pro->GetFilesCount();

    for ( i = 0 ; i != c ; i++ )
    {
        pjf     =   _pro->GetFile(i);

        if ( ! pjf->editorOpen )
        {
            GWR_INF("ProjectFile [%p][%s] has _NOT_ editorOpen, dropping", pjf, pjf->file.GetFullPath());
            continue;
        }

        if ( ! OpenFilesListPlus::CB::X_cbEditor_from_file_path(&ed, pjf->file.GetFullPath()) )
        {
            GWR_TKE("ProjectFile [%p][%s] has editorOpen, but no cbEditor found", pjf, pjf->file.GetFullPath());
            continue;
        }

        panel = panels()->x_panel_from_editor( ed );

        if ( ! panel )
        {
            GWR_TKE("ProjectFile [%p][%s] has editorOpen, but not found in any panel", pjf, pjf->file.GetFullPath());
            continue;
        }

        if ( ! panel->is_bulk() )
        {
            GWR_TKI("+[%s] -> [%s][%s]",
                pjf->relativeFilename.wc_str()  ,
                panel->uid().str().wc_str()     ,
                panel->title().wc_str()         );

            xe3 =   new TiXmlElement("File");
            filenode = static_cast< TiXmlElement* >( rootnode->InsertEndChild(*xe3) );
            if ( ! filenode )
                goto lab_failure;

            filenode->SetAttribute( "name"  , cbU2C(pjf->relativeFilename           ));
            filenode->SetAttribute( "uid"   , cbU2C(panel->uid().str()              ));

            delete xe3;
        }
    }
    //  ............................................................................................
    //  write file
    bret    =   cbSaveTinyXMLDocument(&doc, profn);
    //  ............................................................................................
lab_exit:
    if ( xe1 )  delete xe1;
    if ( xe2 )  delete xe2;

    return bret;
    //  ............................................................................................
lab_failure:
    if ( xe1 )  delete xe1;
    if ( xe2 )  delete xe2;
    if ( xe3 )  delete xe3;

    return FALSE;
}

bool    OflpModLayout::     x_xml_parse_workspace   (TiXmlDocument* _doc)
{
    OFLP_LOG_FUNC_ENTER("Layout::z_xml_parse_workspace()");
    //  ............................................................................................
    TiXmlElement    *   root    =   nullptr;
    TiXmlElement    *   panels  =   nullptr;
    TiXmlElement    *   panel   =   nullptr;
    //  ............................................................................................
    root = _doc->FirstChildElement("CodeBlocks_workspace_oflp_file");
    if ( ! root )
    {
        GWR_ERR("%s", wxS("no <CodeBlocks_workspace_oflp_file> found"));
        goto lab_exit_failure;
    }

    panels = root->FirstChildElement("Panels");
    if ( ! panels )
    {
        GWR_ERR("%s", wxS("no <Panels> found"));
        goto lab_exit_failure;
    }

    panel = panels->FirstChildElement("Panel");
    while ( panel )
    {
        long        l1      =   0;
        wxString    uid     =   cbC2U( panel->Attribute("uid")  );
        wxString    name    =   cbC2U( panel->Attribute("name") );
        wxString    index   =   cbC2U( panel->Attribute("index"));

        if ( ! index.ToLong(&l1) )
        {
            GWR_ERR("panel [%s][%s][%s] - index could not be transformed to int",
                uid.wc_str(), name.wc_str(), index.wc_str());
                goto lab_exit_failure;
        }
        z_pnas_add( new PnAs(oflp::UID(uid), name, static_cast< int >( l1 )) );

        panel = panel->NextSiblingElement("Panel");
    }

    //  ............................................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

bool    OflpModLayout::     x_xml_parse_project     (cbProject* _project, TiXmlDocument* _doc)
{
    PjAs    *   pjas    =   nullptr;
    //  ............................................................................................
    //  first see if the project has not already been parsed :
    //  - reloading of project
    if ( x_pjas( &pjas, _project ) )
    {
        GWR_TKI("xml_parse_project():project[%s] already parsed", _project->GetTitle().wc_str());
        return true;
    }
    //  ............................................................................................
    //  parse
    TiXmlElement*   root    =   _doc->FirstChildElement("CodeBlocks_project_oflp_file");
    if ( ! root )
    {
        //GetpMsg()->DebugLog(_T("Not a valid Code::Blocks workspace file..."));
        return NULL;
    }

    pjas = new PjAs(_project);

    TiXmlElement*   file    =   root->FirstChildElement("File");
    while ( file )
    {
        wxString    name    =   cbC2U( file->Attribute("name")  );
        wxString    uid     =   cbC2U( file->Attribute("uid") );

        wxFileName  wxfn( _project->GetBasePath() + wxString(wxS("/")) + name );

        GWR_TKI("xml_parse_project():file[%s] -> panel[%s]", name.wc_str(), uid.wc_str());

        pjas->add( wxfn, name, uid );

                    file    =   file->NextSiblingElement("File");
    }

    z_pjas_add(pjas);
    return true;
}
//  ################################################################################################
//                              Assignments
//  ################################################################################################
bool        OflpModLayout::PjAs::   x_flas  (
    FlAs        const   **  _flas   ,
    wxString    const   &   _fn     ,
    wxString    const   &   _rfp    )   const
{
    FlAs                    *           flas    =   nullptr;
    oflp::HString::tHash                hfn     =   oflp::HString::Hash(_fn);
    oflp::HString::tHash                hrfp    =   oflp::HString::Hash(_rfp);
    //  ............................................................................................
    OFLP_STL_CFOR( FlAsAr, a_fl_as_ar, it )
    {
        flas = (*it);

        //D GWR_TKI("              ...comparing [%lu] with FileAssignment[%lu][%s]", hash, fas->hrfp(), fas->afp().wc_str());

        if ( flas->hrfp() != hrfp )
            continue;

        if ( flas->rfp().Cmp( _rfp ) )
            continue;

        //  here we are
        *(_flas) = flas;
        return true;
    }
    *(_flas) = nullptr;

    return FALSE;
}
//  ================================================================================================
void        OflpModLayout:: x_reset                                     ()
{
    //  we delete all Assignments, and free the memory used by the wxArrays too

    OFLP_STL_CFOR( PjAsAr, a_pj_as_ar, it )
    {
        delete (*it);
    }

    a_pj_as_ar.Clear();
    GWR_TKI("OflpModLayout::z_reset():a_pj_as_ar GetCount[%lu]", a_pj_as_ar.GetCount());

    OFLP_STL_CFOR( PnAsAr, a_pn_as_ar, it )
    {
        delete (*it);
    }

    a_pn_as_ar.Clear();
    GWR_TKI("OflpModLayout::z_reset():a_pj_as_ar GetCount[%lu]", a_pn_as_ar.GetCount());
}

void        OflpModLayout:: z_pnas_add      (PnAs* _pnas)
{
    OFLP_LOG_FUNC_ENTER("OFLP::Layout::z_pnas_add()");

    GWR_TKI("adding PnAs[%s][%i][%s]",
        _pnas->uid().str().wc_str(), _pnas->index(), _pnas->name().wc_str());
    a_pn_as_ar.Add( _pnas);

    OFLP_LOG_FUNC_EXIT();
}

bool        OflpModLayout:: z_pnas_sub      (PnAs* _pnas)
{
    PnAs    *   pnas    =   nullptr;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::Layout::z_pnas_sub()");
    //  ............................................................................................
    OFLP_STL_CFOR( PnAsAr, a_pn_as_ar, it )
    {
        pnas = (*it);

        if ( pnas == _pnas )
        {
            GWR_TKI("              ...PnAs removing[%s][%s][%i]",
                pnas->uid().str().wc_str(), pnas->name().wc_str(), pnas->index());
            a_pn_as_ar.Remove(pnas);
            delete pnas;
            goto lab_exit_success;
        }
    }
    GWR_TKE("              ...PnAs _NOT_ removed[%s][%s][%i]",
                pnas->uid().str().wc_str(), pnas->name().wc_str(), pnas->index());

    goto lab_exit_failure;
    //  ............................................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}



void        OflpModLayout:: z_pjas_add      (PjAs*      _pra)
{
    a_pj_as_ar.Add(_pra);
}

bool        OflpModLayout:: z_pjas_sub      (cbProject* _pro)
{
    PjAs    *   pjas    =   nullptr;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::Layout::z_pjas_sub()");
    //  ............................................................................................
    OFLP_STL_CFOR( PjAsAr, a_pj_as_ar, it )
    {
        pjas = (*it);

        if ( pjas->project() == _pro )
        {
            a_pj_as_ar.Remove(pjas);
            delete pjas;
            GWR_TKI("              ...ProjectAssignments removed for project[%s]", _pro->GetTitle().wc_str());
            goto lab_exit_success;
        }
    }
    GWR_TKE("              ...No ProjectAssignments was removed for project[%s]", _pro->GetTitle().wc_str());
    goto lab_exit_failure;
    //  ............................................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}
/*
bool        OflpModLayout:: p0_project_assignments_get_from_editor_base (
    EditorBase              *   _in_edb                     ,
    ProjectAssignments      *&  _out_project_assignments    ,
    ProjectFile             *&  _out_project_file           )
{
    cbProject                   *       pro         =   NULL;

    ProjectAssignments          *       pa          =   NULL;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::Layout::p0_project_assignments_get_from_editor_base()");
    //  ............................................................................................
    //  init outputs
    _out_project_assignments    =   NULL;
    _out_project_file           =   NULL;
    //  ............................................................................................
    //  get the project the file belongs too ( if any )
    if ( ! oflp()->CB::X_cbProject_from_file_path(_in_edb->GetFilename(), &pro, &_out_project_file) )
    {
        GWR_TKI("              ...no cbProject contains file[%s]", _in_edb->GetFilename().wc_str());
        goto lab_exit_failure;
    }
    //  ............................................................................................
    if ( ! p0_project_assignments_get_from_cbProject(pro, _out_project_assignments) )
    {
        GWR_TKW("              ...no ProjectAssignments for cbProject[%s]", pro->GetTitle().wc_str());
        goto lab_exit_failure;
    }
    //  ............................................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}
*/
//  ================================================================================================
bool        OflpModLayout::         x_pjas  (
    PjAs                **  _pjas   ,
    cbProject   const   *   _pro    )
{
    OFLP_STL_CFOR( PjAsAr, a_pj_as_ar, it )
    {
        if ( (*it)->project() == _pro )                                                             //  until we find the one that points on the cbProject
        {
            *(_pjas) = (*it);
            return true;
        }
    }

    *(_pjas) = NULL;
    return FALSE;
}

bool        OflpModLayout::         x_flas  (
    FlAs        const   **  _flas       ,
    cbProject   const   *   _nn_prj     ,
    wxString    const   &   _fn         ,
    wxString    const   &   _rfp        )
{
    ProjectFile                 *       pjf         =   NULL;
    PjAs                        *       pjas        =   NULL;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::Layout::x_flas()");
    //  ............................................................................................
    //  get the ProjectAssignments from the cbProject
    if ( ! x_pjas(&pjas, _nn_prj) )
    {
        GWR_TKW("              ...no ProjectAssignments for cbProject[%s]", _nn_prj->GetTitle().wc_str());
        goto lab_exit_failure;
    }
    //  ............................................................................................
    //  get the FileAssignment from ProjectAssignments
    if ( ! pjas->x_flas( _flas, _fn, _rfp ) )
    {
        GWR_TKW("              ...no FileAssignment found for cbProject[%s]", _nn_prj->GetTitle().wc_str());
        goto lab_exit_failure;
    }
    //  ............................................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}
//  ################################################################################################
//                              Actions
//  ################################################################################################
//  ################################################################################################
//                              Events
//  ################################################################################################
bool    OflpModLayout::     x_workspace_load            ()
{
    OFLP_LOG_FUNC_ENTER("Layout::workspace_load()");
    //  ............................................................................................
    ProjectManager      *   pjm     =   NULL;
    cbWorkspace         *   wsp     =   NULL;

    wxString                fp;
    TiXmlDocument           doc;
    //  ............................................................................................
    //  get current workspace & projects
    pjm = Manager::Get()->GetProjectManager();
    wsp = pjm->GetWorkspace();
    GWR_TKI("workspace[%s]", wsp->GetTitle().wc_str());
    //  ............................................................................................
    //  load the oflp workspace layout
    Cb_filename_to_oflp_filename( wsp->GetFilename(), fp, 1 );                                      // build oflp workspace layout filename
    GWR_TKI("workspace oflp file [%s]", fp.wc_str());

    if ( ! TinyXML::LoadDocument(fp, &doc) )
    {
        GWR_TKE("%s", wxS("workspace oflp file could _NOT_ be loaded"));
        goto lab_exit_failure;
    }
    GWR_TKI("%s", wxS("workspace oflp file loaded"));
    //  ............................................................................................
    //  parse
    if ( ! x_xml_parse_workspace(&doc) )
        goto lab_exit_failure;
    //  ............................................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

bool    OflpModLayout::     x_workspace_save            ()
{
    OFLP_LOG_FUNC_ENTER("Layout::workspace_save()");

    GWR_TKI("workspace[%s]", Manager::Get()->GetProjectManager()->GetWorkspace()->GetTitle().wc_str());

    if ( ! x_xml_save_workspace() )
        goto lab_exit_failure;

    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

bool    OflpModLayout::     x_project_load              (cbProject* _pro)
{
    PjAs            *   pa  =   NULL;
    TiXmlDocument       doc;
    wxString            fp;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("Layout::project_load()");
    GWR_INF("project title[%s]", _pro->GetTitle().wc_str());
    //  ............................................................................................
    //  load the project's oflp-layout
    Cb_filename_to_oflp_filename( _pro->GetFilename(), fp, 2 );                                     //  build oflp project layout filename
    GWR_TKI("project oflp file [%s]", fp.wc_str());

    if ( ! TinyXML::LoadDocument(fp, &doc) )
    {
        GWR_TKE("%s", wxS("project oflp file could _NOT_ be loaded"));
        goto lab_exit_failure;
    }
    GWR_TKI("%s", wxS("project oflp file loaded"));
    //  ............................................................................................
    //  parse
    if ( ! x_xml_parse_project(_pro, &doc) )
        goto lab_exit_failure;
    //  ............................................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

bool    OflpModLayout::     x_project_close             (cbProject* _pro)
{
    OFLP_LOG_FUNC_ENTER("Layout::z_project_close()");

    GWR_INF("project title[%s]", _pro->GetTitle().wc_str());

    if ( ! x_project_save(_pro) )
    {
        GWR_ERR("project title[%s] could not be saved", _pro->GetTitle().wc_str());
    }

    z_pjas_sub(_pro);
    //  ............................................................................................
lab_exit:
    OFLP_LOG_FUNC_EXIT();
    return true;
}

bool    OflpModLayout::     x_project_save              (cbProject* _pro)
{
    OFLP_LOG_FUNC_ENTER("Layout::z_project_save()");

    GWR_INF("project title[%s]", _pro->GetTitle().wc_str());

    if ( ! x_xml_save_project(_pro) )
        goto lab_exit_failure;
    //  ............................................................................................
    GWR_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}


