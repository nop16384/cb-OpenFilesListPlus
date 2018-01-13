/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __OFLP_PLUGIN_MOD_LAYOUT_HH__
#define __OFLP_PLUGIN_MOD_LAYOUT_HH__
//  ................................................................................................
#include    "oflp-common.hh"

#include    "oflp-plugin-module.hh"
//  ................................................................................................
#include <tinyxml/tinyxml.h>
#include <tinyxml/tinywxuni.h>
/// ************************************************************************************************
//! \class  OflpModLayout
//!
//! \brief  Save & Load OpenFilesListPlus panels & open files layout
/// ************************************************************************************************
class OflpModLayout : public OflpModule
{
    //friend class    OpenFilesListPlus;

  public:
    class   PnAs;
    class   FlAs;
    class   PjAs;
    //  --------------------------------------------------------------------------------------------
    WX_DEFINE_ARRAY(PnAs*, PnAsAr);                                                                 //  panels of the workspace
    WX_DEFINE_ARRAY(FlAs*, FlAsAr);                                                                 //  per project : file -> panel assignment
    WX_DEFINE_ARRAY(PjAs*, PjAsAr);
    /// ********************************************************************************************
    //! \class  PnAs
    //!
    //! \brief  PanelAssignment : one ( OflpPanel name, OflpPanel index) couple
    /// ********************************************************************************************
  public:
    class   PnAs
    {
      private:
        oflp::UID               a_uid;
        wxString                a_name;
        int                     a_index;

      public:
        oflp::UID                               uid()       const   { return a_uid;         }
        wxString                                name()      const   { return a_name;        }
        int                                     index()     const   { return a_index;       }

      public:
        PnAs(oflp::UID _panel_uid, wxString& _panel_name, int _panel_index)
            {
                a_uid   =   _panel_uid;
                a_name  =   _panel_name;
                a_index =   _panel_index;
            }
       ~PnAs()
            {
            }
    };
    /// ********************************************************************************************
    //! \class  FlAs
    //!
    //! \brief  (filename -> OflpPanel) unidirectional association
    //!
    //! \detail Using some oflp::HString-s for accelerating access-s
    /// ********************************************************************************************
  public:
    class   FlAs
    {
      private:
        wxFileName                  a_absolute_filename;                                            //! absolute filename
        oflp::HString               a_absolute_filepath;                                            //! absolute filepath  + hash
        oflp::HString               a_relative_filepath;                                            //! project-relative filepath + hash
        oflp::UID                   a_panel_uid;                                                    //! assigned panel name + hash

      public:
        wxFileName                  const   &   awxfn()     const   { return a_absolute_filename;           }
        wxString                    const   &   afp()       const   { return a_absolute_filepath.get();     }
        oflp::HString::tHash                    hfp()               { return a_absolute_filepath.hash();    }
        wxString                    const   &   rfp()       const   { return a_relative_filepath.get();     }
        oflp::HString::tHash                    hrfp()              { return a_relative_filepath.hash();    }
        oflp::UID                               uid()       const   { return a_panel_uid;                   }

      public:
        FlAs(
            wxFileName  const &     _absolute_filename  ,
            wxString    const &     _relative_filepath  ,
            oflp::UID               _panel_uid          )
            {
                a_absolute_filename     =   _absolute_filename;
                a_absolute_filepath     =   _absolute_filename.GetFullPath();
                a_relative_filepath.set ( _relative_filepath );
                a_panel_uid             =   _panel_uid;
            }
       ~FlAs()
            {
            }
    };
    /// ********************************************************************************************
    //! \class  PjAs
    //!
    //! \brief  All FileAssignment-s for one project
    /// ********************************************************************************************
  public:
    class   PjAs
    {
      private:
        cbProject   *   a_project;
        FlAsAr          a_fl_as_ar;

      public:
        cbProject           const   *   project()
            {
                return a_project;
            }

        void                    add(wxFileName const & _wx_filename, wxString const & _relative_filename, oflp::UID _panel_uid)
            {
                a_fl_as_ar.Add( new FlAs(_wx_filename, _relative_filename, _panel_uid) );
            }
        void                    sub(FlAs const * _flas)
            {
                a_fl_as_ar.Remove( const_cast< FlAs* >(_flas) );
            }
        bool                    x_flas(FlAs const ** _flas, wxString const & _fn, wxString const & _rfp)    const;

      public:
        PjAs(cbProject* _project) :   a_project(_project) {}
       ~PjAs()
            {
                for ( FlAsAr::iterator
                    it  =   a_fl_as_ar.begin()   ;
                    it  !=  a_fl_as_ar.end()     ;
                    it++                            )
                {
                    delete (*it);
                }
                a_fl_as_ar.Clear();
            }
    };

  private:
    static  void            Cb_filename_to_oflp_filename    (wxString _in_filename, wxString& _out_filename, int _type);
    //  --------------------------------------------------------------------------------------------
  private:
    PnAsAr                  a_pn_as_ar;
    PjAsAr                  a_pj_as_ar;

  private:
    void                    z_pnas_dump     ();
    void                    z_pnas_add      (PnAs* _pnas);
    bool                    z_pnas_sub      (PnAs* _pnas);

    void                    z_pjas_dump     ();
    void                    z_pjas_add      (PjAs*);
    bool                    z_pjas_sub      (cbProject*);

  public:
    bool                    x_pjas          (PjAs       **, cbProject const* _nn_prj);
    bool                    x_flas          (FlAs const **, cbProject const* _nn_prj, wxString const & _fn, wxString const & _rfp);
    size_t                  x_pnas_size     ()              { return a_pn_as_ar.GetCount(); }
    PnAs    const   *       x_pnas          (size_t _ix)    { return a_pn_as_ar.Item(_ix);  }
    //  --------------------------------------------------------------------------------------------
  public:
    void                x_reset                 ();

    bool                x_xml_save_workspace    ();
    bool                x_xml_save_project      (cbProject*);

    bool                x_xml_parse_workspace   (TiXmlDocument*);
    bool                x_xml_parse_project     (cbProject* _project, TiXmlDocument*);
    //  ............................................................................................
  public:
    bool                x_workspace_load        ();
    bool                x_workspace_save        ();

    bool                x_project_load          (cbProject*);
    bool                x_project_close         (cbProject*);
    bool                x_project_save          (cbProject*);
    //  --------------------------------------------------------------------------------------------
  public:
    OflpModLayout()                                                             {}
   ~OflpModLayout()                                                             {}
};

#endif
