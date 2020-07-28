#include "ArenaHandler.h"
#include "Page.h"
#include "TInstance.h"

/**
 * Method: ArenaHandler::ArenaHandler
 * Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - the instance associated with the Page/handler
 *				TString& name - name of the handler, used for identifying the Camera Handler
 * Returns: New ArenaHandler instance
 */
ArenaHandler::ArenaHandler(TrecPointer<TInstance> instance, TString& name): EventHandler(instance, name)
{
	this->name.Set(name);
}

/**
 * Method: ArenaHandler::~ArenaHandler
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
ArenaHandler::~ArenaHandler()
{
}

/**
 * Method: ArenaHandler::Initialize
 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
 * Returns: void
 */
void ArenaHandler::Initialize(TrecPointer<Page> page)
{
	if (!page.Get() || !app.Get() || !page->GetWindowHandle().Get())
		return;

	auto control = page->GetRootControl();
	if (!control.Get())
		return;

	arenaControl = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TArena>(control);

	this->engine = page->GetWindowHandle()->GetNewArenaEngine(name);

	assert(arenaControl.Get());
	arenaControl->setEngine(this->engine);

	this->page = page;
}

/**
 * Method: ArenaHandler::HandleEvents
 * Purpose: Handles Events produced from the set of TControls
 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
 * Returns: void
 */
void ArenaHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
	int e_id = -1;
	EventArgs ea;
	for (UINT c = 0; c < eventAr.Size(); c++)
	{
		auto tc = eventAr.at(c).control;
		if (!tc.Get())
			continue;
		ea = tc->getEventArgs();
		e_id = ea.methodID;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < arenaHandlers.Size())
		{
			// call method
			if (arenaHandlers[e_id])
				(this->*arenaHandlers[e_id])(tc, ea);
		}
	}

	//onDraw();
	eventAr.RemoveAll();
}

/**
 * Method: ArenaHandler::ProcessMessage
 * Purpose: Processes the message sent to the handler
 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
 * Returns: void
 *
 * Note: The Arena expects the message to be of the format:
 *     "[object] [task] [float;float...]"
 *
 *  where "[object]" currently would be "Camera".
 *  "[task]" can be "Location" (3 floats), "Direction" (3 floats), "Tanslate" (3 floats), or "Rotate" (2 floats)
 */
void ArenaHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
	if (!message.Get() || !arenaControl.Get() || !app.Get())
		return;
	TString strMessage(message->GetMessage_());

	auto messages = strMessage.split(L"\s ");

	if (!messages.Get() || messages->Size() < 3)
		return;

	if (!messages->at(0).Compare(L"Camera"))
	{
		TString data(messages->at(2));
		auto dataPoints = data.split(L";");
		if (!dataPoints.Get() || dataPoints->Size() < 2)
			return;

		float x, y, z, m;

		if (dataPoints->at(0).ConvertToFloat(x) || dataPoints->at(1).ConvertToFloat(y))
			return;

		if (!messages->at(1).Compare(L"Location") && dataPoints->Size() > 2 && !dataPoints->at(2).ConvertToFloat(z))
		{
			arenaControl->UpdatePos(x, 0);
			arenaControl->UpdatePos(y, 1);
			arenaControl->UpdatePos(z, 2);
		}
		else if (!messages->at(1).Compare(L"Direction") && dataPoints->Size() > 2 && !dataPoints->at(2).ConvertToFloat(z))
		{
			arenaControl->UpdateDir(x, 0);
			arenaControl->UpdateDir(y, 1);
			arenaControl->UpdateDir(z, 2);
		}
		else if (!messages->at(1).Compare(L"Translate") && dataPoints->Size() > 3 && !dataPoints->at(2).ConvertToFloat(z) && !dataPoints->at(2).ConvertToFloat(m))
		{
			arenaControl->Translate(m, DirectX::XMFLOAT3(x, y, z));
		}
		else if (!messages->at(1).Compare(L"Rotate"))
			arenaControl->Rotate(x, y);

		auto dir = arenaControl->GetCameraDirection();
		auto loc = arenaControl->GetCameraLocation();

		TString messageStr;
		messageStr.Format(L"Camera Location %f;%f;%f Direction %f;%f;%f", loc.x, loc.y, loc.z, dir.x, dir.y, dir.z);

		TrecPointer<HandlerMessage> newMessage = TrecPointerKey::GetNewTrecPointer<HandlerMessage>(name, handler_type::handler_type_camera, 0, message_transmission::message_transmission_name_type, 0, messageStr);
		TrecPointerKey::GetTrecPointerFromSoft<TInstance>(app)->DispatchAnagameMessage(newMessage);
	}
}

/**
 * Method: ArenaHandler::ShouldProcessMessageByType
 * Purpose: Reports whether this Object is of the correct type to recieve the message
 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
 * Returns: bool - true if the type matches, false oherwise
 */
bool ArenaHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	if (!message.Get())
		return false;
	return message->GetHandlerType() == handler_type::handler_type_arena;
}
