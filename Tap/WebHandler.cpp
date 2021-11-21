#include "WebHandler.h"
#include <TcInterpretor.h>
void WebHandler::Initialize(TrecPointer<TPage> page)
{
}

void WebHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{
}

void WebHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

WebHandler::WebHandler(TrecPointer<TEnvironment> env, TrecPointer<TProcess> instance) : TapEventHandler(instance)
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

bool WebHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	return false;
}
