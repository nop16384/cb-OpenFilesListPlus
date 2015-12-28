/// ****************************************************************************
//! \class  OflpModSettings
//!
//! \brief  Contain all OFLP options
/// ****************************************************************************
class OflpModSettings       : public OflpModule
{
    friend class OpenFilesListPlus;
    //  ------------------------------------------------------------------------
  public:
    enum
    {
        eFILE_SELECTION_MODE_STANDARD          =   0x0001  ,
        eFILE_SELECTION_MODE_PRODUCTIVITY      =   0x0002
    };
    //  ------------------------------------------------------------------------
  private:
    struct  OflpOptLog  a_opt_log;
    struct  OflpOptSel  a_opt_sel;
    /*
    int                 a_file_selection_mode;

    bool                a_logs_enabled;
    bool                    a_log_console;
    bool                    a_log_window;
    */
  protected:
    bool                mode_standard       ()  { return a_opt_sel.dclick();    }
    bool                mode_productivity   ()  { return a_opt_sel.sclick();    }
    bool                log_enabled         ()  { return a_opt_log.enabled;     }
    bool                log_console         ()  { return a_opt_log.console;     }
    bool                log_window          ()  { return a_opt_log.window;      }
    //  ------------------------------------------------------------------------
  private:
    OFLPSettings    *   dw_settings;
    //  ........................................................................
  protected:
    void                action                      (wxCommandEvent&);
    void                settings_window_activated   (bool _b);

    void                popup   (wxCommandEvent&);
    void                popout  (wxCommandEvent&);
    void                popout  ();
    void                update_from_user_input();
    /*
    void                check_item_set_log_console  (bool _b)   { a_log_console = _b    ; dw_i2_log_console ->Check(_b); }
    void                check_item_set_log_window   (bool _b)   { a_log_window = _b     ; dw_i2_log_window  ->Check(_b); }
  public:
    bool                mode_standard()     { return ( a_mode ==  eMODE_STANDARD    );  }
    bool                mode_productivity() { return ( a_mode ==  eMODE_PRODUCTIVITY);  }
    bool                log_console()       { return a_log_console; }
    bool                log_window()        { return a_log_window;  }
    //  ------------------------------------------------------------------------
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
    */
    //  ------------------------------------------------------------------------
  public:
    OflpModSettings();
   ~OflpModSettings()
        {
            //delete dw_m1_mode;
            //delete dw_m1_log;
        }
};
