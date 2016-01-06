/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

/*******************************************************************************
 * Name:      cb-oflp-plugin
 * Purpose:   Code::Blocks plugin
 * Author:     ()
 * Created:   2015-08-23
 * Copyright:
 * License:   GPL
 ******************************************************************************/
#ifndef __OFLP_PLUGIN_HH__
#define __OFLP_PLUGIN_HH__
//  ............................................................................
#include    "oflp-common.hh"                                                    //  standard wx includes + OFLP log system
/// ****************************************************************************
//! \class  OpenFilesListPlus
//!
//! \brief
/// ****************************************************************************
class OpenFilesListPlus : public cbPlugin
{
        //  ####################################################################
        //                      BOILERPLATE STUFF
        //  ####################################################################
    public:
        /** Constructor. */
        OpenFilesListPlus();
        /** Destructor. */
        virtual ~OpenFilesListPlus();

        /** Invoke configuration dialog. */
        virtual int Configure();

        /** Return the plugin's configuration priority.
          * This is a number (default is 50) that is used to sort plugins
          * in configuration dialogs. Lower numbers mean the plugin's
          * configuration is put higher in the list.
          */
        virtual int GetConfigurationPriority() const { return 50; }

        /** Return the configuration group for this plugin. Default is cgUnknown.
          * Notice that you can logically OR more than one configuration groups,
          * so you could set it, for example, as "cgCompiler | cgContribPlugin".
          */
        //virtual int GetConfigurationGroup() const { return cgUnknown; }

        /** Return plugin's configuration panel.
          * @param parent The parent window.
          * @return A pointer to the plugin's cbConfigurationPanel. It is deleted by the caller.
          */
        virtual cbConfigurationPanel* GetConfigurationPanel(wxWindow* parent){ return 0; }

        /** Return plugin's configuration panel for projects.
          * The panel returned from this function will be added in the project's
          * configuration dialog.
          * @param parent The parent window.
          * @param project The project that is being edited.
          * @return A pointer to the plugin's cbConfigurationPanel. It is deleted by the caller.
          */
        virtual cbConfigurationPanel* GetProjectConfigurationPanel(wxWindow* parent, cbProject* project){ return 0; }

        /** This method is called by Code::Blocks and is used by the plugin
          * to add any menu items it needs on Code::Blocks's menu bar.\n
          * It is a pure virtual method that needs to be implemented by all
          * plugins. If the plugin does not need to add items on the menu,
          * just do nothing ;)
          * @param menuBar the wxMenuBar to create items in
          */
        virtual void BuildMenu(wxMenuBar* menuBar);

        /** This method is called by Code::Blocks core modules (EditorManager,
          * ProjectManager etc) and is used by the plugin to add any menu
          * items it needs in the module's popup menu. For example, when
          * the user right-clicks on a project file in the project tree,
          * ProjectManager prepares a popup menu to display with context
          * sensitive options for that file. Before it displays this popup
          * menu, it asks all attached plugins (by asking PluginManager to call
          * this method), if they need to add any entries
          * in that menu. This method is called.\n
          * If the plugin does not need to add items in the menu,
          * just do nothing ;)
          * @param type the module that's preparing a popup menu
          * @param menu pointer to the popup menu
          * @param data pointer to FileTreeData object (to access/modify the file tree)
          */
        virtual void BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data = 0);

        /** This method is called by Code::Blocks and is used by the plugin
          * to add any toolbar items it needs on Code::Blocks's toolbar.\n
          * It is a pure virtual method that needs to be implemented by all
          * plugins. If the plugin does not need to add items on the toolbar,
          * just do nothing ;)
          * @param toolBar the wxToolBar to create items on
          * @return The plugin should return true if it needed the toolbar, false if not
          */
        virtual bool BuildToolBar(wxToolBar* toolBar);
    protected:
        /** Any descendent plugin should override this virtual method and
          * perform any necessary initialization. This method is called by
          * Code::Blocks (PluginManager actually) when the plugin has been
          * loaded and should attach in Code::Blocks. When Code::Blocks
          * starts up, it finds and <em>loads</em> all plugins but <em>does
          * not</em> activate (attaches) them. It then activates all plugins
          * that the user has selected to be activated on start-up.\n
          * This means that a plugin might be loaded but <b>not</b> activated...\n
          * Think of this method as the actual constructor...
          */
        virtual void OnAttach();

        /** Any descendent plugin should override this virtual method and
          * perform any necessary de-initialization. This method is called by
          * Code::Blocks (PluginManager actually) when the plugin has been
          * loaded, attached and should de-attach from Code::Blocks.\n
          * Think of this method as the actual destructor...
          * @param appShutDown If true, the application is shutting down. In this
          *         case *don't* use Manager::Get()->Get...() functions or the
          *         behaviour is undefined...
          */
        virtual void OnRelease(bool appShutDown);
        //  ####################################################################
        //                      CORE
        //  ####################################################################
      private:
        static  OpenFilesListPlus   *   s_singleton;
        wxMenu                      *   aw_menu_view;

      public:
        static  OpenFilesListPlus   *   Instance()  { return s_singleton; }
        //  ====================================================================
      protected:
        bool    a_mode_degraded;
        //  ====================================================================
        friend class OflpModSettings;                                           //  cf bugs#18

      protected:
        void    reset();
        void    degrade();
        bool    degraded();

      protected:
        void    RefreshOpenFileState        (EditorBase* _edb);
        void    RefreshOpenFileLayout       (EditorBase* _edb);
        void    RefreshOpenFilesLayout      ();

      public:
        void    emergency();

        void    dump_project_manager_state  ();

        bool    FindCbProjectForFile        (wxString const & _abs_fpath, cbProject** _pro, ProjectFile** _pjf);
        //  ####################################################################
        //                      EVENTS
        //  ####################################################################
      public:
        void    evh_view_open_files_plus        (wxCommandEvent& event);
        void    evh_update_ui                   (wxUpdateUIEvent& event);

        void    evh_tree_item_activated         (wxTreeEvent& event);
        void    evh_tree_item_right_click       (wxTreeEvent& event);
        void    evh_tree_sel_changed            (wxTreeEvent &event);

        void    evh_editor_opened               (CodeBlocksEvent& event);
        void    evh_editor_closed               (CodeBlocksEvent& event);
        void    evh_editor_activated            (CodeBlocksEvent& event);
        void    evh_editor_deactivated          (CodeBlocksEvent& event);
        void    evh_editor_modified             (CodeBlocksEvent& event);
        void    evh_editor_saved                (CodeBlocksEvent& event);

        void    evh_workspace_loading_complete  (CodeBlocksEvent& event);
        void    evh_workspace_close_begin       (CodeBlocksEvent& event);
        void    evh_workspace_close_complete    (CodeBlocksEvent& event);
        void    evh_project_open                (CodeBlocksEvent& event);
        void    evh_project_close               (CodeBlocksEvent& event);
        void    evh_project_save                (CodeBlocksEvent& event);

        void    evh_panel_header_button_clicked_add     (wxCommandEvent &);
        void    evh_panel_header_button_clicked_del     (wxCommandEvent &);
        void    evh_panel_header_button_clicked_mm      (wxCommandEvent &);
        void    evh_panel_header_button_clicked_up      (wxCommandEvent &);
        void    evh_panel_header_button_clicked_down    (wxCommandEvent &);
        void    evh_panel_header_button_clicked_opt     (wxCommandEvent &);

        void    evh_settings_activated                  (wxActivateEvent& _e);
        //  ####################################################################
        //                      DRAG N DROP
        //  ####################################################################
      private:
        OflpPanel           *   a_dnd_panel_src;
        OflpPanel           *   a_dnd_panel_dst;
        EditorBase          *   a_dnd_editor_base;

      public:
        OflpPanel   const   *   dnd_panel_src() { return a_dnd_panel_src;   }
        OflpPanel   const   *   dnd_panel_dst() { return a_dnd_panel_dst;   }
        EditorBase          *   dnd_editor()    { return a_dnd_editor_base; }

      public:
        void                    dnd_panel_src_set   (OflpPanel* _panel)
            {
                a_dnd_panel_src = _panel;
            }
        void                    dnd_panel_dst_set   (OflpPanel* _panel)
            {
                a_dnd_panel_dst = _panel;
            }
        void                    dnd_editor_set      (EditorBase* _edb)
            {
                a_dnd_editor_base = _edb;
            }

        void                    editor_drag_and_dropped();
        //  ====================================================================
        //  modules
        #define OFPL_MEMBER_MODULE(MODULE_NAME, MEMBER_NAME, GET_NAME)          \
          private:                                                              \
            MODULE_NAME *   MEMBER_NAME;                                        \
          public:                                                               \
            MODULE_NAME *   GET_NAME()                                          \
                {                                                               \
                    return MEMBER_NAME;                                         \
                }
        OFPL_MEMBER_MODULE(OflpModGfx           , d_gfx             , gfx       );
        OFPL_MEMBER_MODULE(OflpModLayout        , d_layout          , layout    );
        OFPL_MEMBER_MODULE(OflpModEditors       , d_editors         , editors   );
        OFPL_MEMBER_MODULE(OflpModPanels        , d_panels          , panels    );
        OFPL_MEMBER_MODULE(OflpModSettings      , d_settings        , settings  );
        //  ====================================================================
    private:
        DECLARE_EVENT_TABLE();
};

#endif                                                                          // __OFLP_PLUGIN_HH__
