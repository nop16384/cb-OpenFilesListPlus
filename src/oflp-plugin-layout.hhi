  private:
    /// ************************************************************************
    //! \class  Layout
    //!
    //! \brief  Save & Load OpenFilesListPlus panels & open files layout
    /// ************************************************************************
    class   Layout
    {
        friend class    OpenFilesListPlus;
        //  --------------------------------------------------------------------
      private:
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
      private:
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
      private:
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
      private:
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
        /// ********************************************************************
        //! \class  XmlWorkspaceSnapshot
        //!
        //! \brief  Memorize a workspace's layout
        //!
        //! \detail Due to uncomplete cbEvent framework, I am forced to do
        //!     memorizations.
        /// ********************************************************************
      private:
        class   XmlWorkspaceSnapshot
        {
          private:
            TiXmlDocument   *   d_xml_doc;
            wxString            a_title;
            wxString            a_xml_filename;

          public:
            TiXmlDocument   *   xml_doc()       { return d_xml_doc;         }
            wxString        &   xml_filename()  { return a_xml_filename;    }
            wxString        &   wsp_title()     { return a_title;           }

          public:
            XmlWorkspaceSnapshot(cbWorkspace* _wsp, TiXmlDocument* _doc);
            XmlWorkspaceSnapshot()                      :   d_xml_doc(NULL)     {}
           ~XmlWorkspaceSnapshot()
                {
                    if ( d_xml_doc ) delete d_xml_doc;
                }
        };
        //  ====================================================================
      private:
        static  void        P0_filename_cb_to_oflp  (wxString _in_filename, wxString& _out_filename, int _type);
                void        p0_dump_project_manager ();
        //  ....................................................................
      private:
        XmlWorkspaceSnapshot        *   d_xml_wsp_snapshot_1;

      private:
        bool        xml_load_document    (wxString   & _filename , TiXmlDocument*);
        bool        xml_load_workspace   (cbWorkspace*           , TiXmlDocument*);
        bool        xml_load_project     (cbProject  *           , TiXmlDocument*);

        XmlWorkspaceSnapshot    *   xml_workspace_take_snapshot     ();
        void                        xml_workspace_update_snapshot   ();
        bool                        xml_workspace_save_snapshot     ();
        bool                        xml_workspace_snapshot_new      ();
        bool                        xml_save_project                (cbProject  *);

        bool        xml_parse_workspace     (TiXmlDocument*);
        bool        xml_parse_project       (cbProject              * _project, TiXmlDocument*);
        //  ....................................................................
      private:
        PanelAssignmentArray            a_panel_assignments_array;
        ProjectAssignmentsArray         a_project_assignments_array;

        void                assignments_reset        ();
        wxString            assignment_file_find     (EditorBase*);
        //  ....................................................................
      private:
        void                p0_reset();

        void                create_panels   ();
        void                apply           ();

        void                on_workspace_loading_complete   ();
        void                on_project_opened               (cbProject*);

        void                on_project_save                 (cbProject*);

      public:
        Layout()
            :   d_xml_wsp_snapshot_1(NULL)                                      {}
       ~Layout()
            {
                if ( d_xml_wsp_snapshot_1 ) delete d_xml_wsp_snapshot_1;
            }
    };
    //  ========================================================================
  private:
    Layout              *   d_layout;                                           //!< OpenFilesListPlus layout stuff
    //  ------------------------------------------------------------------------
  public:
    Layout              *   layout()                                    const
        {
            return d_layout;
        }
