/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PLUGIN_MODULE_HH__
#define __OFLP_PLUGIN_MODULE_HH__
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
    OflpModSettings         *   a_module_settings;

  protected:
    OpenFilesListPlus       *   oflp()      { return a_instance;        }
    OflpModLayout           *   layout()    { return a_module_layout;   }
    OflpModPanels           *   panels()    { return a_module_panels;   }
    OflpModGfx              *   gfx()       { return a_module_gfx;      }
    OflpModSettings         *   settings()  { return a_module_settings; }

  public:
    void                        init();
};

#endif
