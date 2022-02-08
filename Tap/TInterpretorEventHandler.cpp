#include "TInterpretorEventHandler.h"

TInterpretorEventHandler::TInterpretorEventHandler(TrecPointer<TProcess> instance): TapEventHandler(instance)
{
}

void TInterpretorEventHandler::Initialize(TrecPointer<TPage> page)
{
}

void TInterpretorEventHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{
}

void TInterpretorEventHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

bool TInterpretorEventHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	return false;
}
