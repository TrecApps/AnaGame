#include "CameraHandler.h"
#include "TInstance.h"
#include <TLayout.h>
#include <AnafacePage.h>

// Series of Handlers in String form, so that they could be referenced in a TML file
TString on_TextDirectionX(L"TextDirectionX");
TString on_TextDirectionY(L"TextDirectionY");
TString on_TextDirectionZ(L"TextDirectionZ");
TString on_TextLocationX(L"TextLocationX");
TString on_TextLocationY(L"TextLocationY");
TString on_TextLocationZ(L"TextLocationZ");
TString on_Up(L"OnUp");
TString on_Down(L"OnDown");
TString on_Left(L"OnLeft");
TString on_Right(L"OnRight");
TString on_Far(L"OnFar");
TString on_Near(L"OnNear");
TString on_SetCameraRotate(L"OnSetCameraRotate");
TString on_SetCameraTranslate(L"OnSetCameraTranslate");
TString on_SelectCurrentObject(L"OnSelectObject");
TString on_GetDefaultObject(L"OnGetDefaultObject");
TString on_ToggleObjectAndCamera(L"OnToggleObjectAndCamera");

/**
 * Method: CameraHandler::CameraHandler
 *Purpose: Constructor
 * Parameters: TrecPointer<TInstance> instance - the instance associated with the Page/handler
 *				TString& name - name of the handler, used for identifying the Arena Handler
 * Returns: New ArenaHandler instance
 */
CameraHandler::CameraHandler(TrecPointer<TProcess> instance, TString& name): TapEventHandler(instance, name)
{
	arenaName.Set(name);
	
	arenaHandlers.push_back(&CameraHandler::TextDirectionX);
	arenaHandlers.push_back(&CameraHandler::TextDirectionY);
	arenaHandlers.push_back(&CameraHandler::TextDirectionZ);

	arenaHandlers.push_back(&CameraHandler::TextLocationX);
	arenaHandlers.push_back(&CameraHandler::TextLocationY);
	arenaHandlers.push_back(&CameraHandler::TextLocationZ);

	arenaHandlers.push_back(&CameraHandler::OnUp);
	arenaHandlers.push_back(&CameraHandler::OnDown);
	arenaHandlers.push_back(&CameraHandler::OnLeft);
	arenaHandlers.push_back(&CameraHandler::OnRight);
	arenaHandlers.push_back(&CameraHandler::OnFar);
	arenaHandlers.push_back(&CameraHandler::OnNear);
	arenaHandlers.push_back(&CameraHandler::OnSetCameraRotate);
	arenaHandlers.push_back(&CameraHandler::OnSetCameraTranslate);

	arenaHandlers.push_back(&CameraHandler::OnGetDefaultObject);
	arenaHandlers.push_back(&CameraHandler::OnSelectObject);
	arenaHandlers.push_back(&CameraHandler::OnToggleObjectAndCamera);

	// Now set the structure to link the listeners to their text name
	events.addEntry(on_TextDirectionX, 0);
	events.addEntry(on_TextDirectionY,1);
	events.addEntry(on_TextDirectionZ,2);
	events.addEntry(on_TextLocationX,3);
	events.addEntry(on_TextLocationY,4);
	events.addEntry(on_TextLocationZ,5);
	events.addEntry(on_Up,6);
	events.addEntry(on_Down,7);
	events.addEntry(on_Left,8);
	events.addEntry(on_Right,9);
	events.addEntry(on_Far,10);
	events.addEntry(on_Near,11);
	events.addEntry(on_SetCameraRotate,12);
	events.addEntry(on_SetCameraTranslate,13);
	events.addEntry(on_GetDefaultObject, 14);
	events.addEntry(on_SelectCurrentObject,15);
	events.addEntry(on_ToggleObjectAndCamera,16);

	change = 0.1;
	t_dx = t_dy = t_dz = t_lx = t_ly = 0.0f;
	t_lz = 1.0f;
	rotateMode = true;
}

/**
 * Method: CameraHandler::~CameraHandler
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
CameraHandler::~CameraHandler()
{
}

/**
 * Method: CameraHandler::GetType
 * Purpose: Returns a String Representation of the object type
 * Parameters: void
 * Returns: TString - representation of the object type
 */
TString CameraHandler::GetType()
{
	return TString(L"CameraHandler;") + EventHandler::GetType();
}

/**
 * Method: CameraHandler::Initialize
 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
 * Returns: void
 */
void CameraHandler::Initialize(TrecPointer<TPage> page)
{
	if (!page.Get())
		return;
	ThreadLock();
	auto control = dynamic_cast<AnafacePage*>(page.Get())->GetRootControl();

	TrecSubPointer<TPage, TLayout> mainLay = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>(control);

	assert(mainLay.Get());

	TrecSubPointer<TPage, TLayout> panelLay = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TLayout>(mainLay->GetPage(0, 0));

	assert(panelLay.Get());

	d_x = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(panelLay->GetPage(1, 1));
	l_x = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(panelLay->GetPage(3, 1));
	d_y = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(panelLay->GetPage(1, 2));
	l_y = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(panelLay->GetPage(3, 2));
	d_z = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(panelLay->GetPage(1, 3));
	l_z = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TTextInput>(panelLay->GetPage(3, 3));

	assert(d_x.Get() && d_y.Get() && d_z.Get()&&
		l_x.Get() && l_y.Get() && l_z.Get());
	ThreadRelease();
}

/**
 * Method: CameraHandler::HandleEvents
 * Purpose: Handles Events produced from the set of TControls
 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
 * Returns: void
 */
void CameraHandler::HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)
{
	int e_id = -1;
	EventArgs ea;
	ThreadLock();
	for (UINT c = 0; c < eventAr.Size(); c++)
	{
		auto tc = eventAr.at(c).expression;
		UINT u_id = 0;
		if (!events.retrieveEntry(tc, u_id))
			continue;
		e_id = u_id;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < arenaHandlers.Size())
		{
			// call method
			if (arenaHandlers[e_id])
				(this->*arenaHandlers[e_id])(eventAr[c].control, ea);
		}
	}

	//onDraw();
	eventAr.RemoveAll();
	ThreadRelease();
}

/**
 * Method: CameraHandler::ProcessMessage
 * Purpose: Processes the message sent to the handler
 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
 * Returns: void
 *
 * The Camera Handler expects the message to come in the form:
 *      "Camera Location [float;float;float] Direction [float;float;float]"
 *
 *      "Location" and "Direction" can be swapped
 */
void CameraHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
	ThreadLock();
	if (!message.Get())
	{
		ThreadRelease();
		return;
	}
	TString messageStr = message->GetMessage_();

	auto pieces = messageStr.split(L" ");

	if (!pieces.Get() || pieces->Size() < 5 || pieces->at(0).Compare(L"Camera"))
	{
		ThreadRelease();
		return;
	}
	UINT mode = 0;

	for (UINT Rust = 1; Rust < pieces->Size(); Rust++)
	{
		if (!mode)
		{
			if (!pieces->at(Rust).Compare(L"Location"))
				mode = 1;
			else if (!pieces->at(Rust).Compare(L"Direction"))
				mode = 2;

			if (mode)
				continue;
		}

		if (mode == 1 || mode == 2)
		{
			auto data = pieces->at(Rust);

			auto datum = data.split(L";");

			if (!datum.Get() || datum->Size() != 3)
			{
				mode = 0;
				continue;
			}


			if (mode == 1)
			{
				datum->at(0).ConvertToFloat(t_lx);
				datum->at(1).ConvertToFloat(t_ly);
				datum->at(2).ConvertToFloat(t_lz);
			}
			else
			{
				datum->at(0).ConvertToFloat(t_dx);
				datum->at(1).ConvertToFloat(t_dy);
				datum->at(2).ConvertToFloat(t_dz);
			}
			mode = 0;
		}
	}
	UpdatePanelText();
	ThreadRelease();
}

/**
 * Method: CameraHandler::ShouldProcessMessageByType
 * Purpose: Reports whether this Object is of the correct type to recieve the message
 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
 * Returns: bool - true if the type matches, false oherwise
 */
bool CameraHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	ThreadLock();
	bool ret = (!message.Get()) ? false : message->GetHandlerType() == handler_type::handler_type_camera;
	ThreadRelease();
	return ret;
}

/**
 * Method: CameraHandler::SendMessageToArena
 * Purpose: Prepares a message to send to the Arena Handler
 * Parameters: const TString& target - The Name of the object to operate on
 *				const TString& attribute - the Task to perform on the object
 *				const TString& value - set of floats to provide
 * Returns: void
 */
void CameraHandler::SendMessageToArena(const TString& target, const TString& attribute, const TString& value)
{
	ThreadLock();
	TString messageStr(target + L" " + attribute + L" " + value);
	TrecPointer<HandlerMessage> newMessage = TrecPointerKey::GetNewTrecPointer<HandlerMessage>(name,
		handler_type::handler_type_arena, 0, message_transmission::message_transmission_name_type, 0, messageStr);
	if(app.Get())
		dynamic_cast<TInstance*>(TrecPointerKey::GetTrecPointerFromSoft<>(app).Get())->DispatchAnagameMessage(newMessage);
	ThreadRelease();
}


/**
 * Method: CameraHandler::UpdatePanelText
 * Purpose: Updates the various text controls that show the x-y-z values of both the Direction and Location of the camera
 * Parameters: void
 * Returns: void
 */
void CameraHandler::UpdatePanelText()
{
	ThreadLock();
	if (d_x.Get())
		d_x->SetNumericText(t_dx);
	if (d_y.Get())
		d_y->SetNumericText(t_dy);
	if (d_z.Get())
		d_z->SetNumericText(t_dz);

	if (l_x.Get())
		l_x->SetNumericText(t_lx);
	if (l_y.Get())
		l_y->SetNumericText(t_ly);
	if (l_z.Get())
		l_z->SetNumericText(t_lz);
	ThreadRelease();
}

/**
 * Method: CameraHandler::TextDirectionX
 * Purpose: Responds to text changes from the x-direction control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::TextDirectionX(TrecPointer<TPage> tc, EventArgs ea)
{
	float f = 0.0f;
	ThreadLock();
	if (!ea.text.ConvertToFloat(f))
	{
		t_dx = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_dx, t_dy, t_dz);
		SendMessageToArena(TString(L"Camera"), TString(L"Direction"), formatedText);
	}
	ThreadRelease();
}

/**
 * Method: CameraHandler::TextLocationX
 * Purpose: Responds to text changes from the x-location control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::TextLocationX(TrecPointer<TPage> tc, EventArgs ea)
{
	float f = 0.0f;
	ThreadLock();
	if (!ea.text.ConvertToFloat(f))
	{
		t_lx = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_lx, t_ly, t_lz);
		SendMessageToArena(TString(L"Camera"), TString(L"Location"), formatedText);
	}
	ThreadRelease();
}

/**
 * Method: CameraHandler::TextDirectionY
 * Purpose: Responds to text changes from the y-direction control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::TextDirectionY(TrecPointer<TPage> tc, EventArgs ea)
{
	float f = 0.0f;
	ThreadLock();
	if (!ea.text.ConvertToFloat(f))
	{
		t_dy = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_dx, t_dy, t_dz);
		SendMessageToArena(TString(L"Camera"), TString(L"Direction"), formatedText);
	}
	ThreadRelease();
}

/**
 * Method: CameraHandler::TextLocationY
 * Purpose: Responds to text changes from the y-location control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::TextLocationY(TrecPointer<TPage> tc, EventArgs ea)
{
	float f = 0.0f;
	ThreadLock();
	if (!ea.text.ConvertToFloat(f))
	{
		t_ly = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_lx, t_ly, t_lz);
		SendMessageToArena(TString(L"Camera"), TString(L"Location"), formatedText);
	}
	ThreadRelease();
}

/**
 * Method: CameraHandler::TextDirectionZ
 * Purpose: Responds to text changes from the z-direction control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::TextDirectionZ(TrecPointer<TPage> tc, EventArgs ea)
{
	float f = 0.0f;
	ThreadLock();
	if (!ea.text.ConvertToFloat(f))
	{
		t_dz = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_dx, t_dy, t_dz);
		SendMessageToArena(TString(L"Camera"), TString(L"Direction"), formatedText);
	}
	ThreadRelease();
}

/**
 * Method: CameraHandler::TextLocationZ
 * Purpose: Responds to text changes from the z-location control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::TextLocationZ(TrecPointer<TPage> tc, EventArgs ea)
{
	float f = 0.0f;
	ThreadLock();
	if (!ea.text.ConvertToFloat(f))
	{
		t_lz = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_lx, t_ly, t_lz);
		SendMessageToArena(TString(L"Camera"), TString(L"Location"), formatedText);
	}
	ThreadRelease();
}

/**
 * Method: CameraHandler::OnUp
 * Purpose: Responds to the Up button, and moves the camera "up", whether location or rotation
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnUp(TrecPointer<TPage> tc, EventArgs ea)
{
	TString formatedText;
	ThreadLock();
	if (rotateMode)
	{
		formatedText.Format(L"%f;%f", 0.0f, change);
		SendMessageToArena(TString(L"Camera"), TString(L"Rotate"), formatedText);
	}
	else
	{
		formatedText.Format(L"%f;%f;%f;%f", 0.0f, 1.0f, 0.0f, change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}
	UpdatePanelText();
	ThreadRelease();
}

/**
 * Method: CameraHandler::OnDown
 * Purpose: Responds to the Up button, and moves the camera "down", whether location or rotation
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnDown(TrecPointer<TPage> tc, EventArgs ea)
{
	TString formatedText;
	ThreadLock();
	if (rotateMode)
	{
		formatedText.Format(L"%f;%f", 0.0f, -change);
		SendMessageToArena(TString(L"Camera"), TString(L"Rotate"), formatedText);
	}
	else
	{
		formatedText.Format(L"%f;%f;%f;%f", 0.0f, 1.0f, 0.0f, -change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}

	UpdatePanelText();
	ThreadRelease();
}

/**
 * Method: CameraHandler::OnLeft
 * Purpose: Responds to the Up button, and moves the camera "left", whether location or rotation
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnLeft(TrecPointer<TPage> tc, EventArgs ea)
{
	TString formatedText;
	ThreadLock();
	if (rotateMode)
	{
		formatedText.Format(L"%f;%f", change, 0.0f);
		SendMessageToArena(TString(L"Camera"), TString(L"Rotate"), formatedText);
	}
	else
	{
		formatedText.Format(L"%f;%f;%f;%f", 1.0f, 0.0f, 0.0f, change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}
	UpdatePanelText();
	ThreadRelease();
}

/**
 * Method: CameraHandler::OnRight
 * Purpose: Responds to the Up button, and moves the camera "right", whether location or rotation
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnRight(TrecPointer<TPage> tc, EventArgs ea)
{
	TString formatedText;
	ThreadLock();
	if (rotateMode)
	{
		formatedText.Format(L"%f;%f", -change, 0.0f);
		SendMessageToArena(TString(L"Camera"), TString(L"Rotate"), formatedText);
	}
	else
	{
		formatedText.Format(L"%f;%f;%f;%f", 1.0f, 0.0f, 0.0f, -change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}

	UpdatePanelText();
	ThreadRelease();
}

/**
 * Method: CameraHandler::OnNear
 * Purpose: Responds to the Up button, and moves the camera "near", when set to location (rotation is not affected)
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnNear(TrecPointer<TPage> tc, EventArgs ea)
{
	ThreadLock();
	if (!rotateMode)
	{
		TString formatedText;
		formatedText.Format(L"%f;%f;%f;%f", 0.0f, 0.0f, 1.0f, change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}
	UpdatePanelText();
	ThreadRelease();
}

/**
 * Method: CameraHandler::OnFar
 * Purpose: Responds to the Up button, and moves the camera "far", when set to location (rotation is not affected)
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnFar(TrecPointer<TPage> tc, EventArgs ea)
{
	ThreadLock();
	if (!rotateMode)
	{
		TString formatedText;
		formatedText.Format(L"%f;%f;%f;%f", 0.0f, 0.0f, 1.0f, -change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}
	UpdatePanelText();
	ThreadRelease();
}

/**
 * Method: CameraHandler::OnSetCameraRotate
 * Purpose: Sets the mode for rotation when buttons are clicked
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnSetCameraRotate(TrecPointer<TPage> tc, EventArgs ea)
{
	ThreadLock();
	rotateMode = true;
	ThreadRelease();
}

/**
 * Method: CameraHandler::OnSetCameraTranslate
 * Purpose: Sets the mode for translation when buttons are clicked
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnSetCameraTranslate(TrecPointer<TPage> tc, EventArgs ea)
{
	ThreadLock();
	rotateMode = false;
	ThreadRelease();
}

/**
 * Method: CameraHandler::OnSelectObject
 * Purpose: [TBD - written when using the old architexture]
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnSelectObject(TrecPointer<TPage> tc, EventArgs ea)
{
	//if (!modelCollection.Get()) return;

	//currentModel = modelCollection->GetModel(ea.arrayLabel);
}

/**
 * Method: CameraHandler::OnToggleObjectAndCamera
 * Purpose: [TBD - written when using the old architexture]
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnToggleObjectAndCamera(TrecPointer<TPage> tc, EventArgs ea)
{
	//focusOnModel = !focusOnModel;

	//if (!camModToggleSign.Get())
	//	return;

	//if (focusOnModel)
	//	camModToggleSign->getText(1)->setCaption(TString(L"Model"));
	//else
	//	camModToggleSign->getText(1)->setCaption(TString(L"Camera"));

	//UpdatePanelText();
}

/**
 * Method: CameraHandler::OnGetDefaultObject
 * Purpose: [TBD - written when using the old architexture]
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnGetDefaultObject(TrecPointer<TPage> tc, EventArgs ea)
{
	//if (ea.arrayLabel >= 0 && ea.arrayLabel < basicModels.Size() && modelCollection.Get() && basicModels[ea.arrayLabel].Get())
	//{
	//	TrecPointer<ArenaModel> newModel = TrecPointerKey::GetNewSelfTrecPointer<ArenaModel>(*basicModels[ea.arrayLabel].Get());
	//	modelCollection->AddModel(newModel);
	//}
}