#pragma once
#include <EventHandler.h>
#include <TCombobox.h>

/**
 * Class: NewResourceHandler
 * Purpose: Handles Events for the NewResource DIalog
 * 
 * SuperClass:  TapEventHandler
 */
class NewResourceHandler :
    public TapEventHandler
{
public:
    NewResourceHandler(TrecPointer<TProcess> instance);
    virtual ~NewResourceHandler();
    virtual TString GetType()override;
    void Initialize(TrecPointer<TPage> page) override;
    void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)override;
    virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;

    virtual void SetWindow(TrecPointer<TWindow> window)override;

protected:
    void OnOkay(TControl* control, EventArgs ea);

    void SetUpEnvironmentInCombobox();

    TrecSubPointer<TPage, TCombobox> envComboBox;

    TDataArray<TString> entries;
    
};

