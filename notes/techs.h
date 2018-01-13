 (001)  2018.01.03     In OflpPanelDropTarget::OnData(wxCoord, wxCoord, wxDragResult), deleting
        the wxTreeItem from the source will cause a SELECTION_CHANGE event with a NULL data, because
        the wxTReeItem was deleted
