/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PLUGIN_MOD_PANELS_HH__
#define __OFLP_PLUGIN_MOD_PANELS_HH__

//#include    "oflp-common.hh"

#include    "oflp-plugin-module.hh"
#include    "oflp-util-uid.hh"

/// ************************************************************************************************
//! \class  OflpModPanels
//!
//! \brief  Contain all panel-s stuff
/// ************************************************************************************************
class OflpModPanels : public OflpModule
{
    friend class OpenFilesListPlus;
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
    wxPanel             *   z_get_main()        { return dw_main_panel;     }
    OflpPanel           *   z_get_bulk()        { return a_panel_bulk;      }

  public:
    size_t                  x_card              ()                  { return a_panels_array.size();     }
    OflpPanel   const   *   x_get               (size_t _ix)        { return a_panels_array[_ix];       }
    int                     x_get_vix           (OflpPanel const *)                                         const;
    bool                    x_get_from_uid      (OflpPanel** _panel, oflp::UID _uid);
    OflpPanel           *   x_get_from_editor   (EditorBase*);


  public:
    void                    x_set_col__bg_p     (wxColour&);
    void                    x_set_col__bg_h     (wxColour&);
    //  --------------------------------------------------------------------------------------------
  private:
    void                    z_destroy               ();
    void                    z_reset                 ();

    void                    z_resize                ();
    void                    z_layout                ();

    void                    z_unselect_except   (OflpPanel*);
    OflpPanel           *   z_add_bulk          (wxString const _title);
    OflpPanel           *   z_add               (wxString const _title, oflp::UID);
    bool                    z_sub               (OflpPanel*);                                           //  x
    bool                    z_move_up           (OflpPanel*);                                           //  x
    bool                    z_move_dn           (OflpPanel*);                                           //  x
    bool                    z_minmax            (OflpPanel*);                                           //  x

  public:
    void                    init();
    void                    x_resize_and_layout   ();
    //  --------------------------------------------------------------------------------------------
    //  editors stuff
    //  --------------------------------------------------------------------------------------------
  private:
    bool                    z_editors_mov   (OflpPanel* _nn_dst, OflpPanel* _nn_src);
    bool                    z_editor_mov    (OflpPanel* _nn_dst, OflpPanel* _nn_src, EditorBase* _nn_edb);
    //  --------------------------------------------------------------------------------------------
  private:
    virtual     void    z_release() {}
    //  --------------------------------------------------------------------------------------------
  public:
    OflpModPanels();
   ~OflpModPanels();
};

#endif
