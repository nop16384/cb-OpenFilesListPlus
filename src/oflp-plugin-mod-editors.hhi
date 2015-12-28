/// ****************************************************************************
//! \class  OflpModEditors
//!
//! \brief  Module for all OFLP graphic stuff
/// ****************************************************************************
class OflpModEditors    :   public  OflpModule
{
    friend class    OpenFilesListPlus;
    //  ------------------------------------------------------------------------
  private:
    bool            add             (EditorBase* _nn_edb);
    bool            sub             (EditorBase* _nn_edb);
    bool            mov             (OFLPPanel* _dst, OFLPPanel* _src, EditorBase* _edb);

  public:
    void            dump            (EditorBase* _edb);
};
