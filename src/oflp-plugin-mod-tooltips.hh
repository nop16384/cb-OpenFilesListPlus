/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PLUGIN_MOD_TOOLTIPS_HH__
#define __OFLP_PLUGIN_MOD_TOOLTIPS_HH__

#include    <list>

#include    "oflp-common-defines.hh"

#include    "oflp-plugin-module.hh"
/// ************************************************************************************************
//! \class  OflpModTooltips
//!
//! \brief  Module for
//!         - registering all tooltips in OFPL, so we can enable / disable them
/// ************************************************************************************************
class OflpModTooltips       :   public  OflpModule
{
    friend class    OpenFilesListPlus;
    //  --------------------------------------------------------------------------------------------
  private:
    class   TooltipInfo
    {
      public:
        wxWindow    *   a_window;
        wxString        a_str;
        bool            a_show;

      public:
        TooltipInfo(wxWindow* _i_win, const char* _i_text, bool _i_show = true)
        {
            a_window    =   _i_win;
            a_str       =   wxString::FromUTF8(_i_text);
            a_show      =   _i_show;
        }
        TooltipInfo()
        {
            a_window    =   nullptr;
            a_show      =   true;
        }
        virtual ~TooltipInfo()
        {
        }
    };
    //  --------------------------------------------------------------------------------------------
  private:
    typedef     std::list< TooltipInfo* >       TooltipInfoList;
    typedef     std::vector< TooltipInfoList* > TilVector;

    TilVector       a_slots;
    //  --------------------------------------------------------------------------------------------
  private:
    void            z_dump                  ();

  private:
    void            z_list_del              (TooltipInfoList*);

    size_t          z_slot_get_first_free   ();
    bool            z_slot_get_list         (size_t _i_slot_ix, TooltipInfoList*& _o_til);

  public:
    size_t          x_slot_add              ();
    void            x_slot_sub              (size_t);


  public:
    void        x_add                           (size_t _i_slot_ix, wxWindow*, char const* , bool _i_show = true);
    void        x_sub                           (size_t _i_slot_ix, wxWindow*);

  private:
    void        z_refresh_tooltip_visibility    (TooltipInfo*, bool);
  public:
    void        x_refresh_tooltips_visibility   ();
    //  --------------------------------------------------------------------------------------------
    //  module
  private:
    virtual     void    modinit()       {};
    virtual     void    modrelease()    {};
    //  --------------------------------------------------------------------------------------------
  public:
    OflpModTooltips()
    {
    }
    ~OflpModTooltips ()
    {
    }
};

#endif                                                                                              //  __OFLP_PLUGIN_MOD_TOOLTIPS_HH__
