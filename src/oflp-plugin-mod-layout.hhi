/// ****************************************************************************
//! \class  OflpModLayout
//!
//! \brief  Save & Load OpenFilesListPlus panels & open files layout
/// ****************************************************************************
class OflpModLayout : public wxMenu, public OflpModule
{
    //friend class    OpenFilesListPlus;
    //  ------------------------------------------------------------------------
  public:
    class   FileAssignment;
    class   PanelAssignment;
    class   ProjectAssignments;
    //  ------------------------------------------------------------------------
    WX_DEFINE_ARRAY(TiXmlDocument       *, TiXmlDocArray );                 //  temporary storage
    WX_DEFINE_ARRAY(PanelAssignment     *, PanelAssignmentArray );          //  panels of the workspace
    WX_DEFINE_ARRAY(FileAssignment      *, FileAssignmentArray );           //  per project : file -> panel assignment
    WX_DEFINE_ARRAY(ProjectAssignments  *, ProjectAssignmentsArray );
    /// ************************************************************************
    //! \class  PanelAssignment
    //!
    //! \brief  (OFLPPanel name, OFLPPanel index) couple
    /// ************************************************************************
  public:
    class   PanelAssignment
    {
      private:
        wxString    a_name;
        int         a_index;

      public:
        wxString                    const   &   name()      { return a_name;    }
        int                                     index()     { return a_index;   }

      public:
        PanelAssignment(wxString& _panel_name, int _panel_index)
            {
                a_name  =   _panel_name;
                a_index =   _panel_index;
            }
       ~PanelAssignment()
            {
            }
    };
    /// ************************************************************************
    //! \class  FileAssignment
    //!
    //! \brief  (filename -> OFLPPanel) unidirectional association
    //!
    //! \detail Using some earlgreycb::HString-s for accelerating access-s
    /// ************************************************************************
  public:
    class   FileAssignment
    {
      private:
        wxFileName                  a_absolute_filename;                    //! absolute filename
        earlgreycb::HString         a_absolute_filepath;                    //! absolute filepath  + hash
        earlgreycb::HString         a_relative_filepath;                    //! project-relative filepath + h
        earlgreycb::HString         a_panel_name;                           //! assigned panel name + hash

      public:
        wxFileName                  const   &   awxfn()     { return a_absolute_filename;           }
        wxString                    const   &   afp()       { return a_absolute_filepath.get();     }
        earlgreycb::HString::tHash              hafp()      { return a_absolute_filepath.hash();    }
        wxString                    const   &   rfp()       { return a_relative_filepath.get();     }
        earlgreycb::HString::tHash              hrfp()      { return a_relative_filepath.hash();    }
        wxString                    const   &   pname()     { return a_panel_name.get();            }
        earlgreycb::HString::tHash              hpname()    { return a_panel_name.hash();           }

      public:
        FileAssignment(
            wxFileName  const &   _absolute_filename  ,
            wxString    const &   _relative_filepath  ,
            wxString    const &   _panel_name         )
            {
                a_absolute_filename     =   _absolute_filename;
                a_absolute_filepath     =   _absolute_filename.GetFullPath();
                a_relative_filepath.set ( _relative_filepath );
                a_panel_name.set        (_panel_name);
            }
       ~FileAssignment()
            {
            }
    };
    /// ************************************************************************
    //! \class  ProjectAssignments
    //!
    //! \brief  All FileAssignment-s for one project
    /// ************************************************************************
  public:
    class   ProjectAssignments
    {
      private:
        cbProject               *   a_project;
        FileAssignmentArray         a_assignments;

      public:
        cbProject           const   *   project()
            {
                return a_project;
            }

        void                    add(wxFileName const & _wx_filename, wxString const & _relative_filename, wxString const & _panel_name)
            {
                a_assignments.Add( new FileAssignment(_wx_filename, _relative_filename, _panel_name) );
            }
        void                    sub(FileAssignment* _fa)
            {
                a_assignments.Remove( _fa );
            }
        bool                    fas_get_from_rel_fpath(wxString const & _rel_path, FileAssignment *& _fa);

      public:
        ProjectAssignments(cbProject* _project) :   a_project(_project) {}
       ~ProjectAssignments()
            {
                for ( FileAssignmentArray::iterator
                    it  =   a_assignments.begin()   ;
                    it  !=  a_assignments.end()     ;
                    it++                            )
                {
                    delete (*it);
                }
                a_assignments.Clear();
            }
    };
    //  ########################################################################
  private:
    static  void            Filename_cb_to_oflp     (wxString _in_filename, wxString& _out_filename, int _type);
    //  ------------------------------------------------------------------------
  private:
    bool                    xml_load_document       (wxString   & _filename , TiXmlDocument*);
    bool                    xml_load_workspace      (cbWorkspace*           , TiXmlDocument*);
    bool                    xml_load_project        (cbProject  *           , TiXmlDocument*);

    bool                    xml_save_workspace      ();
    bool                    xml_save_project        (cbProject*);

    bool                    xml_parse_workspace     (TiXmlDocument*);
    ProjectAssignments  *   xml_parse_project       (cbProject* _project, TiXmlDocument*);
    //  ------------------------------------------------------------------------
  private:
    PanelAssignmentArray    a_panel_assignment_array;
    ProjectAssignmentsArray a_project_assignments_array;

    bool                    p0_project_assignments_get_from_cbProject   (cbProject*, ProjectAssignments *&);

    void                    p0_reset_assignments                        ();
    void                    p0_project_assignments_add                  (ProjectAssignments*);
    void                    p0_project_assignments_sub                  (cbProject*);

    bool                    p0_project_assignments_get_from_editor_base (EditorBase*, ProjectAssignments*& _project_assignments, ProjectFile*& _out_project_file);
    bool                    p0_file_assignment_get_from_editor_base     (EditorBase*, FileAssignment*& _file_assignment);

  public:
    OFLPPanel       *       file_assignment_find_panel_from_editor_base (EditorBase* _nn_edb);
    void                    file_assignment_update                      (EditorBase* _nn_edb, OFLPPanel* _nn_dst_panel);

    PanelAssignmentArray    const   &   panel_assignment_array          ()  { return a_panel_assignment_array;      }
    //  ------------------------------------------------------------------------
  public:
    void                reset               ();

    void                workspace_load      ();
    bool                workspace_close     ();
    bool                workspace_save      ();

    void                project_load        (cbProject*);
    void                project_close       (cbProject*);
    void                project_save        (cbProject*);
    //  ------------------------------------------------------------------------
  public:
    OflpModLayout()                                                             {}
   ~OflpModLayout()                                                             {}
};
