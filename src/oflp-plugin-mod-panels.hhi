/// ****************************************************************************
//! \class  OflpModPanels
//!
//! \brief  Contain all panel-s stuff
/// ****************************************************************************
class OflpModPanels : public wxMenu, public OflpModule
{
    friend class OpenFilesListPlus;
    friend class OflpModEditors;
    //  ....................................................................
  private:
    wxPanel                     *   dw_MainPanel;
    wxBoxSizer                  *   dw_MainSizer;

    OpenFilesListPlusPanel      *   a_BulkPanel;                            //!< Bulk panel permanent access
    PanelArray                      a_panels_array;                         //!< All panels, including bulk panel
    //  ....................................................................
  private:
    wxPanel                     *&  p0_main()   { return dw_MainPanel;  }
    wxBoxSizer                  *&  p0_sizer()  { return dw_MainSizer;  }
    OFLPPanel                   *&  p0_bulk()   { return a_BulkPanel;   }
  public:
    OFLPPanel       const       *   bulk()      { return a_BulkPanel;       }
    PanelArray      const       &   array()     { return a_panels_array;    }
    //  ....................................................................
  private:
    void                    p0_reset            ();

    void                    p0_freeze()         { dw_MainPanel->Freeze();   }
    void                    p0_thaw()           { dw_MainPanel->Thaw();     }

    void                    p0_resize           ();

    void                    p0_editors_mov      (OFLPPanel* _dst, OFLPPanel* _src);
    void                    p0_editor_mov       (OFLPPanel* _dst, OFLPPanel* _src, EditorBase* _edb);

    void                    p0_unselect_except  (OFLPPanel*);
    OFLPPanel           *   p0_add              (wxString _title, bool _bulk = false);
    void                    p0_move_up          (OFLPPanel*);
    void                    p0_move_dn          (OFLPPanel*);
    void                    p0_sub              (OFLPPanel*);
    //  ....................................................................
  private:
    OFLPPanel           *   p0_find             (EditorBase* _nn_edb)
        {
            return const_cast< OFLPPanel* >( find(_nn_edb) );
        }
    //  ....................................................................
  public:
    OFLPPanel           *   get_by_name                 (wxString const & _name);
    OFLPPanel           *   get_from_absolute_filepath  (wxString& _absolute_filepath);
    OFLPPanel           *   find                        (EditorBase*);
    int                     get_visual_index            (OFLPPanel*);
    //  ....................................................................
  public:
    OflpModPanels()    {}
   ~OflpModPanels()    {}
};
