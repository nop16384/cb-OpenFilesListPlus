  protected:
    void    evh_view_open_files_plus(wxCommandEvent& event);
    void    evh_update_ui           (wxUpdateUIEvent& event);

    void    evh_tree_item_activated     (wxTreeEvent& event);
    void    evh_tree_item_right_click   (wxTreeEvent& event);
    void    evh_tree_sel_changed        (wxTreeEvent &event);

    void    evh_editor_opened       (CodeBlocksEvent& event);
    void    evh_editor_closed       (CodeBlocksEvent& event);
    void    evh_editor_activated    (CodeBlocksEvent& event);
    void    evh_editor_deactivated  (CodeBlocksEvent& event);
    void    evh_editor_modified     (CodeBlocksEvent& event);
    void    evh_editor_saved        (CodeBlocksEvent& event);

    void    evh_workspace_loading_complete  (CodeBlocksEvent& event);
    void    evh_workspace_close_begin       (CodeBlocksEvent& event);
    void    evh_workspace_close_complete    (CodeBlocksEvent& event);
    void    evh_project_open                (CodeBlocksEvent& event);
    void    evh_project_close               (CodeBlocksEvent& event);
    void    evh_project_save                (CodeBlocksEvent& event);

    void    evh_panel_header_button_clicked_add     (wxCommandEvent &);
    void    evh_panel_header_button_clicked_del     (wxCommandEvent &);
    void    evh_panel_header_button_clicked_mm      (wxCommandEvent &);
    void    evh_panel_header_button_clicked_up      (wxCommandEvent &);
    void    evh_panel_header_button_clicked_down    (wxCommandEvent &);
    void    evh_panel_header_button_clicked_opt     (wxCommandEvent &);


  protected:
    OFLPPanel           *   a_dnd_destination_panel;

  public:
    OFLPPanel   const   *   dnd_dst_panel()
        {
            return a_dnd_destination_panel;
        }

    void                    dnd_dst_panel_set(OFLPPanel* _panel)
        {
            a_dnd_destination_panel = _panel;
        }

  private:
        void                cb_event_dump               (CodeBlocksEvent& _evt);
        void                editor_dump                 (EditorBase* _edb);
        OFLPPanel       *   file_assignment_find_panel  (ProjectFile* _nn_pjf);
        void                editor_add                  (EditorBase* _edb);
