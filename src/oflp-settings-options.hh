/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_SETTINGS_OPTIONS_HH__
#define __OFLP_SETTINGS_OPTIONS_HH__
//  ............................................................................
class   OflpOptLog
{
  public:
    bool    enabled;
    bool        window;
    bool        console;
};

class   OflpOptSel
{
  private:
    bool    a_sclick;
    bool    a_dclick;

  public:
    bool    sclick()            { return a_sclick; }
    bool    dclick()            { return a_dclick; }
    void    set_sclick(bool _b) { a_sclick = _b ; a_dclick = !_b; }
    void    set_dclick(bool _b) { a_dclick = _b ; a_sclick = !_b; }

  public:
    OflpOptSel()    { set_dclick(true); }
};

#endif
