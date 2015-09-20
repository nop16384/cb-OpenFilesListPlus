    /// ************************************************************************
    //! \class  Actions
    //!
    //! \brief
    /// ************************************************************************
  private:
    class   Actions :   public Module
    {
      public:
        void            cb_event_dump   (CodeBlocksEvent& _evt);
        void            editor_dump     (EditorBase* _edb);

        void            editor_add                  (EditorBase* _nn_edb);
        bool            editor_mov                  (OFLPPanel* _dst, OFLPPanel* _src, EditorBase* _edb);
    };
