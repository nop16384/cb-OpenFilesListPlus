  private:
    wxPanel                         *   dw_MainPanel;
    wxBoxSizer                      *   dw_MainSizer;

    OpenFilesListPlusPanel          *   a_BulkPanel;                            //!< Bulk panel permanent access
    PanelArray                          a_panels_array;                         //!< All panels, including bulk panel
    //  ------------------------------------------------------------------------
  public:
    void                    panels_reset    ();
    void                    panels_unselect (OFLPPanel*);

    OFLPPanel           *   panel_add       (wxString _title, bool _bulk = false);
    void                    panel_move_up   (OFLPPanel*);
    void                    panel_move_dn   (OFLPPanel*);
    void                    panel_del       (OFLPPanel*);

    OFLPPanel           *   panel_get               (EditorBase*);
    int                     panel_get_visual_index  (OFLPPanel*);

