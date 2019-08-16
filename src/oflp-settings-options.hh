/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_SETTINGS_OPTIONS_HH__
#define __OFLP_SETTINGS_OPTIONS_HH__
//  ................................................................................................
class   OflpOptLog
{
  public:
    bool    a_enabled;
    bool        a_window;
    bool        a_console;
};

class   OflpOptSel
{
  private:
    bool    a_sclick;   //  use of private is justified by the 2 methods sclick() & dclick() opearating on single var a_sclick

  public:
    bool    sclick()            { return a_sclick;      }
    void    sclick(bool _b)     { a_sclick = _b;        }
    bool    dclick()            { return ! a_sclick;    }
    void    dclick(bool _b)     { a_sclick = ! _b;      }

  public:
    OflpOptSel()    { dclick(true); }
};

class   OflpOptDivTt
{
  public:
    bool    a_show;
};

class   OflpOptColors
{
  public:
    wxColour    a_bg_p;
    wxColour    a_bg_h;
};


#endif
