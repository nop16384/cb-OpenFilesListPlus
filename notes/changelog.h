//  ============================================================================
2015.09.20
- Added panels resize following the # of editors in them
2015.09.26
- Corrected a bug in panel maximizing
- Better panel sizing
2016.01.07
- created OflpModRunflow class
- TKI editor name on each editor event
- 2 configurations for OpenFilesListPlusSettings because wxFrame behaves
  differently on linux & ms
2018.01.04
- proper wxWidgets DnD ( GetData() call forgotten ! )
- renamed OpenFilesListPlusPanelItemData in OpenFilesListPlusPanelTiData
- renamed OpenFilesListPlusPanelItemInfo in OpenFilesListPlusPanelTiEventInfo
2019.11.10
- re-added layout synchro in oflp-plugin-dnd.c, because incessant calls to SyncEditorsToLayout()
  used to move back editors in bulk panel when closing projects / workspace, which break the layout
  saving process.
- moved compilation environment variables from 'envvars' to 'per-project variables'
- removed member a_harray member of type oflp::HString::Harray from OflpPanel class since it was not used
