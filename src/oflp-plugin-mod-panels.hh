/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PLUGIN_MOD_PANELS_HH__
#define __OFLP_PLUGIN_MOD_PANELS_HH__

#include    "oflp-common.hh"

#include    "oflp-plugin-module.hh"

/// ************************************************************************************************
//! \class  OflpModPanels
//!
//! \brief  Contain all panel-s stuff
/// ************************************************************************************************
class OflpModPanels : public OflpModule
{
    friend class OpenFilesListPlus;
    friend class OflpModEditors;
    //  --------------------------------------------------------------------------------------------
    //  graphical stuff
    //  --------------------------------------------------------------------------------------------
  private:
    wxPanel                         *   dw_main_panel;
    wxBoxSizer                      *   dw_main_sizer;

    OpenFilesListPlusPanel          *   a_panel_bulk;                                               //!< Bulk panel permanent access
    OflpPanelArray                      a_panels_array;
    //  ............................................................................................
  private:
    wxPanel                     *   p0_main()           { return dw_main_panel;     }
    wxBoxSizer                  *   p0_sizer()          { return dw_main_sizer;     }
    OflpPanel                   *   p0_bulk()           { return a_panel_bulk;      }
  public:
    size_t                          size()              { return a_panels_array.size();     }
    OflpPanel                   *   panel(size_t _ix)   { return a_panels_array[_ix];       }
    //  --------------------------------------------------------------------------------------------
  private:
    void                    z_destroy           ();
    void                    z_reset             ();

    void                    p0_freeze()         { dw_main_panel->Freeze();      }
    void                    p0_thaw()           { dw_main_panel->Thaw();        }

    void                    p0_set_bgs          (wxColour&);

    void                    p0_resize           ();
    void                    p0_layout           ();

    void                    p0_unselect_except  (OflpPanel*);
    OflpPanel           *   z_add_bulk          (wxString const _title);
    OflpPanel           *   z_add               (wxString const _title, oflp::UID);
    bool                    p0_sub              (OflpPanel*);                                           //  x
    bool                    p0_move_up          (OflpPanel*);                                           //  x
    bool                    p0_move_dn          (OflpPanel*);                                           //  x
    bool                    p0_minmax           (OflpPanel*);                                           //  x
  public:
    void                    init();
    void                    resize_and_layout   ();
    //  --------------------------------------------------------------------------------------------
    //  logical stuff
    //  --------------------------------------------------------------------------------------------
  private:
    int                     a_id_main_panel;

  public:
    bool                    x_panel_from_uid            (OflpPanel** _panel, oflp::UID _uid);
    OflpPanel           *   x_panel_from_from_file_path (wxString const & _abs_path);
    OflpPanel           *   get_by_name                 (wxString const & _name);
    OflpPanel           *   x_panel_from_editor         (EditorBase*);
    int                     get_visual_index            (OflpPanel const *)             const;
    //  --------------------------------------------------------------------------------------------
    //  editors stuff
    //  --------------------------------------------------------------------------------------------
  private:
    bool                    z_editors_mov   (OflpPanel* _nn_dst, OflpPanel* _nn_src);
    bool                    z_editor_mov    (OflpPanel* _nn_dst, OflpPanel* _nn_src, EditorBase* _nn_edb);
    //  --------------------------------------------------------------------------------------------
  public:
    OflpModPanels();
   ~OflpModPanels();
};

#endif
