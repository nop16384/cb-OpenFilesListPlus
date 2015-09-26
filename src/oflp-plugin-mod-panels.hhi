/// ****************************************************************************
//! \class  OflpModPanels
//!
//! \brief  Contain all panel-s stuff
/// ****************************************************************************
class OflpModPanels : public OflpModule
{
    friend class OpenFilesListPlus;
    friend class OflpModEditors;
    //  ------------------------------------------------------------------------
  private:
    wxPanel                     *   dw_main_panel;
    wxBoxSizer                  *   dw_main_sizer;

    OpenFilesListPlusPanel      *   a_panel_bulk;                               //!< Bulk panel permanent access
    PanelArray                      a_panels_array;                             //!< All panels, including bulk panel
    //  ........................................................................
  private:
    wxPanel                     *&  p0_main()   { return dw_main_panel; }
    wxBoxSizer                  *   p0_sizer()  { return dw_main_sizer; }
    OFLPPanel                   *   p0_bulk()   { return a_panel_bulk;  }
  public:
    PanelArray      const       &   array()     { return a_panels_array;    }
    //  ------------------------------------------------------------------------
  private:
    void                    p0_reset            ();

    void                    p0_freeze()         { dw_main_panel->Freeze();      }
    void                    p0_thaw()           { dw_main_panel->Thaw();        }

    void                    p0_resize           ();

    void                    p0_editors_mov      (OFLPPanel* _dst, OFLPPanel* _src);
    void                    p0_editor_mov       (OFLPPanel* _dst, OFLPPanel* _src, EditorBase* _edb);

    void                    p0_unselect_except  (OFLPPanel*);
    OFLPPanel           *   p0_add              (wxString const _title, bool _bulk = false);
    void                    p0_sub              (OFLPPanel*);
    void                    p0_move_up          (OFLPPanel*);
    void                    p0_move_dn          (OFLPPanel*);
    //  ........................................................................
  public:
    void                    init();
    void                    resize_and_layout   ();
    void                    resize              ();
    void                    layout();

    OFLPPanel           *   get_by_name         (wxString const & _name);
    OFLPPanel           *   get_from_abs_path   (wxString const & _abs_path);
    OFLPPanel           *   get                 (EditorBase*);
    int                     get_visual_index    (OFLPPanel*);
    //  ........................................................................
  public:
    OflpModPanels();
   ~OflpModPanels();
};
