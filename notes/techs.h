(000)   2019.07.23      Defines are repartided as this :

    Compiler                        oflp-common-defines.hh

    OFLP_LINUX
    OFLP_MS

                                    OFLP_LOG
                                        OFLP_LOG_DND
                                        OFLP_LOG_FUNCTIONS

                                    OFLP_DEBUG_MOD_LAYOUT
                                    OFLP_DEBUG_MOD_PANELS
                                    OFLP_DEBUG_PANELS


(001)   2018.01.03      In OflpPanelDropTarget::OnData(wxCoord, wxCoord, wxDragResult), deleting
        the wxTreeItem from the source will cause a SELECTION_CHANGE event with a NULL data, because
        the wxTreeItem was deleted

(002)   2018.01.28      We are forced to synchronize the FileAssignments when DnD, because of
        intermediate calls to SyncToLayout(). Else we could just have saved the final state of the
        layout when closing project / workspace / wholle C::B app.
