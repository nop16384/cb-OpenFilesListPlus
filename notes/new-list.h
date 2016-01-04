earlgrey-cb.log.cc|40|      dw_log_window   = new LogWindow( _parent, _T(" OpenFilesList"), true, false );|

oflp-panel.cc|273|          new OflpPanelItemData(this, _editor)   );|

oflp-plugin-gfx.hhi|23|     a_bitmap_hash[_hash_key] = new wxBitmap( _bmp );|
oflp-plugin-gfx.hhi|67|     d_img_list  =   new wxImageList(16, 16);|
oflp-plugin-gfx.hhi|90|     d_fnt_8 =   new wxFont(8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxString::FromUTF8("monospace") );|

oflp-plugin-menus.hhi|76|   dw_m1_mode = new wxMenu();|
oflp-plugin-menus.hhi|84|   dw_m1_log = new wxMenu();|

oflp-plugin-panels.cci|63|  new OpenFilesListPlusPanelBulk( this, dw_MainPanel, _title )   :|
oflp-plugin-panels.cci|64|  new OpenFilesListPlusPanel( this, dw_MainPanel, _title )       ;|

oflp-plugin.cpp|72|         d_gfx = new Gfx(ConfigManager::GetDataFolder() + _T("/images/"));|
oflp-plugin.cpp|75|         dw_menu_options =   new MenuOptions();|
oflp-plugin.cpp|93|         dw_MainPanel    =   new wxPanel(Manager::Get()->GetAppWindow(), idMainPanel);|
oflp-plugin.cpp|95|         dw_MainSizer    =   new wxBoxSizer(wxVERTICAL);|
oflp-plugin.cpp|116|//pm->RegisterEventSink(cbEVT_PROJECT_OPEN, new cbEventFunctor<OpenFilesListPlugin, CodeBlocksEvent>(this, &OpenFilesListPlugin::OnProjectOpened));|
oflp-plugin.cpp|117|//pm->RegisterEventSink(cbEVT_PROJECT_ACTIVATE, new cbEventFunctor<OpenFilesListPlugin, CodeBlocksEvent>(this, &OpenFilesListPlugin::OnProjectOpened));|
oflp-plugin.cpp|118|pm->RegisterEventSink(cbEVT_EDITOR_OPEN     , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_editor_opened    ));|
oflp-plugin.cpp|119|pm->RegisterEventSink(cbEVT_EDITOR_CLOSE    , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_editor_closed    ));|
oflp-plugin.cpp|120|pm->RegisterEventSink(cbEVT_EDITOR_ACTIVATED, new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_editor_activated ));|
oflp-plugin.cpp|121|pm->RegisterEventSink(cbEVT_EDITOR_MODIFIED , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_editor_modified  ));|
oflp-plugin.cpp|122|pm->RegisterEventSink(cbEVT_EDITOR_SAVE     , new cbEventFunctor<OpenFilesListPlus, CodeBlocksEvent>(this, &OpenFilesListPlus::evh_editor_saved     ));|
oflp-plugin.cpp|123|//pm->RegisterEventSink(cbEVT_EDITOR_DEACTIVATED, new cbEventFunctor<OpenFilesListPlugin, CodeBlocksEvent>(this, &OpenFilesListPlugin::OnEditorDeactivated));|
//  ############################################################################
oflp-panel-header.cci|35|   dw_txt_dyn = new wxTextCtrl( this, wxNewId(), a_title );|
oflp-panel-header.cci|64|   dw_txt_sta  = new wxStaticText( this, wxNewId(), a_title );|
oflp-panel-header.cci|157|  dw_txt_sta      =   new wxStaticText( this, wxNewId(), _title );            //  at fist display, static text displayed|

oflp-panel-header.cci|98|   bt  = new wxBitmapButton(this, wxNewId()                                ,|
oflp-panel-header.cci|119|  bt  = new wxBitmapButton(this, wxNewId()                                ,|

oflp-panel-header.cci|155|  dw_sizer        =   new wxBoxSizer(wxHORIZONTAL);|
//  ############################################################################
oflp-panel-utils.cci|192|   d_data_object   =   new OflpPanelDataObject();|
//  ############################################################################
oflp-panel.cc|156|          d_tree          =   new wxTreeCtrl( this, wxNewId(), wxDefaultPosition, wxSize(150, 100),|
oflp-panel.cc|175|          d_drop_target   =   new OpenFilesListPlusPanelDropTarget(d_tree, this);|
oflp-panel.cc|194|          dw_header   =   new OpenFilesListPlusPanelHeader( this, _title );|
oflp-panel.cc|223|          dw_sizer    =   new wxBoxSizer(wxVERTICAL);|
oflp-panel.cc|451|          dw_header   =   new OpenFilesListPlusPanelHeader( this, _title );|
oflp-panel.cc|468|          dw_sizer    =   new wxBoxSizer(wxVERTICAL);|
