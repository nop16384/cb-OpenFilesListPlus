/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    <wx/tooltip.h>

#include    "oflp-common.hh"
#include    "oflp-common-macros.hh"

#include    "oflp-plugin.hh"

#include    "oflp-plugin-mod-settings.hh"
#include    "oflp-plugin-mod-tooltips.hh"
//  ................................................................................................
#define     ERG_OFLP_SANITY_CHECKS
#include    "generated/oflp-plugin-mod-tooltips--log-defines.cci"
//  ################################################################################################
//
//                          SETTINGS
//
//  ################################################################################################
void    OflpModTooltips::   z_dump()
{
    //  00 01 02 03 04
    //  xx -- xx xx --
    //  ............................................................................................
    size_t              i   =   0;
    TooltipInfoList *   til =   nullptr;
    char                tmp     [256];
    std::string         s1,s2;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::ModTooltips::dump()");
    //  ............................................................................................
    //  ............................................................................................
    ERG_TKI("size[%zu] capacity[%zu]", a_slots.size(), a_slots.capacity());
    //  ............................................................................................
    for ( i = 0 ; i != a_slots.size() ; i++ )
    {
        til = a_slots[i];

        sprintf(tmp, "%02zu ", i);
        s1.append(tmp);

        if ( til )
        {
            sprintf(tmp, "xx ");
            s2.append(tmp);
        }
        else
        {
            sprintf(tmp, "-- ");
            s2.append(tmp);
        }
    }
    ERG_TKI("%s", s1);
    ERG_TKI("%s", s2);
    s1.erase();
    s2.erase();
    //  ............................................................................................
    for ( i = 0 ; i != a_slots.size() ; i++ )
    {
        til = a_slots[i];

        if ( til )
        {
            sprintf(tmp, "  %02zu:[%03zu]:", i, til->size());
            s1.append(tmp);

            OFLP_STL_FOR( TooltipInfoList, *til, itl )
            {
                TooltipInfo * tti = (*itl);

                sprintf(tmp, "[%p] ", tti->a_window);
                s1.append(tmp);
            }
        }
        else
        {
            sprintf(tmp, "  %02zu:[---]:", i);
            s1.append(tmp);
        }

        ERG_TKI("%s", s1);
        s1.erase();
    }

    OFLP_LOG_FUNC_EXIT();
}

void    OflpModTooltips::   z_list_del                      (TooltipInfoList* _i_l)
{
    //  ............................................................................................
    //  ............................................................................................
    OFLP_STL_FOR( OflpModTooltips::TooltipInfoList, *_i_l, it )
    {
        TooltipInfo  *   tti = (*it);
        ERG_TKI("z_list_del():deleting TooltipInfo [%p] win[%p] str[%s] show[%i]",
            tti, tti->a_window, tti->a_str.wc_str(), tti->a_show);
        delete tti;
    }

    ERG_TKI("z_list_del():deleting TooltipInfoList [%p]", _i_l);
    delete _i_l;
}

size_t  OflpModTooltips::   z_slot_get_first_free           ()
{
    size_t      i   =   0;
    //  ............................................................................................
    //  ............................................................................................
    //  try to find a free slot in existing ones...
    for ( i = 0 ; i != a_slots.size() ; i++ )
    {
        if  ( a_slots[i] == nullptr )
        {
            ERG_TKI("found NULL slot [%zu]", i);
            return i;
        }
    }

    //  ( from here, i = a_slots.size(), since we exited the for... loop above )

    //  ...else ensure we can give a new one with index = a_slots.size()
    if ( a_slots.capacity() < ( 1 + i ) )
    {
        ERG_TKI("increasing registrants vector [%zu] -> [%zu]", a_slots.capacity(), 1 + i + 5);
        a_slots.reserve( 1 + i + 5 );
    }

    a_slots.push_back(nullptr);                                                                     // create effectively a free slot
    return i;
}
bool    OflpModTooltips::   z_slot_get_list                 (size_t _i_slot_ix, TooltipInfoList*& _o_til)
{
    //  ............................................................................................
    //  ............................................................................................
    if ( _i_slot_ix >= a_slots.size() )
    {
        ERG_TKE("index overflow ix[%zu] sz[%zu]", _i_slot_ix, a_slots.size());
        _o_til = nullptr;
        return false;
    }

    _o_til = a_slots[_i_slot_ix];

    if ( ! _o_til )
        return false;

    return true;
}

size_t  OflpModTooltips::   x_slot_add                      ()
{
    size_t      i   =   0;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::ModTooltips::x_register()");
    //  ............................................................................................
    //  ............................................................................................
    i           =   z_slot_get_first_free();
    a_slots[i]  =   new TooltipInfoList;
    ERG_TKI("created TooltipInfoList @[%zu]", i);

    z_dump();

    OFLP_LOG_FUNC_EXIT();

    return ( i );
}
void    OflpModTooltips::   x_slot_sub                      (size_t _i_slot_ix)
{
    TooltipInfoList *   til =   nullptr;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::ModTooltips::x_unregister()");
    //  ............................................................................................
    if ( ! z_slot_get_list(_i_slot_ix, til) )
    {
        ERG_TKE("%s", "call to z_slot_get_list() failed");
        return;
    }
    //  ............................................................................................
    z_list_del(til);
    a_slots[_i_slot_ix] = nullptr;
    //  ............................................................................................
    z_dump();

    OFLP_LOG_FUNC_EXIT();
}

void    OflpModTooltips::   x_add                           (size_t _i_slot_ix, wxWindow* _i_win, char const* _i_tooltip_text, bool _i_show)
{
    TooltipInfoList *   til =   nullptr;
    TooltipInfo     *   tti =   nullptr;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::ModTooltips::add()");
    //  ............................................................................................
    //  ............................................................................................
    if ( ! z_slot_get_list(_i_slot_ix, til) )
    {
        ERG_TKE("%s", "call to z_slot_get_list() failed !");
        return;
    }

    tti = new TooltipInfo(_i_win, _i_tooltip_text, _i_show);

    til->push_back( tti );

    ERG_TKI("tooltips card[%zu]",  til->size());

    z_dump();

    z_refresh_tooltip_visibility(tti, oflp::Modules::Instance()->settings()->div_tooltip_show());

    OFLP_LOG_FUNC_EXIT();
}
void    OflpModTooltips::   x_sub                           (size_t _i_slot_ix, wxWindow* _i_win)
{
    TooltipInfoList *   til =   nullptr;
    TooltipInfo     *   tti =   nullptr;
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::ModTooltips::sub()");
    //  ............................................................................................
    //  ............................................................................................
    if ( ! z_slot_get_list(_i_slot_ix, til) )
    {
        ERG_TKE("%s", "call to z_slot_get_list() failed !");
        return;
    }

    OFLP_STL_CFOR( std::list< TooltipInfo* >, *til, it )
    {
        if ( (*it)->a_window == _i_win )
        {
            delete (*it);                                                                           //  delete the TooltipInfo
            til->erase(it);
            ERG_TKI("tooltips card[%zu]",  til->size());
            break;                                                                                  //  list is modified, break out
        }
    }

    z_dump();

    OFLP_LOG_FUNC_EXIT();
}

void    OflpModTooltips::   z_refresh_tooltip_visibility    (TooltipInfo* _i_tti, bool _i_v)
{
    //  ............................................................................................
    if (    (   _i_v   )            &&
            ( _i_tti->a_show        )   )
    {
        _i_tti->a_window->SetToolTip( _i_tti->a_str );
    }
    else
    {
        _i_tti->a_window->UnsetToolTip();
    }
}

void    OflpModTooltips::   x_refresh_tooltips_visibility   ()
{
    TooltipInfoList *   til =   nullptr;
    bool                v   =   oflp::Modules::Instance()->settings()->div_tooltip_show();
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::ModTooltips::refresh()");
    //  ............................................................................................
    //  ............................................................................................
    ERG_TKI("all tooltips will be %s", v ? wxS("SET") : wxS("UNSET") );

    for ( size_t i = 0 ; i != a_slots.size() ; i++ )
    {
        til = a_slots[i];

        if ( til )
        {
            ERG_TKI("slot [%zu]...", i);

            OFLP_STL_FOR( TooltipInfoList, *til, itl )
            {
                TooltipInfo * tti = (*itl);
                ERG_TKI("  tti [%p]", tti->a_window);
                z_refresh_tooltip_visibility(tti, v);
            }
        }
        else
        {
            ERG_TKI("slot [%zu] is EMPTY", i);
        }
    }

    OFLP_LOG_FUNC_EXIT();
}

