#pragma once
#include <EventHandler.h>
#include <TCombobox.h>
#include <TTextInput.h>

/**
 * Class: NewResourceHandler
 * Purpose: Handles Events for the NewResource DIalog
 * 
 * SuperClass:  TapEventHandler
 */
class NewResourceHandler :
    public TapEventHandler
{
    typedef void (NewResourceHandler::* ResourceCall)(TrecPointer<TPage> tc, EventArgs ea);

public:
    NewResourceHandler(TrecPointer<TProcess> instance);
    virtual ~NewResourceHandler();
    virtual TString GetType()override;
    void Initialize(TrecPointer<TPage> page) override;
    void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)override;
    virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;

    virtual void SetWindow(TrecPointer<TWindow> window)override;


    bool IsReady();

    bool GetSpecs(TString& name, TString& type);

protected:
    void OnOkay(TrecPointer<TPage> tc, EventArgs ea);

    void OnCancel(TrecPointer<TPage> tc, EventArgs ea);

    void SetUpEnvironmentInCombobox();

    TrecSubPointer<TPage, TCombobox> envComboBox;
    TrecSubPointer<TPage, TTextInput> nameInput;

    TDataArray<TString> entries;
    /**
     * Method: EventHandler::ShouldProcessMessageByType
     * Purpose: Reports whether this Object is of the correct type to recieve the message
     * Parameters: TrecPointer<HandlerMessage> message - the message to scan
     * Returns: bool - true if the type matches, false oherwise
     *
     * Attributes: abstract
     */
    virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;

    bool ready, onDestroy;
    
};

