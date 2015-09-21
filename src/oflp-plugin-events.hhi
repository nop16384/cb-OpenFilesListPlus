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


    void                    evh_menu_option_checked(wxCommandEvent &);
    wxMenu              *   aw_menu_view;                                       //!< c::b "View" menu


  private:
        void                cb_event_dump               (CodeBlocksEvent& _evt);
        void                editor_dump                 (EditorBase* _edb);
