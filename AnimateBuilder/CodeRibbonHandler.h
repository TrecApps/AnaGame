#pragma once
#include <EventHandler.h>


class CodeRibbonHandler :
    public TapEventHandler
{
protected:

    typedef void (CodeRibbonHandler::* CRibbonCall)(TrecPointer<TPage> tc, EventArgs ea);

    TDataArray<CRibbonCall> calls;

    void OnNewCodeFile(TrecPointer<TPage> tc, EventArgs ea);
    void OnImportCode(TrecPointer<TPage> tc, EventArgs ea);
    void OnProcessCode(TrecPointer<TPage> tc, EventArgs ea);

    TrecSubPointer<TWindow, TIdeWindow> ideWindow;

    TrecPointer<TPage::EventHandler> currentFocus;
    TDataMap<UINT> events;

    void ProcessMessage(TrecPointer<HandlerMessage>);


public:
    CodeRibbonHandler(TrecPointer<TProcess> ins);
    ~CodeRibbonHandler();

    bool ShouldProcessMessageByType(TrecPointer<HandlerMessage>);

    void Initialize(TrecPointer<TPage> page)override;
    void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr);

    virtual void SetCallerHandler(TrecPointer<EventHandler> caller);
};

