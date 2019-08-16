/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PANEL_UTILS_HH__
#define __OFLP_PANEL_UTILS_HH__

#include    "oflp-common-classes.hh"

/// ************************************************************************************************
//! \class  OpenFilesListPlusPanelTreeItemData
//!
//! \brief  Data stored in all tree items of panels
//!
//! \detail Points on the OflpPanel, and the EditorBase
/// ************************************************************************************************
class OpenFilesListPlusPanelTreeItemData    : public wxTreeItemData
{
  public:
    OpenFilesListPlusPanelTreeItemData(
        OpenFilesListPlusPanel      *   _panel  =   nullptr,
        EditorBase                  *   _ed     =   nullptr);

  protected:
    EditorBase                  *   a_editor;
    OpenFilesListPlusPanel      *   a_panel;

  protected:
    bool                                    IsOk()                  { return ( ( a_editor != NULL ) && ( a_panel != NULL ) ); }

  public:
    EditorBase                          *   x_get_editor()  const   { return a_editor;          }
    OpenFilesListPlusPanel              *   x_get_panel()   const   { return a_panel;           }
};
/// ************************************************************************************************
//! \class  OpenFilesListPlusPanelTreeItemEventInfo
//!
//! \brief  Data stored in all tree items of panels + other datas  ; construct
//!     only from a wxTeeeEvent : its only for event handling coding lisibility
//!
//! \detail Points on the OflpPanel, the EditorBase, has the wxTeeeItemId, and
//!     the wxTreeCtrl
/// ************************************************************************************************
class OpenFilesListPlusPanelTreeItemEventInfo   : public OpenFilesListPlusPanelTreeItemData
{
  public:
    enum eOkType
    {
        eOkAll          =   0x00    ,
        eOkExceptData   =   0x01    ,
    };

  public:
    OpenFilesListPlusPanelTreeItemEventInfo(wxTreeEvent& _e);

  private:
    wxTreeCtrl                  *   a_tree;
    wxTreeItemId                    a_iid;

  public:
    virtual bool                    IsOk(eOkType _type = eOkAll)
                                    {
                                        if ( _type == eOkAll )          return ( ( a_tree != NULL ) && ( a_iid.IsOk() ) && OflpPanelTiData::IsOk() );
                                        if ( _type == eOkExceptData )   return ( ( a_tree != NULL ) && ( a_iid.IsOk() ) );
                                        return false;                                               // GWR_TEK g++ does not know this statement is never reached
                                    }
    wxTreeCtrl                  *   x_get_tree()        { return a_tree;    }
    wxTreeItemId                    x_get_item_id()     { return a_iid;     }
};

#endif
