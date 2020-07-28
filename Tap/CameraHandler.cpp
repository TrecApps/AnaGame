#include "CameraHandler.h"
#include "TInstance.h"

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
CameraHandler::CameraHandler(TrecPointer<TInstance> instance, TString& name): EventHandler(instance, name)
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
	eventNameID enid;

	enid.eventID = 0;
	enid.name.Set(on_TextDirectionX);
	events.push_back(enid);

	enid.eventID = 1;
	enid.name.Set(on_TextDirectionY);
	events.push_back(enid);

	enid.eventID = 2;
	enid.name.Set(on_TextDirectionZ);
	events.push_back(enid);

	enid.eventID = 3;
	enid.name.Set(on_TextLocationX);
	events.push_back(enid);

	enid.eventID = 4;
	enid.name.Set(on_TextLocationY);
	events.push_back(enid);

	enid.eventID = 5;
	enid.name.Set(on_TextLocationZ);
	events.push_back(enid);

	enid.eventID = 6;
	enid.name.Set(on_Up);
	events.push_back(enid);

	enid.eventID = 7;
	enid.name.Set(on_Down);
	events.push_back(enid);

	enid.eventID = 8;
	enid.name.Set(on_Left);
	events.push_back(enid);

	enid.eventID = 9;
	enid.name.Set(on_Right);
	events.push_back(enid);

	enid.eventID = 10;
	enid.name.Set(on_Far);
	events.push_back(enid);

	enid.eventID = 11;
	enid.name.Set(on_Near);
	events.push_back(enid);

	enid.eventID = 12;
	enid.name.Set(on_SetCameraRotate);
	events.push_back(enid);

	enid.eventID = 13;
	enid.name.Set(on_SetCameraTranslate);
	events.push_back(enid);

	enid.eventID = 14;
	enid.name.Set(on_GetDefaultObject);
	events.push_back(enid);

	enid.eventID = 15;
	enid.name.Set(on_SelectCurrentObject);
	events.push_back(enid);

	enid.eventID = 16;
	enid.name.Set(on_ToggleObjectAndCamera);
	events.push_back(enid);

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
 * Method: CameraHandler::Initialize
 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
 * Returns: void
 */
void CameraHandler::Initialize(TrecPointer<Page> page)
{
	if (!page.Get())
		return;

	auto control = page->GetRootControl();

	TrecSubPointer<TControl, TLayout> mainLay = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(control);

	assert(mainLay.Get());

	TrecSubPointer<TControl, TLayout> panelLay = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(mainLay->GetLayoutChild(0, 0));

	assert(panelLay.Get());

	d_x = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(panelLay->GetLayoutChild(1, 1));
	l_x = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(panelLay->GetLayoutChild(3, 1));
	d_y = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(panelLay->GetLayoutChild(1, 2));
	l_y = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(panelLay->GetLayoutChild(3, 2));
	d_z = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(panelLay->GetLayoutChild(1, 3));
	l_z = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(panelLay->GetLayoutChild(3, 3));

	assert(d_x.Get() && d_y.Get() && d_z.Get()&&
		l_x.Get() && l_y.Get() && l_z.Get());
}

/**
 * Method: CameraHandler::HandleEvents
 * Purpose: Handles Events produced from the set of TControls
 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
 * Returns: void
 */
void CameraHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
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
	if (!message.Get())
		return;

	TString messageStr = message->GetMessage_();

	auto pieces = messageStr.split(L" ");

	if (!pieces.Get() || pieces->Size() < 5 || pieces->at(0).Compare(L"Camera"))
		return;

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
}

/**
 * Method: CameraHandler::ShouldProcessMessageByType
 * Purpose: Reports whether this Object is of the correct type to recieve the message
 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
 * Returns: bool - true if the type matches, false oherwise
 */
bool CameraHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	if (!message.Get())
		return false;
	return message->GetHandlerType() == handler_type::handler_type_camera;
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
	
	TString messageStr(target + L" " + attribute + L" " + value);
	TrecPointer<HandlerMessage> newMessage = TrecPointerKey::GetNewTrecPointer<HandlerMessage>(name, handler_type::handler_type_arena, 0, message_transmission::message_transmission_name_type, 0, messageStr);
	if(app.Get())
		TrecPointerKey::GetTrecPointerFromSoft<TInstance>(app)->DispatchAnagameMessage(newMessage);
}


/**
 * Method: CameraHandler::UpdatePanelText
 * Purpose: Updates the various text controls that show the x-y-z values of both the Direction and Location of the camera
 * Parameters: void
 * Returns: void
 */
void CameraHandler::UpdatePanelText()
{
	if (d_x.Get())
		d_x->setNumericText(t_dx);
	if (d_y.Get())
		d_y->setNumericText(t_dy);
	if (d_z.Get())
		d_z->setNumericText(t_dz);

	if (l_x.Get())
		l_x->setNumericText(t_lx);
	if (l_y.Get())
		l_y->setNumericText(t_ly);
	if (l_z.Get())
		l_z->setNumericText(t_lz);

}

/**
 * Method: CameraHandler::TextDirectionX
 * Purpose: Responds to text changes from the x-direction control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::TextDirectionX(TrecPointer<TControl> tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(f))
	{
		t_dx = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_dx, t_dy, t_dz);
		SendMessageToArena(TString(L"Camera"), TString(L"Direction"), formatedText);
	}
}

/**
 * Method: CameraHandler::TextLocationX
 * Purpose: Responds to text changes from the x-location control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::TextLocationX(TrecPointer<TControl> tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(f))
	{
		t_lx = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_lx, t_ly, t_lz);
		SendMessageToArena(TString(L"Camera"), TString(L"Location"), formatedText);
	}
}

/**
 * Method: CameraHandler::TextDirectionY
 * Purpose: Responds to text changes from the y-direction control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::TextDirectionY(TrecPointer<TControl> tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(f))
	{
		t_dy = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_dx, t_dy, t_dz);
		SendMessageToArena(TString(L"Camera"), TString(L"Direction"), formatedText);
	}
}

/**
 * Method: CameraHandler::TextLocationY
 * Purpose: Responds to text changes from the y-location control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::TextLocationY(TrecPointer<TControl> tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(f))
	{
		t_ly = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_lx, t_ly, t_lz);
		SendMessageToArena(TString(L"Camera"), TString(L"Location"), formatedText);
	}
}

/**
 * Method: CameraHandler::TextDirectionZ
 * Purpose: Responds to text changes from the z-direction control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::TextDirectionZ(TrecPointer<TControl> tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(f))
	{
		t_dz = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_dx, t_dy, t_dz);
		SendMessageToArena(TString(L"Camera"), TString(L"Direction"), formatedText);
	}
}

/**
 * Method: CameraHandler::TextLocationZ
 * Purpose: Responds to text changes from the z-location control
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::TextLocationZ(TrecPointer<TControl> tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(f))
	{
		t_lz = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_lx, t_ly, t_lz);
		SendMessageToArena(TString(L"Camera"), TString(L"Location"), formatedText);
	}
}

/**
 * Method: CameraHandler::OnUp
 * Purpose: Responds to the Up button, and moves the camera "up", whether location or rotation
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnUp(TrecPointer<TControl> tc, EventArgs ea)
{
	TString formatedText;
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
}

/**
 * Method: CameraHandler::OnDown
 * Purpose: Responds to the Up button, and moves the camera "down", whether location or rotation
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnDown(TrecPointer<TControl> tc, EventArgs ea)
{
	TString formatedText;
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
}

/**
 * Method: CameraHandler::OnLeft
 * Purpose: Responds to the Up button, and moves the camera "left", whether location or rotation
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnLeft(TrecPointer<TControl> tc, EventArgs ea)
{
	TString formatedText;
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
}

/**
 * Method: CameraHandler::OnRight
 * Purpose: Responds to the Up button, and moves the camera "right", whether location or rotation
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnRight(TrecPointer<TControl> tc, EventArgs ea)
{
	TString formatedText;
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
}

/**
 * Method: CameraHandler::OnNear
 * Purpose: Responds to the Up button, and moves the camera "near", when set to location (rotation is not affected)
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnNear(TrecPointer<TControl> tc, EventArgs ea)
{
	if (!rotateMode)
	{
		TString formatedText;
		formatedText.Format(L"%f;%f;%f;%f", 0.0f, 0.0f, 1.0f, change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}
	UpdatePanelText();
}

/**
 * Method: CameraHandler::OnFar
 * Purpose: Responds to the Up button, and moves the camera "far", when set to location (rotation is not affected)
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnFar(TrecPointer<TControl> tc, EventArgs ea)
{
	if (!rotateMode)
	{
		TString formatedText;
		formatedText.Format(L"%f;%f;%f;%f", 0.0f, 0.0f, 1.0f, -change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}
	UpdatePanelText();
}

/**
 * Method: CameraHandler::OnSetCameraRotate
 * Purpose: Sets the mode for rotation when buttons are clicked
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnSetCameraRotate(TrecPointer<TControl> tc, EventArgs ea)
{
	rotateMode = true;
}

/**
 * Method: CameraHandler::OnSetCameraTranslate
 * Purpose: Sets the mode for translation when buttons are clicked
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnSetCameraTranslate(TrecPointer<TControl> tc, EventArgs ea)
{
	rotateMode = false;
}

/**
 * Method: CameraHandler::OnSelectObject
 * Purpose: [TBD - written when using the old architexture]
 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
 *				EventArgs ea - The parameters of the event
 * Returns: void
 */
void CameraHandler::OnSelectObject(TrecPointer<TControl> tc, EventArgs ea)
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
void CameraHandler::OnToggleObjectAndCamera(TrecPointer<TControl> tc, EventArgs ea)
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
void CameraHandler::OnGetDefaultObject(TrecPointer<TControl> tc, EventArgs ea)
{
	//if (ea.arrayLabel >= 0 && ea.arrayLabel < basicModels.Size() && modelCollection.Get() && basicModels[ea.arrayLabel].Get())
	//{
	//	TrecPointer<ArenaModel> newModel = TrecPointerKey::GetNewSelfTrecPointer<ArenaModel>(*basicModels[ea.arrayLabel].Get());
	//	modelCollection->AddModel(newModel);
	//}
}