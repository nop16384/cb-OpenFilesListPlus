  private:
    WX_DEFINE_ARRAY(OpenFilesListPlusPanel  *, PanelArray);

    wxPanel                         *   dw_MainPanel;
    wxBoxSizer                      *   dw_MainSizer;

    OpenFilesListPlusPanel          *   a_BulkPanel;                            //!< Bulk panel permanent access
    PanelArray                          a_panels_array;                         //!< All panels, including bulk panel
    //  ------------------------------------------------------------------------
  private:
    void                    p0_freeze()     { dw_MainPanel->Freeze();   }
    void                    p0_thaw()       { dw_MainPanel->Thaw();     }

    void                    p0_panel_items_transfer     (OFLPPanel* _dst, OFLPPanel* _src);
    void                    p0_panel_reset              (OFLPPanel*);
    void                    p0_panels_reset             (bool _update_layout_workspace_snapshot);

    OFLPPanel           *   p0_panel_get_by_name        (wxString const & _name);

  public:
    void                    panels_unselect (OFLPPanel*);

    OFLPPanel           *   panel_add       (wxString _title, bool _bulk = false);
    void                    panel_move_up   (OFLPPanel*);
    void                    panel_move_dn   (OFLPPanel*);
    void                    panel_sub       (OFLPPanel*);

    OFLPPanel           *   panel_find              (EditorBase*);
    OFLPPanel           *   panel_find              (ProjectFile*);
    int                     panel_get_visual_index  (OFLPPanel*);

