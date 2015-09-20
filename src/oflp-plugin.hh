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
//  ............................................................................
#include    <wx/imaglist.h>                                                     //  for OFPLPlugin::Gfx
#include    <wx/xrc/xmlres.h>                                                    //  for OFPLPlugin::Gfx
#include    <wx/artprov.h>                                                      //  for OFPLPlugin::Gfx
//  ............................................................................
#include    "cbplugin.h"                                                        //  for "class cbPlugin"
#include    "cbworkspace.h"                                                        //  for "class cbPlugin"
/// ****************************************************************************
//! \class  OpenFilesListPlus
//!
//! \brief
/// ****************************************************************************
class OpenFilesListPlus : public cbPlugin
{
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
        virtual int GetConfigurationGroup() const { return cgUnknown; }

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
        //                      PLUGIN STUFF
        //  ####################################################################
      private:
        static  OpenFilesListPlus   *   s_singleton;

      public:
        static  OpenFilesListPlus   *   Instance()  { return s_singleton; }
        //  ====================================================================
    private:
        class   Gfx;
        class   Layout;
        class   Panels;
        class   Actions;
        class   MenuOptions;
        /// ********************************************************************
        //! \class  Module
        //!
        //! \brief  OpenFilesListPlus is splitted in many inner-classes called
        //!     modules, for code lisibility. Each module inherits from inner
        //!     class Module, which allows inter-module & OpenFilesListPlus
        //!     instance access.
        /// ********************************************************************
      private:
        class  Module
        {
            friend class    OpenFilesListPlus;

          private:
            OpenFilesListPlus               *   a_instance;
            OpenFilesListPlus::Gfx          *   a_module_gfx;
            OpenFilesListPlus::Layout       *   a_module_layout;
            OpenFilesListPlus::Panels       *   a_module_panels;
            OpenFilesListPlus::Actions      *   a_module_actions;
            OpenFilesListPlus::MenuOptions  *   a_module_menu_options;

          protected:
            OpenFilesListPlus           *   oflp()      { return a_instance;        }
            OpenFilesListPlus::Layout   *   layout()    { return a_module_layout;   }
            OpenFilesListPlus::Panels   *   panels()    { return a_module_panels;   }

        };
        //  ....................................................................
        #define OFPL_MEMBER_MODULE(MODULE_NAME, MEMBER_NAME, GET_NAME)          \
          private:                                                              \
            MODULE_NAME *   MEMBER_NAME;                                        \
          public:                                                               \
            MODULE_NAME *   GET_NAME()                                          \
                {                                                               \
                    return MEMBER_NAME;                                         \
                }
        //  ....................................................................
      private:
        void    module_init(Module* _module)
            {
                _module->a_instance             =   this;

                _module->a_module_gfx           =   this->gfx();
                _module->a_module_layout        =   this->layout();
                _module->a_module_panels        =   this->panels();
                //_module->a_module_actions       =   this->actions();
                _module->a_module_menu_options  =   this->menu_options();
            }
        //  ====================================================================
        friend class OpenFilesListPlusPanel;                                    //  _GWR_TODO_ only for evh handler
        friend class OpenFilesListPlusPanelBulk;                                //  ...remove friend stuff

      protected:
        void    reset();

        void    AddAllOpenFiles         ();
        void    RefreshOpenFileState    (EditorBase* _edb, bool _remove = false);
        void    RefreshOpenFileLayout   (EditorBase* _edb);
        void    RefreshOpenFilesLayout  ();
        //  ====================================================================
        WX_DEFINE_ARRAY(OpenFilesListPlusPanel  *, PanelArray);
        //  ====================================================================
        //  non-modularized stuff
        #include    "oflp-plugin-events.hhi"
        #include    "oflp-plugin-dnd.hhi"
        //  ====================================================================
        //  modularized stuff
        OFPL_MEMBER_MODULE(Gfx          , d_gfx             , gfx           );
        OFPL_MEMBER_MODULE(MenuOptions  , dw_menu_options   , menu_options  );
        OFPL_MEMBER_MODULE(Layout       , d_layout          , layout        );
        OFPL_MEMBER_MODULE(Actions      , d_actions         , actions       );
        OFPL_MEMBER_MODULE(Panels       , d_panels          , panels        );
        #include    "oflp-plugin-gfx.hhi"
        #include    "oflp-plugin-menus.hhi"
        #include    "oflp-plugin-panels.hhi"
        #include    "oflp-plugin-layout.hhi"
        #include    "oflp-plugin-actions.hhi"
        //  ====================================================================
    private:
        DECLARE_EVENT_TABLE();
};

#endif                                                                          // __OFLP_PLUGIN_HH__
