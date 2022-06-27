#include "WebHandler.h"
#include <TcInterpretor.h>
#include "WebEventManager.h"
#include <TObjectVariable.h>
void WebHandler::Initialize(TrecPointer<TPage> page)
{
}

void WebHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{
	WebEventManager* webManager = dynamic_cast<WebEventManager*>(webEventManager.Get());
	webManager->SetEvents(currentMessageType, eventAr);

	TString intType(L"JavaScript");
	// To-Do: Scan for Interpretor type


	bool pres;
	auto runner = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(environment->GetVariable(intType, pres, env_var_type::evt_interpretor));

	auto nativeWebManager = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TObjectVariable>(webEventManager);

	runner->UpdateVariable(L"event", nativeWebManager, true);
	runner->UpdateVariable(L"evt", nativeWebManager, true);
	runner->UpdateVariable(L"e", nativeWebManager, true);

	TString exp;
	TrecPointer<TPage> page;
	while (webManager->Run(exp, page))
	{
		// Set 'this' as the web node that made the call
		auto thisPage = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TObjectVariable>(TrecPointerKey::GetTrecObjectPointer(page));
		runner->UpdateVariable(L"this", thisPage, true);

		ReturnObject ret;
		runner->ProcessIndividualStatement(exp, ret);
		if(ret.errorMessage.GetSize())
			environment->PrintLine(ret.errorMessage);
	}
}

void WebHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

WebHandler::WebHandler(TrecPointer<TEnvironment> env, TrecPointer<TProcess> instance) : TapEventHandler(instance), webEventManager(TrecPointerKey::GetTrecObjectPointer(TrecPointerKey::GetNewTrecPointer<WebEventManager>()))
{
	environment = env;
}

void WebHandler::HandleWebEvents(const TString& exp, TrecPointer<TVariable> ev)
{
	TString intType(L"JavaScript");
	// To-Do: Scan for Interpretor type


	bool pres;
	auto runner =TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(environment->GetVariable(intType, pres, env_var_type::evt_interpretor));

	runner->UpdateVariable(L"event", ev, true);
	runner->UpdateVariable(L"evt", ev, true);
	runner->UpdateVariable(L"e", ev, true);
	ReturnObject ret;
	runner->ProcessIndividualStatement(exp, ret);
}

void WebHandler::SetCurrentMessageType(R_Message_Type r)
{
	currentMessageType = r;
}

bool WebHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	return false;
}
