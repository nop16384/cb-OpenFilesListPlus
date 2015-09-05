    /// ************************************************************************
    //! \class  MenuOptions
    //!
    //! \brief  Contain all oflp options
    /// ************************************************************************
    class MenuOptions       : public wxMenu
    {
        friend class OpenFilesListPlus;
      public:
        enum
        {
            eMODE_STANDARD          =   0x0001  ,
            eMODE_PRODUCTIVITY      =   0x0002  ,

            eENABLE_LOG_WINDOW      =   0x00fe  ,
            eENABLE_LOG_CONSOLE     =   0x00ff
        };
        //  ....................................................................
      private:
        wxMenu      *       dw_m1_mode;
        wxMenu      *       dw_m1_log;
        wxMenuItem  *           dw_m1_log_console;
        wxMenuItem  *           dw_m1_log_window;
        //  ....................................................................
        int                 a_mode;
        bool                a_log_console;
        bool                a_log_window;
        //  ....................................................................
      protected:
        wxMenu      *       m1mode()    { return dw_m1_mode;    }
        wxMenu      *       m1log()     { return dw_m1_log;     }
        //  ....................................................................
      protected:
        void                check_item_set_log_console  (bool _b)   { a_log_console = _b    ; dw_m1_log_console ->Check(_b); }
        void                check_item_set_log_window   (bool _b)   { a_log_window = _b     ; dw_m1_log_window  ->Check(_b); }
      public:
        bool                mode_standard()     { return ( a_mode ==  eMODE_STANDARD    );  }
        bool                mode_productivity() { return ( a_mode ==  eMODE_PRODUCTIVITY);  }
        bool                log_console()       { return a_log_console; }
        bool                log_window()        { return a_log_window;  }
        //  ....................................................................
      protected:
        void    evh_menu_option_checked         (wxCommandEvent& _e)
        {
            switch( _e.GetId() )
            {

            case eMODE_STANDARD         :
            //GWR_INF( "%s", wxS("mode standard") );
            a_mode  =   eMODE_STANDARD;
            break;

            case eMODE_PRODUCTIVITY     :
            //GWR_INF( "%s", wxS("mode productivity") );
            a_mode  =   eMODE_PRODUCTIVITY;
            break;

            case eENABLE_LOG_WINDOW     :
            a_log_window = m1log()->IsChecked(eENABLE_LOG_WINDOW)   ? true : false;
            //GWR_INF( "show log window:[%i]", a_log_window);
            break;

            case eENABLE_LOG_CONSOLE    :
            a_log_console = m1log()->IsChecked(eENABLE_LOG_CONSOLE)  ? true : false;
            //GWR_INF( "show log window:[%i]", a_log_window);
            break;
            }
        }
        //  ....................................................................
      public:
        MenuOptions()
            :   a_mode          (eMODE_STANDARD)    ,
                a_log_console   (false)             ,
                a_log_window    (false)
            {
                dw_m1_mode = new wxMenu();
                dw_m1_mode->AppendRadioItem(eMODE_STANDARD    , wxS("Standard")       , _("Edit files by double-click"));
                dw_m1_mode->AppendRadioItem(eMODE_PRODUCTIVITY, wxS("Productivity ")  , _("Edit files by single-click"));

                this->AppendSubMenu(dw_m1_mode, wxS("Mode"));

                this->AppendSeparator();

                dw_m1_log = new wxMenu();
                dw_m1_log_window    = dw_m1_log->AppendCheckItem(eENABLE_LOG_WINDOW  , wxS("wxLogWindow "));
                dw_m1_log_console   = dw_m1_log->AppendCheckItem(eENABLE_LOG_CONSOLE , wxS("console "));

                this->AppendSubMenu(dw_m1_log, wxS("Logs"));
            };
       ~MenuOptions()
            {
                //delete dw_m1_mode;
                //delete dw_m1_log;
            }
    };
    //  ========================================================================
  private:
    MenuOptions         *   dw_menu_options;                                    //!< OpenFilesListPlus options menu
    //  ------------------------------------------------------------------------
  public:
    MenuOptions         *   menu_options()                              const
        {
            return dw_menu_options;
        }

    void                    evh_menu_option_checked(wxCommandEvent &);
    //  ========================================================================
  private:
    wxMenu              *   aw_menu_view;                                       //!< c::b "View" menu
    //  ========================================================================
