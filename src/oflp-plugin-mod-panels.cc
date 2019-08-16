/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */
#include    "oflp-common.hh"
#include    "oflp-common-macros.hh"

#include    "oflp-plugin.hh"
#include    "oflp-plugin-mod-settings.hh"
#include    "oflp-plugin-mod-panels.hh"
#include    "oflp-plugin-mod-gfx.hh"

#include    "oflp-panel.hh"
#include    "oflp-panel-header.hh"
//  ................................................................................................
//  specific to this file oflp defines
#define     ERG_OFLP_SANITY_CHECKS
#include    "generated/oflp-plugin-mod-panels--log-defines.cci"
//  ################################################################################################
                OflpModPanels:: OflpModPanels   ()
{
}

                OflpModPanels::~OflpModPanels   ()
{
}

void            OflpModPanels:: init            ()
{
    dw_main_panel   =   new wxPanel(Manager::Get()->GetAppWindow(), wxNewId());
    dw_main_panel->SetFont( oflp::Modules::Instance()->gfx()->fnt8() );

    dw_main_sizer   =   new wxBoxSizer(wxVERTICAL);
    //dw_main_sizer->SetSizeHints(panels()->p0_main());
    dw_main_panel->SetSizer(dw_main_sizer);

    a_panel_bulk    =   z_add_bulk( wxString::FromUTF8("Bulk") );
}

void            OflpModPanels:: z_destroy       ()
{
    dw_main_panel->Destroy();
    dw_main_panel   =   NULL;
}

void            OflpModPanels:: z_reset         ()
{
    OflpPanel                       *       panel   =   NULL;
    //  ............................................................................................
    ERG_TKI("%s", wxS("OFLP::Panels::z_panels_reset()"));
    //  ............................................................................................
    // delete all panels
    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        panel = *(it);

        if ( ! panel->is_bulk() )
        {
            //  code copy of z_sub() because we loop on a_panels_array which
            //  would be modified by z_sub()
            z_editors_mov(a_panel_bulk, panel);

            dw_main_sizer->Detach(panel);

            //delete panel;
            //p0_sub( panel );
            panel->Destroy();
        }
    }

    a_panels_array.Clear();
    a_panels_array.Add( static_cast< OflpPanel* >(a_panel_bulk) );

    dw_main_sizer->Layout();
}

void            OflpModPanels:: z_resize        ()
{
    OflpPanel                       *   panel   =   NULL;
    int                                 ned     =   0;                          //  total # of open editors
    int                                 np      =   0;                          //  total # of panels
    int                                 p       =   0;
    int                                 pmin    =   5;                          //  minimum proportion for a panel
    int                                 pdist   =   100;                        //  remaining proportion distribute-able to panels
    //  ............................................................................................
    OFLP_LOG_FUNC_ENTER("OFLP::Panels::p0_resize()");
    //  ............................................................................................
    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        panel = *it;
        if ( ! panel->is_minimized() )
            ned = ned + static_cast< int >( (*it)->get_editors()->size() );
    }

    np      =   static_cast< int >( a_panels_array.size() );

    pdist   =   pdist - ( pmin * np );
    //D ERG_TKI("              ...pdist[%i]", pdist);
    //  ............................................................................................
    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        panel = *(it);

        //  after each panel has been attributed pmin %, it remains some % :
        //  distribute it following the # of editors per panel
        if ( pdist > 0 )
        {
            //  there are open editors
            if ( ned != 0 )
            {
                if ( panel->is_minimized() )
                {
                    p   =   0;
                }
                else
                {
                    p   =   pdist * panel->get_editors()->size();               //  affect a part of the remaining %...
                    p   =   p / ned;                                            //  ...following the # of open editors
                    p   =   p   +   pmin;                                       //  add the minimum
                }
            }
            //  no editor is open
            else
            {
                p   =   pdist   /   np;                                         //  affect the remainign % following the # of panels
                p   =   p       +   pmin;                                       //  add the minimum
            }
        }
        //  after each panel has been attributed pmin %, it does not remain
        //  any % ; do a classical allocation ( some panels will have vertical
        //  scrollbars )
        else
        {
            if ( panel->is_minimized() )
            {
                p   =   0;
            }
            else
            {
                p   =   pmin;
            }
        }
        dw_main_sizer->GetItem( panel )->SetProportion( p );

        ERG_TKI("              ...[%03i] for panel[%s]", p, panel->title().wc_str());

    }

    OFLP_LOG_FUNC_EXIT();
}

void            OflpModPanels:: z_layout        ()
{
    dw_main_sizer->Layout();
}

void            OflpModPanels:: x_set_col__bg_p  (wxColour& _c)
{
    OflpPanel                           *   panel   =   NULL;
    //  ............................................................................................
    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        panel   =   *it;
        panel->z_set_col_bg(_c);
    }
}
void            OflpModPanels:: x_set_col__bg_h  (wxColour& _c)
{
    OflpPanel                           *   panel   =   NULL;
    //  ............................................................................................
    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        panel   =   *it;
        panel->dw_header->z_set_col_bg(_c);
    }
}

OflpPanel*      OflpModPanels:: z_add_bulk      (wxString const _title)
{
    OflpPanel   *   panel   =   NULL;
    //  ............................................................................................
    ERG_TKI("%s", wxS("OFLP::Panels::z_add_bulk()"));
    //  ............................................................................................
    panel   =   new OpenFilesListPlusPanelBulk  ( dw_main_panel, _title                 );

    panel               ->z_set_col_bg( oflp::Modules::Instance()->settings()->app_col_bg_p() );
    panel->dw_header    ->z_set_col_bg( oflp::Modules::Instance()->settings()->app_col_bg_h() );

    dw_main_sizer->Add(panel, 100, wxEXPAND, 0);

    a_panels_array.Add( panel );

    return panel;
}

OflpPanel*      OflpModPanels:: z_add           (wxString const _title, oflp::UID _uid)
{
    OflpPanel   *   panel   =   NULL;
    //  ............................................................................................
    ERG_TKI("%s", wxS("OFLP::Panels::z_add()"));
    //  ............................................................................................
    panel   =   new OpenFilesListPlusPanel      ( dw_main_panel, _title, _uid, false    )   ;

    panel               ->z_set_col_bg( oflp::Modules::Instance()->settings()->app_col_bg_p() );
    panel->dw_header    ->z_set_col_bg( oflp::Modules::Instance()->settings()->app_col_bg_h() );

    dw_main_sizer->Add(panel, 100, wxEXPAND, 0);

    a_panels_array.Add( panel );

    return panel;
}

bool            OflpModPanels:: z_sub           (OflpPanel* _nn_panel)
{
    z_editors_mov(a_panel_bulk, _nn_panel);

    dw_main_sizer->Detach(_nn_panel);                                                               //  _ERG_NO_RCHECK_

    a_panels_array.Remove(_nn_panel);

    delete _nn_panel;

    return true;
}

bool            OflpModPanels:: z_move_up       (OflpPanel* _nn_panel)
{
    size_t  prev_panel_ix   =   0;
    int     pix             =   x_get_vix(_nn_panel);
    //  ............................................................................................
    ERG_TKI("OFLP::Panels::z_move_up():ix[%i]", pix);
    //  ............................................................................................
    #ifdef  ERG_OFLP_SANITY_CHECKS                                              //  _ERG_SANITY_CHECK_
    if ( pix == wxNOT_FOUND )
        return false;
    #endif

    if ( pix == 1 )
        return true;

    prev_panel_ix = static_cast< size_t >( pix - 1 );

    dw_main_sizer->Detach(_nn_panel);                                           //  _GWR_NO_RCHECK_
    dw_main_sizer->Insert( prev_panel_ix, _nn_panel, 1, wxEXPAND, 0);
    return true;
}

bool            OflpModPanels:: z_move_dn       (OflpPanel* _nn_panel)
{
    size_t  next_panel_ix   =   0;
    int     pix             =   x_get_vix(_nn_panel);
    //  ............................................................................................
    ERG_TKI("OFLP::Panels::z_move_dn():ix[%i]", pix);
    //  ............................................................................................
    #ifdef  ERG_OFLP_SANITY_CHECKS                                              //  _ERG_SANITY_CHECK_
    if ( pix == wxNOT_FOUND )
        return false;
    #endif

    if ( ( 1 + pix ) == a_panels_array.size() )
    {
        ERG_TKI("%s", wxS("              ...already the last"));
        return true;
    }

    next_panel_ix = static_cast< size_t >( pix + 1 );

    dw_main_sizer->Detach(_nn_panel);                                           //  _GWR_NO_RCHECK
    dw_main_sizer->Insert( next_panel_ix, _nn_panel, 1, wxEXPAND, 0);
    return true;
}

bool            OflpModPanels:: z_minmax        (OflpPanel* _nn_panel)
{
    wxSizerItem *   sitem   = dw_main_sizer->GetItem(_nn_panel);
    //  ............................................................................................
    #ifdef  ERG_OFLP_SANITY_CHECKS                                              //  _ERG_SANITY_CHECK_
    if ( ! sitem )
    {
        ERG_ERR("%s", wxS("  NULL wxSizerItem"));
        return false;
    }
    #endif
    //  ............................................................................................
    if ( _nn_panel->is_minimized() )
    {
        ERG_TKI("%s", wxS("  maximizing"));
        _nn_panel->maximize();
    }
    else
    {
        ERG_TKI("%s", wxS("  minimizing"));
        _nn_panel->minimize();
        sitem->SetProportion(0);
    }

    return true;
}
//  ################################################################################################
void            OflpModPanels:: x_resize_and_layout         ()
{
    z_resize();
    z_layout();
}

int             OflpModPanels:: x_get_vix                   (OflpPanel const * _panel)              const
{
    //  - wxWindowList is not re-ordered regarding layout changes, so use wxSizer
    //  - wxSizer 2.8.12 does not provide GetItemCount(), so use GetChildren()
    //  ............................................................................................
    wxSizerItemList                 list;
    wxSizerItemList::iterator       it;
    wxWindow                    *   win     =   NULL;
    int                             index   =   0;
    //  ............................................................................................
    #ifdef  ERG_OFLP_SANITY_CHECKS                                              //  _ERG_SANITY_CHECK_
    //if ( a_panels_array.Index(_panel) == wxNOT_FOUND )
        //return wxNOT_FOUND;
    #endif

    list = dw_main_sizer->GetChildren();

    for ( it = list.begin() ; it != list.end() ; it++ )
    {
        win =   (*it)->GetWindow();

        //D ERG_INF("OFLPlugin::GetPanelVisualIndex(%p):ix[%02i] win[%p]",
        //D   _panel, index, win );

        if ( win == _panel )
            return index;

        index   =   index   +   1;
    }

    return wxNOT_FOUND;
}

OflpPanel*      OflpModPanels:: x_get_from_editor           (EditorBase* _editor )
{
    OflpPanel                       *   panel   =   NULL;
    //  ............................................................................................
    //D ERG_INF("%s", wxS("OFLP::Panels::get(Editor*)"));
    //  ............................................................................................
    // loop all panels
    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        panel = *(it);
        if ( panel->editor_has(_editor) )
            OFLP_STL_RETV( panel );
    }

    return NULL;
}

bool            OflpModPanels:: x_get_from_uid              (OflpPanel** _panel, oflp::UID _uid)
{
    OflpPanel                       *   panel   =   NULL;
    //  ............................................................................................
    //D ERG_INF("%s", wxS("OFLP::Panels::get(Editor*)"));
    //  ............................................................................................
    // loop all panels
    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        panel = *(it);
        if ( panel->uid().str().Cmp( _uid.str() ) == 0 )
        {
            (*_panel) = panel;
            return true;
        }
    }

    *(_panel) = nullptr;
    return false;
}

void            OflpModPanels:: z_unselect_except           (OflpPanel* _panel)
{
    OFLP_FUNC_ENTER_MARK("OFLP::Panels::z_unselect_except()");
    //  ............................................................................................
    ERG_TKI("              *panel[%p]", _panel);

    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        if ( (*it) != _panel )
        {
            ERG_TKI("              -panel[%p]", *it);
            (*it)->editors_deselect();
        }
        else
        {
            ERG_TKI("              =panel[%p]", *it);
        }
    }
}
//  ################################################################################################
bool            OflpModPanels:: z_editors_mov   (OflpPanel* _nn_dst, OflpPanel* _nn_src)
{
    EditorArray const   *   array   = _nn_src->get_editors();
    //  ............................................................................................
    for ( EditorArray::const_iterator it = array->begin() ; it != array->end() ; it++)
    {
        if ( ! _nn_dst->editor_add( *it ) )
            return false;
    }

    _nn_src->editors_del();
    return true;
}

bool            OflpModPanels:: z_editor_mov    (OflpPanel* _nn_dst, OflpPanel* _nn_src, EditorBase* _nn_edb)
{
    if ( ! _nn_src->editor_del(_nn_edb) )
        return false;

    if ( ! _nn_dst->editor_add(_nn_edb) )
        return false;

    return true;
}

