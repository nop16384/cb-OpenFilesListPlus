/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PLUGIN_MODULE_HH__
#define __OFLP_PLUGIN_MODULE_HH__

#include    "oflp-common-defines.hh"

namespace   oflp
{

class   Modules
{
    friend class    ::OpenFilesListPlus;

    #define OFPL_MODULES_MODULE(MODULE_NAME, MEMBER_NAME, GET_NAME)         \
      private:                                                              \
        MODULE_NAME *   MEMBER_NAME;                                        \
      public:                                                               \
        MODULE_NAME *   GET_NAME()                                          \
            {                                                               \
                return MEMBER_NAME;                                         \
            }
    OFPL_MODULES_MODULE(OflpModGfx          , d_gfx             , gfx       );
    OFPL_MODULES_MODULE(OflpModLayout       , d_layout          , layout    );
    OFPL_MODULES_MODULE(OflpModPanels       , d_panels          , panels    );
    OFPL_MODULES_MODULE(OflpModSettings     , d_settings        , settings  );
    OFPL_MODULES_MODULE(OflpModRunflow      , d_runflow         , runflow   );
    OFPL_MODULES_MODULE(OflpModTooltips     , d_tooltips        , tooltips  );
    OFPL_MODULES_MODULE(OflpModFLog         , d_flog            , flog      );
    //  --------------------------------------------------------------------------------------------
  private:
    static  Modules *   s_instance;
  public:
    static  Modules *   Instance()
                        {
                            return s_instance;
                        }
    //  --------------------------------------------------------------------------------------------
  private:
    void    modinit();
    void    modrelease();
    //  --------------------------------------------------------------------------------------------
  public:
                Modules();
    virtual    ~Modules();
};

//  shortened modules accessors
#define OFLP_MODULE_SHORTCUT_DECL(MODNAME, MODNAMEMAJ)                                              \
    OflpMod##MODNAMEMAJ     *   mod##MODNAMEMAJ();

#define OFLP_MODULE_SHORTCUT_IMPL(MODNAME, MODNAMEMAJ)                                              \
    OflpMod##MODNAMEMAJ     *   mod##MODNAMEMAJ()   { return oflp::Modules::Instance()->MODNAME(); }

OFLP_MODULE_SHORTCUT_DECL(gfx       , Gfx);
OFLP_MODULE_SHORTCUT_DECL(layout    , Layout    );
OFLP_MODULE_SHORTCUT_DECL(panels    , Panels    );
OFLP_MODULE_SHORTCUT_DECL(settings  , Settings  );
OFLP_MODULE_SHORTCUT_DECL(runflow   , Runflow   );
OFLP_MODULE_SHORTCUT_DECL(tooltips  , Tooltips  );
OFLP_MODULE_SHORTCUT_DECL(flog      , FLog      );

}

/// ************************************************************************************************
//! \class  OflpModule
//!
//! \brief  OpenFilesListPlus is splitted in many classes called
//!     modules, for code lisibility. Each module inherits from class
//!     OflpModule, which allows inter-module & OpenFilesListPlus
//!     instance access.
/// ************************************************************************************************
class OflpModule
{
    friend class oflp::Modules;

  private:
    virtual     void            modinit()                                                       = 0;
    virtual     void            modrelease()                                                    = 0;
};

#endif
