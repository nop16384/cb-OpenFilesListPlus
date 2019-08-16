/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-common.hh"

#include    "oflp-plugin-module.hh"
#include    "oflp-plugin-mod-gfx.hh"
#include    "oflp-plugin-mod-layout.hh"
#include    "oflp-plugin-mod-panels.hh"
#include    "oflp-plugin-mod-settings.hh"
#include    "oflp-plugin-mod-runflow.hh"
#include    "oflp-plugin-mod-tooltips.hh"
#include    "oflp-plugin-mod-flog.hh"
//  ................................................................................................
void    OflpModule::init()
{
}

//  ................................................................................................
namespace   oflp
{

Modules     *   Modules::s_instance     =   nullptr;

//  shortened modules accessors
OFLP_MODULE_SHORTCUT_IMPL(gfx       , Gfx       );
OFLP_MODULE_SHORTCUT_IMPL(layout    , Layout    );
OFLP_MODULE_SHORTCUT_IMPL(panels    , Panels    );
OFLP_MODULE_SHORTCUT_IMPL(settings  , Settings  );
OFLP_MODULE_SHORTCUT_IMPL(runflow   , Runflow   );
OFLP_MODULE_SHORTCUT_IMPL(tooltips  , Tooltips  );
OFLP_MODULE_SHORTCUT_IMPL(flog      , FLog      );

Modules::Modules()
{
    d_gfx        =   new OflpModGfx(ConfigManager::GetDataFolder() + wxS("/images/"));
    d_layout     =   new OflpModLayout();
    d_panels     =   new OflpModPanels();
    d_settings   =   new OflpModSettings();
    d_runflow    =   new OflpModRunflow();
    d_tooltips   =   new OflpModTooltips();
    d_flog       =   new OflpModFLog();
}
Modules::~Modules()
{
}

void    Modules::init()
{
    d_gfx           ->OflpModule::init();
    d_layout        ->OflpModule::init();
    d_panels        ->OflpModule::init();
    d_settings      ->OflpModule::init();
    d_runflow       ->OflpModule::init();
    d_tooltips      ->OflpModule::init();
    d_flog          ->OflpModule::init();
}










}
