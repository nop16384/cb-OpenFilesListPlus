#ifndef __GWR_CB_HSTRING_HH__
#define __GWR_CB_HSTRING_HH__
//  ............................................................................
#include    <stdio.h>
#include    <wx/string.h>
//  ............................................................................
namespace   earlgreycb
{

class   HString
{
  private:
                wxString    a_str;
    unsigned    long        a_hash;

  private:
    void        p0_compute_djb2()
    {
        char    const   *   str     =   get().ToAscii();
        int                 c;
        //  ....................................................................
        a_hash = 5381;

        while ( c = *str++ )
            a_hash = ( (a_hash << 5) + a_hash) + c;                             /* hash * 33 + c */
    }

  public:
    void                    set(wxString& _wxstr)
        {
            a_str = _wxstr;
            p0_compute_djb2();
        }

    wxString    const   &   get()
        {
            return a_str;
        }

    unsigned long           hash()
        {
            return a_hash;
        }

  public:
    HString()
        {
            a_hash = 0;
        }
    HString(wxString& _wxstr)
        {
            set(_wxstr);
        }
    HString(wxString  _wxstr)
        {
            set(_wxstr);
        }
   ~HString()
        {
        }
};

}

#endif                                                                          //  __GWR_CB_HSTRING_HH__


