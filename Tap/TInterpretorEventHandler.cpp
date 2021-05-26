#include "TInterpretorEventHandler.h"

TInterpretorEventHandler::TInterpretorEventHandler(TrecPointer<TInstance> instance): EventHandler(instance)
{
}

void TInterpretorEventHandler::Initialize(TrecPointer<Page> page)
{
}

void TInterpretorEventHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
}

void TInterpretorEventHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

bool TInterpretorEventHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	return false;
}
