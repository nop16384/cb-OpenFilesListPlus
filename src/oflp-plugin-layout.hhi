  private:
    /// ************************************************************************
    //! \class  Layout
    //!
    //! \brief  Save & Load OpenFilesListPlus panels & open files layout
    /// ************************************************************************
    class   Layout
    {
        //friend class    OpenFilesListPlus;
        //  --------------------------------------------------------------------
      public:
        class   FileAssignment;
        class   PanelAssignment;
        class   ProjectAssignments;
        //  --------------------------------------------------------------------
        WX_DEFINE_ARRAY(TiXmlDocument       *, TiXmlDocArray );                 //  temporary storage
        WX_DEFINE_ARRAY(PanelAssignment     *, PanelAssignmentArray );          //  panels of the workspace
        WX_DEFINE_ARRAY(FileAssignment      *, FileAssignmentArray );           //  per project : file -> panel assignment
        WX_DEFINE_ARRAY(ProjectAssignments  *, ProjectAssignmentsArray );
        /// ********************************************************************
        //! \class  FileAssignment
        //!
        //! \brief  (filename -> OFLPPanel) unidirectional association
        //!
        //! \detail Filenames are stored as earlgreycb::HString so they have a
        //!     hash value.
        /// ********************************************************************
      public:
        class   FileAssignment
        {
          private:
            earlgreycb::HString     a_file;
            earlgreycb::HString     a_panel;

          public:
            wxString                    const   &   file()      { return a_file.get();  }
            earlgreycb::HString::tHash              hfile()     { return a_file.hash(); }
            wxString                    const   &   panel()     { return a_panel.get(); }
            earlgreycb::HString::tHash              hpanel()    { return a_panel.hash();}

          public:
            FileAssignment(wxString& _file_name, wxString& _panel_name)
                {
                    a_file.set  (_file_name );
                    a_panel.set (_panel_name);
                }
           ~FileAssignment()
                {
                }
        };
        /// ********************************************************************
        //! \class  PanelAssignment
        //!
        //! \brief  (OFLPPanel name, OFLPPanel index) couple
        /// ********************************************************************
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
        /// ********************************************************************
        //! \class  ProjectAssignments
        //!
        //! \brief  All FileAssignment-s for one project
        /// ********************************************************************
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

            FileAssignmentArray const   &   array()
                {
                    return a_assignments;
                }

            void                    add(wxString& _filename, wxString& _panel_name)
                {
                    a_assignments.Add( new FileAssignment(_filename, _panel_name) );
                }

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
        //  ====================================================================
      private:
        static  void        Filename_cb_to_oflp  (wxString _in_filename, wxString& _out_filename, int _type);
                void        dump_project_manager ();
        //  ....................................................................
      private:
        bool                    xml_load_document       (wxString   & _filename , TiXmlDocument*);
        bool                    xml_load_workspace      (cbWorkspace*           , TiXmlDocument*);
        bool                    xml_load_project        (cbProject  *           , TiXmlDocument*);

        bool                    xml_save_workspace      ();
        bool                    xml_save_project        (cbProject*);

        bool                    xml_parse_workspace     (TiXmlDocument*);
        ProjectAssignments  *   xml_parse_project       (cbProject* _project, TiXmlDocument*);
        //  ....................................................................
      private:
        PanelAssignmentArray            a_panel_assignment_array;
        ProjectAssignmentsArray         a_project_assignments_array;

        void                reset_assignments           ();
        void                project_assignments_add     (ProjectAssignments*);
        void                project_assignments_sub     (cbProject*);

      public:
        wxString                            file_assignment_find_panel_name (ProjectFile* _nn_pjf);
        PanelAssignmentArray    const   &   panel_assignment_array      ()  { return a_panel_assignment_array;      }
        ProjectAssignmentsArray const   &   project_assignments_array   ()  { return a_project_assignments_array;   }
        //  ....................................................................
      public:
        void                reset               ();

        void                workspace_load      ();
        bool                workspace_close     ();
        bool                workspace_save      ();

        void                project_load        (cbProject*);
        void                project_close       (cbProject*);
        void                project_save        (cbProject*);
        //  ....................................................................
      public:
        Layout()                                                                {}
       ~Layout()                                                                {}
    };
    //  ========================================================================
  private:
    Layout              *   d_layout;                                           //!< OpenFilesListPlus layout stuff
        //  ....................................................................
  public:
    Layout              *   layout()                                    const
        {
            return d_layout;
        }

