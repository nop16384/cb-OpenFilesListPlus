/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PLUGIN_MOD_GFX_HH__
#define __OFLP_PLUGIN_MOD_GFX_HH__
//  ............................................................................
#include    "oflp-common.hh"

#include    "oflp-plugin-module.hh"
//  ................................................................................................
#include    <wx/imaglist.h>                                                                         //  for OFPLPlugin::Gfx
#include    <wx/xrc/xmlres.h>                                                                       //  for OFPLPlugin::Gfx
#include    <wx/artprov.h>                                                                          //  for OFPLPlugin::Gfx
/// ************************************************************************************************
//! \class  OflpModGfx
//!
//! \brief  Module for all OFLP graphic stuff
/// ************************************************************************************************
class OflpModGfx   :   public  OflpModule
{
  public:
    enum
    {
        eBmpFolder          =   0   ,
        eBmpFileAscii       =   1   ,
        eBmpFileModified    =   2   ,
        eBmpFileReadOnly    =   3   ,

        eBmpBarGreen        =   4   ,
        eBmpBarRed          =   5   ,
        eBmpBarBlue         =   6   ,
        eBmpBarUp           =   7   ,
        eBmpBarDown         =   8   ,
        eBmpBarOrange       =   9   ,
        eBmpBarPadding      =  10   ,

        eBmpNotFound        =  99
    };
    //  --------------------------------------------------------------------------------------------
  private:
    static  BitmapPointerHash       A_bitmap_hash;
    static  wxImageList             A_img_list;                                                     //!< For wxTreeCtrl-s
    //  ............................................................................................
  private:
    void                    bitmap_add(int _hash_key, wxBitmap _bmp)
        {
            A_bitmap_hash[_hash_key] = new wxBitmap( _bmp );
        }
    void                    bitmap_add(int _hash_key, wxString _bitmap_file)
        {
            wxBitmap bmp  = cbLoadBitmap(_bitmap_file, wxBITMAP_TYPE_PNG);
            bitmap_add(_hash_key, bmp);
        }
  public:
    wxImageList const   *   image_list      ()
        {
            return & A_img_list;
        }

    static  int             Icon            (EditorBase* ed)
        {
            int mod = 1;          // ascii
            if (ed->IsReadOnly())
                mod = 3;          // read only
            else if (ed->GetModified())
                mod =2;           // modified
            return mod;
        }

    static  wxBitmap        Bitmap          (int _ix)
        {

            return *( A_bitmap_hash[_ix] );
        }
    //  --------------------------------------------------------------------------------------------
  private:
    wxFont              *   d_fnt_8;

  public:
    wxFont  const       &   fnt8()
        {
            return *d_fnt_8;
        }
    //  --------------------------------------------------------------------------------------------
    //  module
  private:
    virtual     void    modinit()       {};
    virtual     void    modrelease()    {};
    //  --------------------------------------------------------------------------------------------
  public:
    OflpModGfx(wxString _gfx_dir)
    {
        wxBitmap    bmp;
        //  ........................................................................................
        // load bitmaps
        bitmap_add( eBmpFolder       , _gfx_dir + wxS("folder_open.png")     );          // 0 folder
        bitmap_add( eBmpFileAscii    , _gfx_dir + wxS("ascii.png")           );          // 1 file
        bitmap_add( eBmpFileModified , _gfx_dir + wxS("modified_file.png")   );          // 2 modified file
        bitmap_add( eBmpFileReadOnly , _gfx_dir + wxS("file-readonly.png")   );          // 3 read only file


        bitmap_add( eBmpBarGreen     , wxXmlResource::Get()->LoadBitmap( wxString::FromUTF8("OFLPPH-BBG")    ));     // add panel
        bitmap_add( eBmpBarRed       , wxXmlResource::Get()->LoadBitmap( wxString::FromUTF8("OFLPPH-BBR")    ));     // del panel
        bitmap_add( eBmpBarBlue      , wxXmlResource::Get()->LoadBitmap( wxString::FromUTF8("OFLPPH-BBB")    ));     // options
        bitmap_add( eBmpBarOrange    , wxXmlResource::Get()->LoadBitmap( wxString::FromUTF8("OFLPPH-BBO")    ));     // (mini | maxi) mize panel
        bitmap_add( eBmpBarUp        , wxXmlResource::Get()->LoadBitmap( wxString::FromUTF8("OFLPPH-BBUP")   ));     // move panel up
        bitmap_add( eBmpBarDown      , wxXmlResource::Get()->LoadBitmap( wxString::FromUTF8("OFLPPH-BBDN")   ));     // move panel down
        bitmap_add( eBmpBarPadding   , wxXmlResource::Get()->LoadBitmap( wxString::FromUTF8("OFLPPH-BBPD")   ));     // empty graphic for padding

        bitmap_add( eBmpNotFound     , wxArtProvider::GetBitmap(wxART_ERROR, wxART_BUTTON) );

        A_img_list.Add( Bitmap(0) );
        A_img_list.Add( Bitmap(1) );
        A_img_list.Add( Bitmap(2) );
        A_img_list.Add( Bitmap(3) );
        //  ........................................................................................
        //  fonts
        d_fnt_8 =   new wxFont(8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxString::FromUTF8("monospace") );
    }
   ~OflpModGfx()
    {
        delete  d_fnt_8;

        for( BitmapPointerHash::iterator it = A_bitmap_hash.begin(); it != A_bitmap_hash.end(); ++it )
        {
           delete ( it->second );
        }

    }
};

#endif
