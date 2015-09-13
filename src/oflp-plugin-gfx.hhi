    /// ************************************************************************
    //! \class  Gfx
    //!
    //! \brief  Contain all graphic stuff
    /// ************************************************************************
  private:
    class Gfx
    {
        friend class OpenFilesListPlus;
        //  ....................................................................
      private:
        WX_DECLARE_HASH_MAP(    int                 ,
                                wxBitmap*           ,
                                wxIntegerHash       ,
                                wxIntegerEqual      ,
                                BitmapPointerHash   );

        BitmapPointerHash           a_bitmap_hash;
        wxImageList         *       d_img_list;                                 //!< For wxTreeCtrl-s
        //  ....................................................................
      private:
        void                    bitmap_add(int _hash_key, wxBitmap _bmp)
            {
                a_bitmap_hash[_hash_key] = new wxBitmap( _bmp );
            }
        void                    bitmap_add(int _hash_key, wxString _bitmap_file)
            {
                wxBitmap bmp  = cbLoadBitmap(_bitmap_file, wxBITMAP_TYPE_PNG);
                bitmap_add(_hash_key, bmp);
            }
      public:
        wxImageList const   *   image_list      ()
            {
                return d_img_list;
            }

        int                     icon            (EditorBase* ed)
            {
                int mod = 1;          // ascii
                if (ed->IsReadOnly())
                    mod = 3;          // read only
                else if (ed->GetModified())
                    mod =2;           // modified
                return mod;
            }

        wxBitmap                bitmap(int _ix)
            {

                return *( a_bitmap_hash[_ix] );
            }
        //  --------------------------------------------------------------------
      private:
        wxFont              *   d_fnt_8;

      public:
        wxFont  const       &   fnt8()
            {
                return *d_fnt_8;
            }
        //  --------------------------------------------------------------------
      public:
        Gfx(wxString _gfx_dir)
        {
            wxBitmap    bmp;
            //  ................................................................
            // load bitmaps
            d_img_list  =   new wxImageList(16, 16);

            bitmap_add( OpenFilesListPlus::eBmpFolder       , _gfx_dir + wxS("folder_open.png")     );          // 0 folder
            bitmap_add( OpenFilesListPlus::eBmpFileAscii    , _gfx_dir + wxS("ascii.png")           );          // 1 file
            bitmap_add( OpenFilesListPlus::eBmpFileModified , _gfx_dir + wxS("modified_file.png")   );          // 2 modified file
            bitmap_add( OpenFilesListPlus::eBmpFileReadOnly , _gfx_dir + wxS("file-readonly.png")   );          // 3 read only file


            bitmap_add( OpenFilesListPlus::eBmpBarGreen     , wxXmlResource::Get()->LoadBitmap( wxString::FromUTF8("OFLPPH-BBG")    ));     // add panel
            bitmap_add( OpenFilesListPlus::eBmpBarRed       , wxXmlResource::Get()->LoadBitmap( wxString::FromUTF8("OFLPPH-BBR")    ));     // del panel
            bitmap_add( OpenFilesListPlus::eBmpBarBlue      , wxXmlResource::Get()->LoadBitmap( wxString::FromUTF8("OFLPPH-BBB")    ));     // options
            bitmap_add( OpenFilesListPlus::eBmpBarOrange    , wxXmlResource::Get()->LoadBitmap( wxString::FromUTF8("OFLPPH-BBO")    ));     // (mini | maxi) mize panel
            bitmap_add( OpenFilesListPlus::eBmpBarUp        , wxXmlResource::Get()->LoadBitmap( wxString::FromUTF8("OFLPPH-BBUP")   ));     // move panel up
            bitmap_add( OpenFilesListPlus::eBmpBarDown      , wxXmlResource::Get()->LoadBitmap( wxString::FromUTF8("OFLPPH-BBDN")   ));     // move panel down

            bitmap_add( OpenFilesListPlus::eBmpNotFound     , wxArtProvider::GetBitmap(wxART_ERROR, wxART_BUTTON) );

            d_img_list->Add( bitmap(0) );
            d_img_list->Add( bitmap(1) );
            d_img_list->Add( bitmap(2) );
            d_img_list->Add( bitmap(3) );
            //  ................................................................
            //  fonts
            d_fnt_8 =   new wxFont(8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxString::FromUTF8("monospace") );
        }
       ~Gfx()
        {
            delete  d_img_list;

            delete  d_fnt_8;

            for( BitmapPointerHash::iterator it = a_bitmap_hash.begin(); it != a_bitmap_hash.end(); ++it )
            {
               delete ( it->second );
            }

        }
    };
    //  ========================================================================
  private:
    Gfx                 *   d_gfx;                                              //!< OpenFilesListPlus gfx stuff
    //  ------------------------------------------------------------------------
  public:
    Gfx                 *   gfx()                                       const
        {
            return d_gfx;
        }




