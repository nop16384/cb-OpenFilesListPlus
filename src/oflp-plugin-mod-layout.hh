/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PLUGIN_MOD_LAYOUT_HH__
#define __OFLP_PLUGIN_MOD_LAYOUT_HH__
//  ................................................................................................
#include    "oflp-util-uid.hh"
#include    "oflp-plugin-module.hh"
//  ................................................................................................
//  CB 10692 :
//#include <tinyxml/tinyxml.h>
//#include <tinyxml/tinywxuni.h>
//  CB 11267 :
#include <tinyxml.h>
#include <tinywxuni.h>
/// ************************************************************************************************
//! \class  OflpModLayout
//!
//! \brief  Save & Load OpenFilesListPlus panels & open files layout
/// ************************************************************************************************
class OflpModLayout : public OflpModule
{
    friend class    OpenFilesListPlus;
    //  --------------------------------------------------------------------------------------------
  public:
    class   PnAs;
    class   PfAs;
    class   PjAs;
    //  --------------------------------------------------------------------------------------------
    WX_DEFINE_ARRAY(PnAs*, PnAsAr);                                                                 //  panels of the workspace
    WX_DEFINE_ARRAY(PfAs*, PfAsAr);                                                                 //  per project : file -> panel assignment
    WX_DEFINE_ARRAY(PjAs*, PjAsAr);
    /// ********************************************************************************************
    //! \class  PnAs
    //!
    //! \brief  PanelAssignment : one ( Uid, name, visual index) 3-uple
    /// ********************************************************************************************
  public:
    class   PnAs
    {
      private:
        oflp::UID               a_uid;
        wxString                a_name;
        int                     a_visual_index;

      public:
        oflp::UID                               uid()       const   { return a_uid;             }
        wxString                                name()      const   { return a_name;            }
        int                                     vix()       const   { return a_visual_index;    }

      public:
        PnAs(oflp::UID _panel_uid, wxString& _panel_name, int _panel_visual_index)
            {
                a_uid           =   _panel_uid;
                a_name          =   _panel_name;
                a_visual_index  =   _panel_visual_index;
            }
       ~PnAs()
            {
            }
    };
    /// ********************************************************************************************
    //! \class  PfAs
    //!
    //! \brief  ( project file -> OflpPanel ) unidirectional association
    //!
    //! \detail Using some oflp::HString-s for accelerating access-s
    /// ********************************************************************************************
  public:
    class   PfAs
    {
      private:
        oflp::HString               a_relative_filepath;                                            //! project-relative filepath + hash
        oflp::UID                   a_panel_uid;                                                    //! assigned panel UID

      public:
        wxString                    const   &   rfp()       const   { return a_relative_filepath.get();     }
        oflp::HString::tHash                    hrfp()              { return a_relative_filepath.hash();    }
        oflp::UID                               uid()       const   { return a_panel_uid;                   }

      public:
        PfAs(
            wxString    const &     _relative_filepath  ,
            oflp::UID               _panel_uid          )
            {
                a_relative_filepath.set ( _relative_filepath );
                a_panel_uid             =   _panel_uid;
            }
       ~PfAs()
            {
            }
    };
    /// ********************************************************************************************
    //! \class  PjAs
    //!
    //! \brief  All ProjectFileAssignment-s for one project
    /// ********************************************************************************************
  public:
    class   PjAs
    {
      private:
        cbProject   *   a_project;
        PfAsAr          a_pf_as_ar;

      public:
        cbProject           const   *   project()
            {
                return a_project;
            }

        void                    x_pfas_add(wxString const & _relative_filename, oflp::UID _panel_uid)
            {
                a_pf_as_ar.Add( new PfAs(_relative_filename, _panel_uid) );
            }
        void                    x_pfas_sub(PfAs const * _pfas)
            {
                a_pf_as_ar.Remove( const_cast< PfAs* >(_pfas) );
            }
        bool                    x_pfas_get(PfAs const ** _pfas, wxString const & _fn, wxString const & _rfp)    const;

      public:
        PjAs(cbProject* _project) :   a_project(_project) {}
       ~PjAs()
            {
                for ( PfAsAr::iterator
                    it  =   a_pf_as_ar.begin()   ;
                    it  !=  a_pf_as_ar.end()     ;
                    it++                            )
                {
                    delete (*it);
                }
                a_pf_as_ar.Clear();
            }
    };
    //  ############################################################################################
  private:
    static  void        Cb_filename_to_oflp_filename    (wxString _in_filename, wxString& _out_filename, int _type);
    //  --------------------------------------------------------------------------------------------
  private:
    PnAsAr              a_pn_as_ar;
    PjAsAr              a_pj_as_ar;

  private:
    void                z_pnas_add      (PnAs* _pnas);
    bool                z_pnas_sub      (PnAs* _pnas);

    void                z_pjas_add      (PjAs*);
    bool                z_pjas_sub      (cbProject*);

  public:
    bool                x_pjas_get      (PjAs       **, cbProject const* _nn_prj);
    bool                x_pfas_get      (PfAs const **, cbProject const* _nn_prj, wxString const & _fn, wxString const & _rfp);
    size_t              x_pnas_size     ()              { return a_pn_as_ar.GetCount(); }
    PnAs    const   *   x_pnas_get      (size_t _ix)    { return a_pn_as_ar.Item(_ix);  }
    //  --------------------------------------------------------------------------------------------
  private:
    void                z_reset                 ();

    bool                z_xml_save_workspace    ();
    bool                z_xml_save_project      (cbProject*);

    bool                z_xml_parse_workspace   (TiXmlDocument*);
    bool                z_xml_parse_project     (cbProject* _project, TiXmlDocument*);
    //  --------------------------------------------------------------------------------------------
  public:
    bool                x_workspace_load        ();
    bool                x_workspace_save        ();

    bool                x_project_load          (cbProject*);
    bool                x_project_close         (cbProject*);
    bool                x_project_save          (cbProject*);
    //  --------------------------------------------------------------------------------------------
    void                x_dump_assignments      ();
    //  --------------------------------------------------------------------------------------------
    //  module
  private:
    virtual     void    modinit()       {};
    virtual     void    modrelease();
    //  --------------------------------------------------------------------------------------------
  public:
    OflpModLayout()                                                             {}
   ~OflpModLayout()                                                             {}
};

#endif
