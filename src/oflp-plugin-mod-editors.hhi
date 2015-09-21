/// ****************************************************************************
//! \class  OflpModEditors
//!
//! \brief  Module for all OFLP graphic stuff
/// ****************************************************************************
class OflpModEditors    :   public  OflpModule
{
    friend class    OpenFilesListPlus;

  private:
    void            add             (EditorBase* _nn_edb);
    bool            mov             (OFLPPanel* _dst, OFLPPanel* _src, EditorBase* _edb);

  public:
    void            cb_event_dump   (CodeBlocksEvent& _evt);
    void            dump            (EditorBase* _edb);
};
