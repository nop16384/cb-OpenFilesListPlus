/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

/// ************************************************************************************************
//! \class  OflpPanelDndDataObject
//!
//! \brief  Allow parameters sharing between DnD src and dst widgets
//!
//! \detail We inherit from wxDataObjectSimple, and serialize / deserialize some
//!     values.
/// ************************************************************************************************
class OflpPanelDndDataObject    : public wxDataObjectSimple
{
  private:
    //  some constants
    static  const   size_t  s_capacity          =   64;
    static  const   size_t  s_panel_index_max   =   255;
    static  const   size_t  s_item_index_max    =   10000;

  public:
    OflpPanelDndDataObject();

  public:
    //  dnd serialized data
    char    a_data[s_capacity];
    size_t  a_data_size;                                                                            //! a_data_size = 0 <=> no data stored in object

  public:
    //  dnd unserialized data
    int                 a_data_panel_visual_index;                                                  //! OflpPanel DndSrc index
    int                 a_data_item_visual_index;                                                   //! dragged item index in above OflpPanel
    EditorBase  *       a_data_editor;                                                              //! dragged EditorBase*
    int         *       a_data_p1;
    int         *       a_data_p2;
    EditorBase  **      a_data_p3;

  private:
    bool                z_serialize();
    bool                z_deserialize();

  public:
    virtual size_t  GetDataSize ()                              const;
    virtual bool    GetDataHere (void *buf)                     const;
    virtual bool    SetData     (size_t len, const void *buf);
            bool    SetData     (int _panel_ix, int _item_ix, EditorBase*);

            size_t          x_data_capacity ()  { return s_capacity;                }
            int             x_data_panel_vix()  { return a_data_panel_visual_index; }
            int             x_data_item_vix ()  { return a_data_item_visual_index;  }
            EditorBase  *   x_data_editor   ()  { return a_data_editor;             }
            void            x_data_clear    ()  { memset(a_data, 0, s_capacity); a_data_size = 0;   }
};
/// ************************************************************************************************
//! \class  OpenFilesListPlusPanelDropTarget
//!
//! \brief  For Dnd
/// ************************************************************************************************
class OpenFilesListPlusPanelDropTarget    : public wxDropTarget
{
  private:
    wxTreeCtrl              *   a_owner_tree;
    OflpPanel               *   a_owner_panel;
    OflpPanelDndDataObject  *   d_data_object;

  public:
    wxTreeCtrl              *   octrl()     { return a_owner_tree;  }
    OflpPanel               *   opanel()    { return a_owner_panel; }

  public:
    virtual bool            OnDrop(wxCoord x, wxCoord y);
    virtual wxDragResult    OnData(wxCoord x, wxCoord y, wxDragResult _res);

  public:
    OpenFilesListPlusPanelDropTarget(wxTreeCtrl *_owner, OflpPanel* _owner_panel);
   ~OpenFilesListPlusPanelDropTarget();
};
