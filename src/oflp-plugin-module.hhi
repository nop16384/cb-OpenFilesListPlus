/// ****************************************************************************
//! \class  OflpModule
//!
//! \brief  OpenFilesListPlus is splitted in many classes called
//!     modules, for code lisibility. Each module inherits from class
//!     OflpModule, which allows inter-module & OpenFilesListPlus
//!     instance access.
/// ****************************************************************************
class OflpModule
{
  private:
    OpenFilesListPlus       *   a_instance;
    OflpModGfx              *   a_module_gfx;
    OflpModLayout           *   a_module_layout;
    OflpModPanels           *   a_module_panels;
    OflpModEditors          *   a_module_editors;
    OflpModMenuOptions      *   a_module_menu_options;

  protected:
    OpenFilesListPlus       *   oflp()      { return a_instance;        }
    OflpModLayout           *   layout()    { return a_module_layout;   }
    OflpModPanels           *   panels()    { return a_module_panels;   }
    OflpModGfx              *   gfx()       { return a_module_gfx;      }

  public:
    void                        init();
};
