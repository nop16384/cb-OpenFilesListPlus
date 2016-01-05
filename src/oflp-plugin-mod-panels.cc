/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include    "oflp-plugin-mod-panels.hh"

#include    "oflp-plugin.hh"
#include    "oflp-plugin-mod-gfx.hh"

#include    "oflp-panel.hh"
//  ............................................................................
//  specific to this file oflp defines
#define GWR_OFLP_SANITY_CHECKS
#define GWR_LOG(FORMAT, ...)    GWRCB_LOG(FORMAT, __VA_ARGS__);
#define GWR_TKI(FORMAT, ...)    GWRCB_TKI(FORMAT, __VA_ARGS__);
#define GWR_TKE(FORMAT, ...)    GWRCB_TKE(FORMAT, __VA_ARGS__);
#define GWR_INF(FORMAT, ...)    GWRCB_INF(FORMAT, __VA_ARGS__);
#define GWR_WNG(FORMAT, ...)    GWRCB_WNG(FORMAT, __VA_ARGS__);
#define GWR_ERR(FORMAT, ...)    GWRCB_ERR(FORMAT, __VA_ARGS__);
//  ############################################################################
//
//                          PANELS OPERATIONS
//
//  ############################################################################
                OflpModPanels:: OflpModPanels   ()
{
}

                OflpModPanels::~OflpModPanels   ()
{
}

void            OflpModPanels:: init            ()
{
    a_id_main_panel =   wxNewId();

    dw_main_panel   =   new wxPanel(Manager::Get()->GetAppWindow(), a_id_main_panel);
    dw_main_panel->SetFont( gfx()->fnt8() );

    dw_main_sizer   =   new wxBoxSizer(wxVERTICAL);
    //dw_main_sizer->SetSizeHints(panels()->p0_main());
    dw_main_panel->SetSizer(dw_main_sizer);

    a_panel_bulk    =   p0_add( wxString::FromUTF8("Bulk"), true );
}
//  ############################################################################
void            OflpModPanels:: p0_destroy      ()
{
    dw_main_panel->Destroy();
    dw_main_panel   =   NULL;
}

void            OflpModPanels:: p0_reset        ()
{
    OflpPanel                       *       panel   =   NULL;
    //  ........................................................................
    GWR_INF("%s", wxS("OFLP::Panels::p0_panels_reset()"));
    //  ........................................................................
    // delete all panels
    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        panel = *(it);

        if ( ! panel->is_bulk() )
        {
            //  code copy of p0_sub because we loop on a_panels_array which
            //  would be modified by p0_sub
            p0_editors_mov(a_panel_bulk, panel);

            dw_main_sizer->Detach(panel);

            delete panel;
            //p0_sub( panel );
        }
    }
    OFLP_STL_CNEXT(a_panels_array)

    a_panels_array.Clear();
    a_panels_array.Add( static_cast< OflpPanel* >(a_panel_bulk) );

    dw_main_sizer->Layout();
}

void            OflpModPanels:: p0_resize       ()
{
    OflpPanel                       *   panel   =   NULL;
    int                                 ned     =   0;                          //  total # of open editors
    int                                 np      =   0;                          //  total # of panels
    int                                 p       =   0;
    int                                 pmin    =   5;                          //  minimum proportion for a panel
    int                                 pdist   =   100;                        //  remaining proportion distribute-able to panels
    //  ........................................................................
    OFLP_FUNC_ENTER_LOG("OFLP::Panels::p0_resize()");
    //  ........................................................................
    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        panel = *it;
        if ( ! panel->is_minimized() )
            ned = ned + static_cast< int >( (*it)->get_editors()->size() );
    }
    OFLP_STL_CNEXT(a_panels_array)

    np      =   static_cast< int >( a_panels_array.size() );

    pdist   =   pdist - ( pmin * np );
    //D GWR_TKI("              ...pdist[%i]", pdist);
    //  ........................................................................
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

        GWR_TKI("              ...[%03i] for panel[%s]", p, panel->title().wc_str());

    }
    OFLP_STL_CNEXT(a_panels_array)

    OFLP_FUNC_EXIT_LOG();
}

void            OflpModPanels:: p0_layout       ()
{
    dw_main_sizer->Layout();
}

void            OflpModPanels:: p0_set_bgs      (wxColour& _c)
{
    OflpPanel                           *   panel   =   NULL;
    //  ........................................................................
    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        panel   =   *it;
        panel->set_bg(_c);
    }
    OFLP_STL_CNEXT(a_panels_array)
}

bool            OflpModPanels:: p0_editors_mov  (OflpPanel* _nn_dst, OflpPanel* _nn_src)
{
    EditorArray const   *   array   = _nn_src->get_editors();
    //  ........................................................................
    for ( EditorArray::const_iterator it = array->begin() ; it != array->end() ; it++)
    {
        if ( ! _nn_dst->editor_add( *it ) )
            return false;
    }

    _nn_src->editors_del();
    return true;
}

bool            OflpModPanels:: p0_editor_mov   (OflpPanel* _nn_dst, OflpPanel* _nn_src, EditorBase* _nn_edb)
{
    if ( ! _nn_src->editor_del(_nn_edb) )
        return false;

    if ( ! _nn_dst->editor_add(_nn_edb) )
        return false;
}

OflpPanel*      OflpModPanels:: p0_add          (wxString const _title, bool _bulk)
{
    OflpPanel   *   panel   =   NULL;
    //  ........................................................................
    GWR_INF("%s", wxS("OFLP::Panels::p0_add()"));
    //  ........................................................................
    panel   =   _bulk                                                           ?
                new OpenFilesListPlusPanelBulk  ( OpenFilesListPlus::Instance(), dw_main_panel, _title )  :
                new OpenFilesListPlusPanel      ( OpenFilesListPlus::Instance(), dw_main_panel, _title )  ;

    dw_main_sizer->Add(panel, 100, wxEXPAND, 0);

    a_panels_array.Add( panel );

    return panel;
}

bool            OflpModPanels:: p0_sub          (OflpPanel* _nn_panel)
{
    p0_editors_mov(a_panel_bulk, _nn_panel);

    dw_main_sizer->Detach(_nn_panel);                                           //  _GWR_NO_RCHECK_

    a_panels_array.Remove(_nn_panel);

    delete _nn_panel;

    return true;
}

bool            OflpModPanels:: p0_move_up      (OflpPanel* _nn_panel)
{
    size_t  prev_panel_ix   =   0;
    int     pix             =   get_visual_index(_nn_panel);
    //  ........................................................................
    GWR_INF("OFLP::Panels::p0_move_up():ix[%i]", pix);
    //  ........................................................................
    #ifdef  GWR_OFLP_SANITY_CHECKS                                              //  _GWR_SANITY_CHECK_
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

bool            OflpModPanels:: p0_move_dn      (OflpPanel* _nn_panel)
{
    size_t  next_panel_ix   =   0;
    int     pix             =   get_visual_index(_nn_panel);
    //  ........................................................................
    GWR_INF("OFLP::Panels::p0_move_dn():ix[%i]", pix);
    //  ........................................................................
    #ifdef  GWR_OFLP_SANITY_CHECKS                                              //  _GWR_SANITY_CHECK_
    if ( pix == wxNOT_FOUND )
        return false;
    #endif

    if ( ( 1 + pix ) == a_panels_array.size() )
    {
        GWR_INF("%s", wxS("              ...already the last"));
        return true;
    }

    next_panel_ix = static_cast< size_t >( pix + 1 );

    dw_main_sizer->Detach(_nn_panel);                                           //  _GWR_NO_RCHECK
    dw_main_sizer->Insert( next_panel_ix, _nn_panel, 1, wxEXPAND, 0);
    return true;
}

bool            OflpModPanels:: p0_minmax       (OflpPanel* _nn_panel)
{
    wxSizerItem *   sitem   = panels()->p0_sizer()->GetItem(_nn_panel);
    //  ........................................................................
    #ifdef  GWR_OFLP_SANITY_CHECKS                                              //  _GWR_SANITY_CHECK_
    if ( ! sitem )
    {
        GWR_ERR("%s", wxS("  NULL wxSizerItem"));
        return false;
    }
    #endif
    //  ........................................................................
    if ( _nn_panel->is_minimized() )
    {
        GWR_INF("%s", wxS("  maximizing"));
        _nn_panel->maximize();
    }
    else
    {
        GWR_INF("%s", wxS("  minimizing"));
        _nn_panel->minimize();
        sitem->SetProportion(0);
    }

    return true;
}
//  ############################################################################
void            OflpModPanels:: resize_and_layout   ()
{
    p0_resize();
    p0_layout();
}

int             OflpModPanels:: get_visual_index    (OflpPanel const * _panel)  const
{
    //  - wxWindowList is not re-ordered regarding layout changes, so use wxSizer
    //  - wxSizer 2.8.12 does not provide GetItemCount(), so use GetChildren()
    //  ........................................................................
    wxSizerItemList                 list;
    wxSizerItemList::iterator       it;
    wxWindow                    *   win     =   NULL;
    int                             index   =   0;
    //  ........................................................................
    #ifdef  GWR_OFLP_SANITY_CHECKS                                              //  _GWR_SANITY_CHECK_
    //if ( a_panels_array.Index(_panel) == wxNOT_FOUND )
        //return wxNOT_FOUND;
    #endif

    list = dw_main_sizer->GetChildren();

    for ( it = list.begin() ; it != list.end() ; it++ )
    {
        win =   (*it)->GetWindow();

        //D GWR_INF("OFLPlugin::GetPanelVisualIndex(%p):ix[%02i] win[%p]",
        //D   _panel, index, win );

        if ( win == _panel )
            return index;

        index   =   index   +   1;
    }

    return wxNOT_FOUND;
}

OflpPanel*      OflpModPanels:: get_by_name         (wxString const & _panel_name)
{
    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        OflpPanel* panel = *(it);

        if ( ! panel->title().Cmp( _panel_name ) )
        {
            OFLP_STL_RETV( a_panels_array, (*it) );
        }
    }
    OFLP_STL_CNEXT(a_panels_array)

    return NULL;
}

OflpPanel*      OflpModPanels:: get                 (EditorBase* _editor )
{
    OflpPanel                       *   panel   =   NULL;
    //  ........................................................................
    //D GWR_INF("%s", wxS("OFLP::Panels::get(Editor*)"));
    //  ........................................................................
    // loop all panels
    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        panel = *(it);
        if ( panel->editor_has(_editor) )
            OFLP_STL_RETV( a_panels_array, panel );
    }
    OFLP_STL_CNEXT(a_panels_array)

    return NULL;
}

OflpPanel*      OflpModPanels:: get_from_abs_path   (wxString const & _abs_path)
{
    OflpPanel                       *   panel   =   NULL;
    //  ........................................................................
    GWR_INF("OFLP::Panels::get(wxString&):[%s] panels[%i]", _abs_path.wc_str(), a_panels_array.size());
    //  ........................................................................
    // loop all panels
    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        panel = *it;

        if ( panel->editor_from_absolute_filepath(_abs_path) )
        {
            GWR_INF("              ...panel[%s]", panel->title().wc_str());
            OFLP_STL_RETV( a_panels_array, panel );
        }
    }
    OFLP_STL_CNEXT(a_panels_array)

    return NULL;
}

void            OflpModPanels:: p0_unselect_except  (OflpPanel* _panel)
{
    OFLP_FUNC_ENTER_MARK("OFLP::Panels::p0_unselect_except()");
    //  ........................................................................
    GWR_TKI("              *panel[%p]", _panel);

    OFLP_STL_CFOR( OflpPanelArray, a_panels_array, it )
    {
        if ( (*it) != _panel )
        {
            GWR_TKI("              -panel[%p]", *it);
            (*it)->editors_deselect();
        }
        else
        {
            GWR_TKI("              =panel[%p]", *it);
        }
    }
    OFLP_STL_CNEXT(a_panels_array)
}