/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PANEL_HH__
#define __OFLP_PANEL_HH__
//  ............................................................................
#include    "oflp-common.hh"
//  ............................................................................
#include    <wx/dnd.h>                                                          //  for OpenFilesListPlusPanelDropTarget
//  ............................................................................
#include    <wx/dynarray.h>

#include    "oflp-panel-header.hh"
#include    "oflp-panel-utils.hh"
/// ****************************************************************************
//! \class  OpenFilesListPlusPanel
//!
//! \brief  wxPanel containing a OpenFilesListPlusPanelHeader and a wxTreeCtrl
/// ****************************************************************************
class OpenFilesListPlusPanel              : public wxPanel
{
    friend class    OpenFilesListPlus;
    friend class    OflpModPanels;
    friend class    OflpModEditors;
    //  ------------------------------------------------------------------------
  protected:
    bool                                a_bulk;
    bool                                a_allow_kill_focus_event;

    OflpPanelHeader                 *   dw_header;
    wxTreeCtrl                      *   d_tree;
    wxBoxSizer                      *   dw_sizer;

    EditorArray                         a_editors_array;                        //  speed up finding editors
    oflp::HString::HArray               a_harray;

    OpenFilesListPlusPanelDropTarget*   d_drop_target;
    //  ........................................................................
  public:
    bool                            is_bulk()                   const   { return a_bulk;                }
    EditorArray const   *           get_editors()               const   { return &a_editors_array;      }
    wxTreeCtrl  const   *   const   tree()                      const   { return d_tree;                }
    wxString    const   &           title()                     const   { return dw_header->title();    }
    //  ------------------------------------------------------------------------
    static      wxString            stringize_drag_result(wxDragResult _dres);
    //  ------------------------------------------------------------------------
  protected:
    void                    p0_create_tree      ();

    wxTreeItemId            item_append         (EditorBase*);                  //  x
    wxTreeItemId            item_find           (EditorBase*);                  //  x
    bool                    item_del            (EditorBase*);                  //  x
    bool                    item_select         (EditorBase*);
    bool                    item_selected       (EditorBase*);

    void                    items_deselect      ();
    void                    items_del           ();
    //  ------------------------------------------------------------------------
  protected:
    void                    reset();

    bool                    editor_add      (EditorBase*);                      //  x
    bool                    editor_del      (EditorBase*);                      //  x
    void                    editor_select   (EditorBase*);
    bool                    editor_selected (EditorBase*);
    void                    editor_sync     (EditorBase*);

    void                    editors_del     ();
    void                    editors_deselect();

    wxButton        *       button(int _ix);
    wxBitmapButton  *       bitmap_button(int _ix);

    void                    minimize();
    void                    maximize();
    //  ........................................................................
  public:
    void                    dump();

    bool                    editor_has                      (EditorBase*);
    bool                    is_minimized                    ();

    EditorBase  *           editor_from_absolute_filepath   (wxString const & _absolute_filepath);
    //  ------------------------------------------------------------------------
  public:
    void                    set_bg      (wxColour& _c);
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
    //  ------------------------------------------------------------------------
  public:
             OpenFilesListPlusPanelBulk(OpenFilesListPlus* _ofl_plugin, wxWindow* _parent, wxString _title);
    virtual ~OpenFilesListPlusPanelBulk();
};
/*
/// ****************************************************************************
//! \class  OpenFilesListPlusPanelOptions
//!
//! \brief  Options for the plugin
/// ****************************************************************************
class OpenFilesListPlusPanelOptions       : public wxPanel
{
  private:
    OFLPHeader      *   dw_header;
    wxBoxSizer      *   dw_sizer;

  public:
             OpenFilesListPlusPanelOptions(OpenFilesListPlus* _ofl_plugin, wxWindow* _parent);
    virtual ~OpenFilesListPlusPanelOptions();
};
*/
#endif                                                                          // __OFLP_PANEL_HH__

