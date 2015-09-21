/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PANEL_HH__
#define __OFLP_PANEL_HH__
//  ............................................................................
#include    "oflp-common.hh"                                                    //  standard wx includes + OFLP log system
//  ............................................................................
#include    <wx/dnd.h>                                                          //  for OpenFilesListPlusPanelDropTarget
//  ............................................................................
#include    <wx/dynarray.h>

#include    "oflp-panel-header.hhi"
#include    "oflp-panel-utils.hhi"
/// ****************************************************************************
//! \class  OpenFilesListPlusPanel
//!
//! \brief  wxPanel containing a OpenFilesListPlusPanelHeader and a wxTreeCtrl
/// ****************************************************************************
class OpenFilesListPlusPanel              : public wxPanel
{
  protected:
    bool                    a_bulk;

    bool                    a_allow_kill_focus_event;

    OFLPPanelHeader     *   dw_header;
    wxTreeCtrl          *   d_tree;
    wxBoxSizer          *   dw_sizer;

    EditorArray                 a_editors_array;                                //  speed up finding editors
    earlgreycb::HString::HArray a_harray;

    OpenFilesListPlusPanelDropTarget  *   d_drop_target;
    //  ------------------------------------------------------------------------
  public:
    bool                            is_bulk()                   const   { return a_bulk;                }
    EditorArray const   *           get_editors()               const   { return &a_editors_array;      }
    wxTreeCtrl  const   *   const   tree()                      const   { return d_tree;                }
    wxString    const   &           get_title()                 const   { return dw_header->get_title();}
    //  ------------------------------------------------------------------------
    static      wxString            stringize_drag_result(wxDragResult _dres);
    //  ------------------------------------------------------------------------
  protected:
    void                    p0_create_tree      ();

    wxTreeItemId            item_append         (EditorBase*);
    wxTreeItemId            item_find           (EditorBase*);
    bool                    item_del            (EditorBase*);
    bool                    item_select         (EditorBase*);
    bool                    item_selected       (EditorBase*);

    void                    items_deselect      ();
    void                    items_del           ();
    //  ------------------------------------------------------------------------
  public:
    void                    reset();

    void                    dump();

    EditorBase  *           editor_from_absolute_filepath   (wxString& _absolute_filepath);

    bool                    editor_has      (EditorBase*);
    void                    editor_add      (EditorBase*);
    void                    editor_del      (EditorBase*);
    void                    editor_select   (EditorBase*);
    bool                    editor_selected (EditorBase*);
    void                    editor_sync     (EditorBase*);

    void                    editors_del     ();
    void                    editors_deselect();

    bool                    is_minimized();
    void                    minimize();
    void                    maximize();
    //  ------------------------------------------------------------------------
  protected:
    void                    OnSelect    (wxCommandEvent&);
    void                    OnDragInit  (wxTreeEvent&);
    void                    OnDragEnd   (wxTreeEvent&);
    //  ------------------------------------------------------------------------
  private:
    void                    p0_allow_kill_focus_event               (bool);
  public:
    void                    evh_title_static_LEFT_DOWN              (wxMouseEvent   &);
    void                    evh_title_dynamic_TEXT_ENTER            (wxCommandEvent &);
    void                    evh_title_dynamic_KILL_FOCUS            (wxFocusEvent   &);
    //  ------------------------------------------------------------------------
  public:
             OpenFilesListPlusPanel();
             OpenFilesListPlusPanel(OpenFilesListPlus* _ofl_plugin, wxWindow* _parent, wxString _title, bool _bulk = false);
    virtual ~OpenFilesListPlusPanel();
};
/// ****************************************************************************
//! \class  OpenFilesListPlusPanelBulk
//!
//! \brief  Like OpenFilesListPlusPanel, but not deletable, not renamable,
//!     and containg the options menu
/// ****************************************************************************
class OpenFilesListPlusPanelBulk          : public OpenFilesListPlusPanel
{
  private:
    wxMenu      *   dw_menu_main;

    wxMenu      *       dw_menu_selection_mode;
    wxMenuItem  *           dw_item_selection_standard;
    wxMenuItem  *           dw_item_selection_productivity;

    //  ------------------------------------------------------------------------
  public:
             OpenFilesListPlusPanelBulk(OpenFilesListPlus* _ofl_plugin, wxWindow* _parent, wxString _title);
    virtual ~OpenFilesListPlusPanelBulk();
};

#endif                                                                          // __OFLP_PANEL_HH__

