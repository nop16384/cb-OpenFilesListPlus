/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-common.hh"
#include    "oflp-common-macros.hh"

#include    "oflp-cb.hh"

#include    "oflp-plugin-mod-layout.hh"

#include    "oflp-plugin.hh"
#include    "oflp-plugin-mod-panels.hh"
#include    "oflp-panel.hh"
//  ................................................................................................
//  defines specific to this module
#define     ERG_OFLP_SANITY_CHECKS
#include    "generated/oflp-plugin-mod-layout--log-defines.cci"
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
bool    OflpModLayout::     z_xml_save_workspace    ()
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
    for ( size_t i = 0 ; i != oflp::Modules::Instance()->panels()->x_card() ; i++ )
    {
        panel = oflp::Modules::Instance()->panels()->x_get(i);

        if ( panel->is_bulk() )
            continue;

        xe4         =   new TiXmlElement("Panel");
        panelnode   =   static_cast< TiXmlElement* >( panelsnode->InsertEndChild(*xe4) );
        if ( ! panelnode )
            goto lab_failure;

        psix.Clear();
        psix << oflp::Modules::Instance()->panels()->x_get_vix(panel);

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

bool    OflpModLayout::     z_xml_save_project      (cbProject* _pro)
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
            ERG_INF("ProjectFile [%p] editorOpen[N], dropping [%s]", pjf, pjf->file.GetFullPath());
            continue;
        }
        ERG_INF("ProjectFile [%p] editorOpen[Y]           [%s]", pjf, pjf->file.GetFullPath());

        //L//if ( ! OpenFilesListPlus::CB::X_cbEditor_from_file_path(&ed, pjf->file.GetFullPath()) )
        if ( ! oflp::CB::X_cbEditor_from_file_path(&ed, pjf->file.GetFullPath()) )
        {
            ERG_TKE("ProjectFile [%p][%s] has editorOpen, but no cbEditor found", pjf, pjf->file.GetFullPath());
            continue;
        }

        panel = oflp::Modules::Instance()->panels()->x_get_from_editor( ed );

        if ( ! panel )
        {
            ERG_TKE("ProjectFile [%p][%s] has editorOpen, but not found in any panel", pjf, pjf->file.GetFullPath());
            continue;
        }

        if ( ! panel->is_bulk() )
        {
            ERG_TKI("+[%s] -> [%s][%s]",
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
        else
        {
            ERG_TKI("ProjectFile [%p][%s] is in bulk panel", pjf, pjf->file.GetFullPath());
        }
    }
    //  ............................................................................................
    //  write file
    bret    =   cbSaveTinyXMLDocument(&doc, profn);
    //  ............................................................................................
lab_exit:
    if ( xe1 )  delete xe1;
    if ( xe2 )  delete xe2;
    OFLP_LOG_FUNC_EXIT();
    return bret;
    //  ............................................................................................
lab_failure:
    if ( xe1 )  delete xe1;
    if ( xe2 )  delete xe2;
    if ( xe3 )  delete xe3;
    OFLP_LOG_FUNC_EXIT();
    return FALSE;
}

bool    OflpModLayout::     z_xml_parse_workspace   (TiXmlDocument* _doc)
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
        ERG_ERR("%s", wxS("no <CodeBlocks_workspace_oflp_file> found"));
        goto lab_exit_failure;
    }

    panels = root->FirstChildElement("Panels");
    if ( ! panels )
    {
        ERG_ERR("%s", wxS("no <Panels> found"));
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
            ERG_ERR("panel [%s][%s][%s] - index could not be transformed to int",
                uid.wc_str(), name.wc_str(), index.wc_str());
                goto lab_exit_failure;
        }
        z_pnas_add( new PnAs(oflp::UID(uid), name, static_cast< int >( l1 )) );

        panel = panel->NextSiblingElement("Panel");
    }

    //  ............................................................................................
    ERG_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

bool    OflpModLayout::     z_xml_parse_project     (cbProject* _project, TiXmlDocument* _doc)
{
    PjAs    *   pjas    =   nullptr;
    //  ............................................................................................
    //  first see if the project has not already been parsed :
    //  - reloading of project
    if ( x_pjas_get( &pjas, _project ) )
    {
        ERG_TKI("z_xml_parse_project():project[%s] already parsed", _project->GetTitle().wc_str());
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

        ERG_TKI("xml_parse_project():file[%s] -> panel[%s]", name.wc_str(), uid.wc_str());

        pjas->x_pfas_add(name, uid );

                    file    =   file->NextSiblingElement("File");
    }

    z_pjas_add(pjas);
    return true;
}
//  ################################################################################################
//                              Module
//  ################################################################################################
void        OflpModLayout::     modrelease          ()
{
    z_reset();
}
//  ################################################################################################
//                              Assignments
//  ################################################################################################
bool        OflpModLayout::PjAs::   x_pfas_get  (
    PfAs        const   **  _pfas   ,
    wxString    const   &   _fn     ,
    wxString    const   &   _rfp    )   const
{
    PfAs                    *           pfas    =   nullptr;
    oflp::HString::tHash                hfn     =   oflp::HString::Hash(_fn);
    oflp::HString::tHash                hrfp    =   oflp::HString::Hash(_rfp);
    //  ............................................................................................
    OFLP_STL_CFOR( PfAsAr, a_pf_as_ar, it )
    {
        pfas = (*it);

        //D ERG_TKI("              ...comparing [%lu] with FileAssignment[%lu][%s]", hash, fas->hrfp(), fas->afp().wc_str());

        if ( pfas->hrfp() != hrfp )
            continue;

        if ( pfas->rfp().Cmp( _rfp ) )
            continue;

        //  here we are
        *(_pfas) = pfas;
        return true;
    }
    *(_pfas) = nullptr;

    return FALSE;
}
//  ================================================================================================
void        OflpModLayout::     z_pnas_add      (PnAs* _pnas)
{
    OFLP_LOG_FUNC_ENTER("OFLP::Layout::z_pnas_add()");

    ERG_TKI("adding PnAs[%s][%i][%s]",
        _pnas->uid().str().wc_str(), _pnas->vix(), _pnas->name().wc_str());
    a_pn_as_ar.Add( _pnas);

    OFLP_LOG_FUNC_EXIT();
}

bool        OflpModLayout::     z_pnas_sub      (PnAs* _pnas)
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
            ERG_TKI("              ...PnAs removing[%s][%s][%i]",
                pnas->uid().str().wc_str(), pnas->name().wc_str(), pnas->vix());
            a_pn_as_ar.Remove(pnas);
            delete pnas;
            goto lab_exit_success;
        }
    }
    ERG_TKE("              ...PnAs _NOT_ removed[%s][%s][%i]",
                pnas->uid().str().wc_str(), pnas->name().wc_str(), pnas->vix());

    goto lab_exit_failure;
    //  ............................................................................................
    ERG_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

void        OflpModLayout::     z_pjas_add      (PjAs*      _pjas)
{
    a_pj_as_ar.Add(_pjas);
}

bool        OflpModLayout::     z_pjas_sub      (cbProject* _pro)
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
            ERG_TKI("              ...ProjectAssignments removed for project[%s]", _pro->GetTitle().wc_str());
            goto lab_exit_success;
        }
    }
    ERG_TKE("              ...No ProjectAssignments was removed for project[%s]", _pro->GetTitle().wc_str());
    goto lab_exit_failure;
    //  ............................................................................................
    ERG_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}
//  ================================================================================================
bool        OflpModLayout::     x_pjas_get  (
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

bool        OflpModLayout::     x_pfas_get  (
    PfAs        const   **  _pfas       ,
    cbProject   const   *   _nn_prj     ,
    wxString    const   &   _fn         ,
    wxString    const   &   _rfp        )
{
    ProjectFile                 *       pjf         =   NULL;
    PjAs                        *       pjas        =   NULL;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::Layout::x_flas_get()");
    //  ............................................................................................
    //  get the ProjectAssignments from the cbProject
    if ( ! x_pjas_get(&pjas, _nn_prj) )
    {
        ERG_TKW("              ...no ProjectAssignments for cbProject[%s]", _nn_prj->GetTitle().wc_str());
        goto lab_exit_failure;
    }
    //  ............................................................................................
    //  get the FileAssignment from ProjectAssignments
    if ( ! pjas->x_pfas_get( _pfas, _fn, _rfp ) )
    {
        ERG_TKW("              ...no FileAssignment found for cbProject[%s]", _nn_prj->GetTitle().wc_str());
        goto lab_exit_failure;
    }
    //  ............................................................................................
    ERG_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}
//  ################################################################################################
//                              Actions
//  ################################################################################################
void        OflpModLayout::     z_reset                                     ()
{
    //  we delete all Assignments, and free the memory used by the wxArrays too

    OFLP_STL_CFOR( PjAsAr, a_pj_as_ar, it )
    {
        delete (*it);
    }

    a_pj_as_ar.Clear();
    ERG_TKI("OflpModLayout::x_reset():a_pj_as_ar GetCount[%lu]", a_pj_as_ar.GetCount());

    OFLP_STL_CFOR( PnAsAr, a_pn_as_ar, it )
    {
        delete (*it);
    }

    a_pn_as_ar.Clear();
    ERG_TKI("OflpModLayout::x_reset():a_pn_as_ar GetCount[%lu]", a_pn_as_ar.GetCount());
}
//  ################################################################################################
//                              Events
//  ################################################################################################
bool    OflpModLayout::         x_workspace_load            ()
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
    ERG_TKI("workspace[%s]", wsp->GetTitle().wc_str());
    //  ............................................................................................
    //  load the oflp workspace layout
    Cb_filename_to_oflp_filename( wsp->GetFilename(), fp, 1 );                                      // build oflp workspace layout filename
    ERG_TKI("workspace oflp file [%s]", fp.wc_str());

    if ( ! TinyXML::LoadDocument(fp, &doc) )
    {
        ERG_TKE("%s", wxS("workspace oflp file could _NOT_ be loaded"));
        goto lab_exit_failure;
    }
    ERG_TKI("%s", wxS("workspace oflp file loaded"));
    //  ............................................................................................
    //  parse
    if ( ! z_xml_parse_workspace(&doc) )
        goto lab_exit_failure;
    //  ............................................................................................
    ERG_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

bool    OflpModLayout::         x_workspace_save            ()
{
    OFLP_LOG_FUNC_ENTER("Layout::workspace_save()");

    ERG_TKI("workspace[%s]", Manager::Get()->GetProjectManager()->GetWorkspace()->GetTitle().wc_str());

    if ( ! z_xml_save_workspace() )
        goto lab_exit_failure;

    ERG_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

bool    OflpModLayout::         x_project_load              (cbProject* _pro)
{
    PjAs            *   pa  =   NULL;
    TiXmlDocument       doc;
    wxString            fp;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("Layout::project_load()");
    ERG_INF("project title[%s]", _pro->GetTitle().wc_str());
    //  ............................................................................................
    //  load the project's oflp-layout
    Cb_filename_to_oflp_filename( _pro->GetFilename(), fp, 2 );                                     //  build oflp project layout filename
    ERG_TKI("project oflp file [%s]", fp.wc_str());

    if ( ! TinyXML::LoadDocument(fp, &doc) )
    {
        ERG_TKE("%s", wxS("project oflp file could _NOT_ be loaded"));
        goto lab_exit_failure;
    }
    ERG_TKI("%s", wxS("project oflp file loaded"));
    //  ............................................................................................
    //  parse
    if ( ! z_xml_parse_project(_pro, &doc) )
        goto lab_exit_failure;
    //  ............................................................................................
    ERG_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}

bool    OflpModLayout::         x_project_close             (cbProject* _pro)
{
    OFLP_LOG_FUNC_ENTER("Layout::x_project_close()");

    ERG_INF("project title[%s]", _pro->GetTitle().wc_str());

    if ( ! x_project_save(_pro) )
    {
        ERG_ERR("project title[%s] could not be saved", _pro->GetTitle().wc_str());
    }

    z_pjas_sub(_pro);
    //  ............................................................................................
lab_exit:
    OFLP_LOG_FUNC_EXIT();
    return true;
}

bool    OflpModLayout::         x_project_save              (cbProject* _pro)
{
    OFLP_LOG_FUNC_ENTER("Layout::x_project_save()");

    ERG_INF("project title[%s]", _pro->GetTitle().wc_str());

    if ( ! z_xml_save_project(_pro) )
        goto lab_exit_failure;
    //  ............................................................................................
    ERG_LABELS_EXIT_SUCCESS_FAILURE_RTF();
}
//  ################################################################################################
//                              Debug
//  ################################################################################################
void    OflpModLayout::         x_dump_assignments              ()
{
    OFLP_LOG_FUNC_ENTER("Layout::x_dump_assignments()");

    ProjectManager      *   pjm     =   Manager::Get()->GetProjectManager();
    //cbWorkspace         *   wsp     =   Manager::Get()->GetProjectManager()->GetWorkspace();

    //wsp->

    //  ............................................................................................
    OFLP_LOG_FUNC_EXIT();
}
