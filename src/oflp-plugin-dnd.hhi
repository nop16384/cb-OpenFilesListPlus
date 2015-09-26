  private:
    OFLPPanel           *   a_dnd_panel_src;
    OFLPPanel           *   a_dnd_panel_dst;
    EditorBase          *   a_dnd_editor_base;

  public:
    OFLPPanel   const   *   dnd_panel_src() { return a_dnd_panel_src;   }
    OFLPPanel   const   *   dnd_panel_dst() { return a_dnd_panel_dst;   }
    EditorBase          *   dnd_editor()    { return a_dnd_editor_base; }

  public:
    void                    dnd_panel_src_set   (OFLPPanel* _panel)
        {
            a_dnd_panel_src = _panel;
        }
    void                    dnd_panel_dst_set   (OFLPPanel* _panel)
        {
            a_dnd_panel_dst = _panel;
        }
    void                    dnd_editor_set      (EditorBase* _edb)
        {
            a_dnd_editor_base = _edb;
        }

  public:
    void    editor_drag_and_dropped()
        {
            editors()->mov( a_dnd_panel_dst, a_dnd_panel_src, a_dnd_editor_base );

            panels()->resize_and_layout();
        }

