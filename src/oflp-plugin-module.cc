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
void    OflpModule::modinit()
{
    modinit();
}
void    OflpModule::modrelease()
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

void    Modules::modinit()
{
    d_gfx           ->OflpModule::modinit();
    d_layout        ->OflpModule::modinit();
    d_panels        ->OflpModule::modinit();
    d_settings      ->OflpModule::modinit();
    d_runflow       ->OflpModule::modinit();
    d_tooltips      ->OflpModule::modinit();
    d_flog          ->OflpModule::modinit();
}
void    Modules::modrelease()
{
    d_gfx           ->OflpModule::modrelease();
    d_layout        ->OflpModule::modrelease();
    d_panels        ->OflpModule::modrelease();
    d_settings      ->OflpModule::modrelease();
    d_runflow       ->OflpModule::modrelease();
    d_tooltips      ->OflpModule::modrelease();
    d_flog          ->OflpModule::modrelease();
}










}
