/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

/// ****************************************************************************
//! \class  OpenFilesListPlusPanelItemData
//!
//! \brief  Data stored in all tree items of panels
//!
//! \detail Points on the OflpPanel, and the EditorBase
/// ****************************************************************************
class OpenFilesListPlusPanelItemData      : public wxTreeItemData
{
  public:
    OpenFilesListPlusPanelItemData(
        OpenFilesListPlusPanel      *   _panel  ,
        EditorBase                  *   _ed     );

  protected:
    EditorBase                  *   a_editor;
    OpenFilesListPlusPanel      *   a_panel;

  protected:
    virtual void                    p1_raz()    { a_editor = NULL; a_panel = NULL; }

  public:
    EditorBase                          *   GetEditor()     const   { return a_editor;          }
    OpenFilesListPlusPanel              *   GetPanel()      const   { return a_panel;           }
};
/// ****************************************************************************
//! \class  OpenFilesListPlusPanelItemInfo
//!
//! \brief  Data stored in all tree items of panels + other datas  ; construct
//!     only from a wxTeeeEvent : its only for event handling coding lisibility
//!
//! \detail Points on the OflpPanel, the EditorBase, has the wxTeeeItemId, and
//!     the wxTreeCtrl
/// ****************************************************************************
class OpenFilesListPlusPanelItemInfo      : public OpenFilesListPlusPanelItemData
{
  public:
    OpenFilesListPlusPanelItemInfo(wxTreeEvent& _e);

  private:
    bool                            a_is_ok;
    wxTreeCtrl                  *   a_tree;
    wxTreeItemId                    a_iid;

  protected:
    virtual void                    p1_raz()    { a_tree = NULL; a_iid.Unset(); }

  public:
    bool                            IsOk()      { return a_is_ok;   }
    wxTreeCtrl                  *   GetTree()   { return a_tree;    }
    wxTreeItemId                    GetIid()    { return a_iid;     }
};
/// ****************************************************************************
//! \class  OflpPanelDataObject
//!
//! \brief  Allow parameters sharing between DnD src and dst widgets
//!
//! \detail We inherit from wxDataObjectSimple, and serialize / deserialize some
//!     values.
/// ****************************************************************************
class OflpPanelDataObject                   : public wxDataObjectSimple
{
  public:
    OflpPanelDataObject();

  private:
    static  const   size_t  s_capacity          =   64;
    static  const   size_t  s_panel_index_max   =   255;
    static  const   size_t  s_item_index_max    =   255;

    //  serialized data for dnd
    char    a_data[s_capacity];
    size_t  a_data_size;                                                        //! a_data_size = 0 <=> no data stored in object

  public:
    //  ( wxWidgets would say ) "formatted" data for OflpPanel
    unsigned    char    a_data_panel_index;                                     //! formatted OflpPanel index
    unsigned    char    a_data_item_index;                                      //! formatted item index in above OflpPanel
    EditorBase  *       a_data_editor;                                          //! formatted EditorBase*

  private:
    bool                p0_serialize();
    bool                p0_deserialize();
    EditorBase  **      p0_serialized_editor_pointer()
        {
            return  (EditorBase**)( a_data + 2 );
        }

  public:
    virtual size_t  GetDataSize ()                              const;
    virtual bool    GetDataHere (void *buf)                     const;
    virtual bool    SetData     (size_t len, const void *buf);

            int             GetDataCapacity ()  { return s_capacity; }
            int             GetDataPanelIx  ();
            int             GetDataItemIx   ();
            EditorBase  *   GetDataEditor   ();
            bool            SetData         (int _panel_ix, int _item_ix, EditorBase*);
            void            ClrData         ()  { a_data_size = 0; }
};
/// ****************************************************************************
//! \class  OpenFilesListPlusPanelDropTarget
//!
//! \brief  For Dnd
/// ****************************************************************************
class OpenFilesListPlusPanelDropTarget    : public wxDropTarget
{
  private:
    wxTreeCtrl              *   a_owner;
    OflpPanel               *   a_owner_panel;
    OflpPanelDataObject     *   d_data_object;

  public:
    wxTreeCtrl              *   octrl()     { return a_owner;       }
    OflpPanel               *   opanel()    { return a_owner_panel; }

  public:
    virtual bool            OnDrop(wxCoord x, wxCoord y);
    virtual wxDragResult    OnData(wxCoord x, wxCoord y, wxDragResult _res);

  public:
    OpenFilesListPlusPanelDropTarget(wxTreeCtrl *_owner, OflpPanel* _owner_panel);
   ~OpenFilesListPlusPanelDropTarget();
};
