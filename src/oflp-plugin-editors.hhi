    /// ************************************************************************
    //! \class  Actions
    //!
    //! \brief
    /// ************************************************************************
  private:
    class   Editors :   public Module
    {
      public:
        void            cb_event_dump   (CodeBlocksEvent& _evt);
        void            dump            (EditorBase* _edb);

        void            add             (EditorBase* _nn_edb);
        bool            mov             (OFLPPanel* _dst, OFLPPanel* _src, EditorBase* _edb);
    };
