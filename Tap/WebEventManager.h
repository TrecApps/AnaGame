#pragma once
#include <TVariable.h>
#include <TcWebNode.h>
#include <TPage.h>
class WebEventManager : public TVObject
{
protected:
    bool runnable;
    TDataArray<TPage::EventID_Cred> events;
    UINT runCount;
public:

    WebEventManager();

    void SetEvents(R_Message_Type type, TDataArray<TPage::EventID_Cred>& events);

    bool Run(TString& expression, TrecPointer<TPage>& page);

    /**
     * Method: TVObject::GetVariable
     * Purpose: Revtrieves a value in Variable form
     * Parameters: const TString& name - the name of the variable
     *              TrecPointer<TVariable>& var - holds the variable retrieved if valid
     * Returns: bool - whether the variable was valid or not
     *
     * Attributes: abstract
     */
    virtual bool GetVariable(const TString& name, TrecPointer<TVariable>& var) override;

    /**
     * Method: TVObject::SetVariable
     * Purpose: Sets a value in Variable form
     * Parameters: const TString& name - the name of the variable
     *              TrecPointer<TVariable> var - the value to set
     * Returns: bool - whether the variable was valid or not
     *
     * Attributes: abstract
     */
    virtual bool SetVariable(const TString& name, TrecPointer<TVariable> var) override;
};

