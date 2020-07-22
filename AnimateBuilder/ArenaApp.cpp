#include "ArenaApp.h"

#include <DirectoryInterface.h>
#include <AnafaceParser.h>
#include <TML_Reader_.h>

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

static UINT arenaCount = 0;


ArenaApp::ArenaApp(TrecPointer<TControl> m, TrecPointer<TControl> o, TrecPointer<TControl> e, TrecPointer<TInstance> i, TString& arenaName) : MiniHandler(m, o, e, i)
{
	focusOnModel = false;

	this->arenaName.Set(arenaName);

	arenaHandlers.push_back(&ArenaApp::TextDirectionX);
	arenaHandlers.push_back(&ArenaApp::TextDirectionY);
	arenaHandlers.push_back(&ArenaApp::TextDirectionZ);

	arenaHandlers.push_back(&ArenaApp::TextLocationX);
	arenaHandlers.push_back(&ArenaApp::TextLocationY);
	arenaHandlers.push_back(&ArenaApp::TextLocationZ);

	arenaHandlers.push_back(&ArenaApp::OnUp);
	arenaHandlers.push_back(&ArenaApp::OnDown);
	arenaHandlers.push_back(&ArenaApp::OnLeft);
	arenaHandlers.push_back(&ArenaApp::OnRight);
	arenaHandlers.push_back(&ArenaApp::OnFar);
	arenaHandlers.push_back(&ArenaApp::OnNear);
	arenaHandlers.push_back(&ArenaApp::OnSetCameraRotate);
	arenaHandlers.push_back(&ArenaApp::OnSetCameraTranslate);

	arenaHandlers.push_back(&ArenaApp::OnGetDefaultObject);
	arenaHandlers.push_back(&ArenaApp::OnSelectObject);
	arenaHandlers.push_back(&ArenaApp::OnToggleObjectAndCamera);

	// Now set the structure to link the listeners to their text name
	eventNameID enid;

	enid.eventID = 0;
	enid.name.Set(on_TextDirectionX);
	handleList.push_back(enid);

	enid.eventID = 1;
	enid.name.Set(on_TextDirectionY);
	handleList.push_back(enid);

	enid.eventID = 2;
	enid.name.Set(on_TextDirectionZ);
	handleList.push_back(enid);

	enid.eventID = 3;
	enid.name.Set(on_TextLocationX);
	handleList.push_back(enid);

	enid.eventID = 4;
	enid.name.Set(on_TextLocationY);
	handleList.push_back(enid);

	enid.eventID = 5;
	enid.name.Set(on_TextLocationZ);
	handleList.push_back(enid);

	enid.eventID = 6;
	enid.name.Set(on_Up);
	handleList.push_back(enid);

	enid.eventID = 7;
	enid.name.Set(on_Down);
	handleList.push_back(enid);

	enid.eventID = 8;
	enid.name.Set(on_Left);
	handleList.push_back(enid);

	enid.eventID = 9;
	enid.name.Set(on_Right);
	handleList.push_back(enid);

	enid.eventID = 10;
	enid.name.Set(on_Far);
	handleList.push_back(enid);

	enid.eventID = 11;
	enid.name.Set(on_Near);
	handleList.push_back(enid);

	enid.eventID = 12;
	enid.name.Set(on_SetCameraRotate);
	handleList.push_back(enid);

	enid.eventID = 13;
	enid.name.Set(on_SetCameraTranslate);
	handleList.push_back(enid);

	enid.eventID = 14;
	enid.name.Set(on_GetDefaultObject);
	handleList.push_back(enid);

	enid.eventID = 15;
	enid.name.Set(on_SelectCurrentObject);
	handleList.push_back(enid);

	enid.eventID = 16;
	enid.name.Set(on_ToggleObjectAndCamera);
	handleList.push_back(enid);

	window = i->GetMainWindow();

	// Before attempting to Finish creating the app, we need to make sure that the Window is set for 3D Drawing
	if (!window->SetUp3D())
		throw L"ERROR! Unable to Prepare a 3D Engine for usage!";

	// Set up the Main Page
	
	TString fileBase;
	fileBase.Set(GetDirectory(CentralDirectories::cd_Executable));
	TString fileBody = fileBase;
	fileBody.Append(L"\\Resources\\ArenaView.tml");

	if (!m.Get() || !dynamic_cast<AnafaceUI*>(m.Get()))
		return;
	D2D1_RECT_F rect = dynamic_cast<AnafaceUI*>(m.Get())->GetControlArea();
	mainPage = window->GetPageByArea(rect);
	TrecPointer<TFile> file = TrecPointerKey::GetNewTrecPointer<TFile>();
	bool fileOpened = file->Open(fileBody, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
	if (!fileOpened)
		return;
	mainPage->SetAnaface(file, handleList);
	file->Close();
	mainControl = mainPage->GetRootControl();

	if (!mainControl.Get())
		return;

	TString name;
	name.Format(L"Arena %d", arenaCount++);
	dynamic_cast<AnafaceUI*>(m.Get())->addControl(mainControl, name);

	// Now Set up the View Panel
	
	fileBody.Set(fileBase);
	fileBody.Append(L"\\Resources\\ArenaViewPanel.txt");

	if (!o.Get() || !dynamic_cast<AnafaceUI*>(o.Get()))
		return;
	rect = dynamic_cast<AnafaceUI*>(o.Get())->GetControlArea();
	outputPane = window->GetPageByArea(rect);
	fileOpened = file->Open(fileBody, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
	if (!fileOpened)
		return;
	outputPane->SetAnaface(file, handleList);
	file->Close();
	outputControl = outputPane->GetRootControl();

	if (!outputControl.Get())
		return;

	dynamic_cast<AnafaceUI*>(o.Get())->addControl(outputControl, TString());

	//Now Do the View Bar
	
	fileBody.Set(fileBase);
	fileBody.Append(L"\\Resources\\ArenaViewBar.txt");

	if (!e.Get() || !dynamic_cast<AnafaceUI*>(e.Get()))
		return;
	rect = dynamic_cast<AnafaceUI*>(e.Get())->GetControlArea();
	explorerPane = window->GetPageByArea(rect);
	fileOpened = file->Open(fileBody, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
	if (!fileOpened)
		return;
	explorerPane->SetAnaface(file, handleList);
	file->Close();
	explorerControl = explorerPane->GetRootControl();

	if (!explorerControl.Get())
		return;

	dynamic_cast<AnafaceUI*>(e.Get())->addControl(explorerControl, TString());
}

ArenaApp::~ArenaApp()
{
	explorerControl.Delete();
	mainControl.Delete();
	outputControl.Delete();

	modelCollection.Delete();
}

bool ArenaApp::InitializeControls()
{
	if (!mainControl.Get() || !outputUI.Get() ||
		!mainUI.Get() || !outputControl.Get() ||
		!explorerControl.Get() || !explorerUI.Get() ||
		!window.Get())
		return false;

	try
	{
		// Initialize Panel Controls
		TLayout* mainLay = dynamic_cast<TLayout*>(outputControl.Get());
		arena = dynamic_cast<TArena*>(mainControl.Get());
		TLayout* panelLay = dynamic_cast<TLayout*>(mainLay->GetLayoutChild(0, 0).Get());
		d_x = dynamic_cast<TTextField*>(panelLay->GetLayoutChild(1, 1).Get());
		l_x = dynamic_cast<TTextField*>(panelLay->GetLayoutChild(3, 1).Get());
		d_y = dynamic_cast<TTextField*>(panelLay->GetLayoutChild(1, 2).Get());
		l_y = dynamic_cast<TTextField*>(panelLay->GetLayoutChild(3, 2).Get());
		d_z = dynamic_cast<TTextField*>(panelLay->GetLayoutChild(1, 3).Get());
		l_z = dynamic_cast<TTextField*>(panelLay->GetLayoutChild(3, 3).Get());

		assert(d_x && d_y && d_z &&
			l_x && l_y && l_z);

		// Set up the Camera/Model Sign to be toggled
		panelLay = dynamic_cast<TLayout*>(mainLay->GetLayoutChild(1, 0).Get());
		camModToggleSign = panelLay->GetLayoutChild(1, 2);

		// Initialize The Arena
		TrecPointer<TString> strName;
		strName = TrecPointerKey::GetNewTrecPointer<TString>(arenaName);
		arena->addAttribute(TString(L"|EngineID"), strName);


		// Create the Arena Engine and Initialize the Explorer Controls
		modelCollection = window->GetNewArenaEngine(arenaName);

		AnafaceUI* a_ui = dynamic_cast<AnafaceUI*>(explorerControl.Get());
		assert(a_ui);
		TrecPointer<TControl> modelData = a_ui->GetChildAt(0);
		TDataBind* tdb = dynamic_cast<TDataBind*>(modelData.Get());
		assert(tdb);
		tdb->setData(modelCollection->GetModelList());

		// now set up Default Model collections
		modelData = a_ui->GetChildAt(1);
		tdb = dynamic_cast<TDataBind*>(modelData.Get());
		assert(tdb);
		tdb->setData(&basicModels);

		// Create a Cube
		TDataArray<DirectX::XMFLOAT3> cubeVertices;
		cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, -1.0, -1.0));   // 0
		cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, 1.0, -1.0));  // 1
		cubeVertices.push_back(DirectX::XMFLOAT3(1.0, 1.0, -1.0));  // 2
		cubeVertices.push_back(DirectX::XMFLOAT3(1.0, -1.0, -1.0)); // 3
		cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, -1.0, 1.0));  // 4
		cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, 1.0, 1.0)); // 5
		cubeVertices.push_back(DirectX::XMFLOAT3(1.0, 1.0, 1.0)); // 6
		cubeVertices.push_back(DirectX::XMFLOAT3(1.0, -1.0, 1.0));// 7

		std::vector<UINT> v = {
		   0,1,2, // front face
		   0,2,3,
		   4,6,5, // back face
		   4,7,6,
		   4,5,1, // left face
		   4,1,0,
		   3,2,6, // right face
		   3,6,7,
		   1,5,6, // top face
		   1,6,2,
		   4,0,3, // bottom face
		   4,3,7
		};
		TDataArray<UINT> indices;
		indices = v;
		v.clear();
		TDataArray<float> floats;

		for (UINT c = 0; c < cubeVertices.Size(); c++)
		{
			floats.push_back(cubeVertices[c].x);
			floats.push_back(cubeVertices[c].y);
			floats.push_back(cubeVertices[c].z);
			//floats.push_back(1.0f);
		}

		TrecPointer<ArenaModel> basicModel = TrecPointerKey::GetNewSelfTrecPointer<ArenaModel>(modelCollection);
		basicModel->SetVertexData(floats, DefaultShader::default_shader_Single_Color);
		basicModel->SetIndices(indices);
		basicModel->setColorBuffer(0.0f, 0.0f, 1.0f, 1.0f);
		basicModel->setPipeColorBuffer(1.0f, 0.0f, 0.0f, 1.0f);

		basicModels.push_back(basicModel);
		basicModel->setName(TString(L"Cube"));
		modelCollection->AddModel(basicModel);

		// Set up model in the actual engine
		arena->setEngine(modelCollection);

		TDataArray<DirectX::XMFLOAT3> scaleVertices;
		scaleVertices.push_back(DirectX::XMFLOAT3(100.0, 0.0, 0.0));   // 0
		scaleVertices.push_back(DirectX::XMFLOAT3(-100.0, 0.0, 0.0));  // 2

		scaleVertices.push_back(DirectX::XMFLOAT3(0.0, 100.0, 0.0));
		scaleVertices.push_back(DirectX::XMFLOAT3(0.0, -100.0, 0.0));

		scaleVertices.push_back(DirectX::XMFLOAT3(0.0, 0.0, 100.0));
		scaleVertices.push_back(DirectX::XMFLOAT3(0.0, 0.0, -100.0));

		floats.RemoveAll();

		for (UINT c = 0; c < scaleVertices.Size(); c++)
		{
			floats.push_back(scaleVertices[c].x);
			floats.push_back(scaleVertices[c].y);
			floats.push_back(scaleVertices[c].z);
			//floats.push_back(1.0f);
		}

		TDataArray<UINT> uints;
		for (UINT c = 0; c < scaleVertices.Size(); c++)
			uints.push_back(c);

		scale = TrecPointerKey::GetNewSelfTrecPointer<ArenaModel>(modelCollection);
		scale->SetVertexData(floats, DefaultShader::default_shader_Single_Color, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		scale->SetIndices(uints);
		scale->setColorBuffer(0.0f, 0.0f, 1.0f, 1.0f);

		modelCollection->AddModel(scale);
		//scale->setPipeColorBuffer(1.0f, 0.0f, 0.0f, 1.0f);

		//modelCollection->AddModel(*(scale.Get()));
		UpdateCameraText();
	}
	catch (std::bad_cast& e)
	{
		return false;
	}

	return true;
}

TString ArenaApp::GetArenaName()
{
	return arenaName;
}

RECT ArenaApp::Get3DArea()
{
	if (!arena)
		return RECT{ 0,0,0,0 };
	return convertD2DRectToRECT(arena->getLocation());
}

void ArenaApp::SetColor(D2D1_COLOR_F c)
{
	if (!arena)
		return;
	if (arena->getContent(1).Get())
		arena->getContent(1)->SetColor(c);
}

void ArenaApp::HandleEvents(TDataArray<EventID_Cred>& cred)
{
	if (!arena)
		return;
	int e_id = -1;
	EventArgs ea;
	for (UINT c = 0; c < cred.Size(); c++)
	{
		auto tc = cred.at(c).control;
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
	cred.RemoveAll();
}

void ArenaApp::SetArenaName(TString& name)
{
	this->arenaName.Set(name);
}

void ArenaApp::UpdatePanelText()
{
	if (focusOnModel)
		UpdateModelText();
	else
		UpdateCameraText();
}

void ArenaApp::UpdateCameraText()
{
	if (!arena)
		return;
	DirectX::XMFLOAT3 dir = arena->GetCameraDirection(),
		loc = arena->GetCameraLocation();

	UpdatePosDirText(dir, loc);
}

void ArenaApp::UpdateModelText()
{
	if (!currentModel.Get())
		return;
	DirectX::XMFLOAT3 dir = currentModel->GetDirection(),
		loc = currentModel->GetLocation();

	UpdatePosDirText(dir, loc);
}

void ArenaApp::UpdatePosDirText(DirectX::XMFLOAT3& dir, DirectX::XMFLOAT3& loc)
{
	if (d_x)
		d_x->setNumericText(dir.x);
	if (d_y)
		d_y->setNumericText(dir.y);
	if (d_z)
		d_z->setNumericText(dir.z);

	if (l_x)
		l_x->setNumericText(loc.x);
	if (l_y)
		l_y->setNumericText(loc.y);
	if (l_z)
		l_z->setNumericText(loc.z);
}

void ArenaApp::TextDirectionX(TrecPointer<TControl> tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(f))
	{
		if (!focusOnModel)
			arena->UpdateDir(f, 0);
		//else if (currentModel.Get())
			//currentModel->Rotate(0.5, DirectX::XMFLOAT3(1.0f,0.0f,0.0f));
	}
}

void ArenaApp::TextLocationX(TrecPointer<TControl> tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(f))
	{
		if (!focusOnModel)
			arena->UpdatePos(f, 0);
	}
}

void ArenaApp::TextDirectionY(TrecPointer<TControl> tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(f))
	{
		if (!focusOnModel)
		arena->UpdateDir(f, 1);
	}
}

void ArenaApp::TextLocationY(TrecPointer<TControl> tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(f))
	{
		if (!focusOnModel)
		arena->UpdatePos(f, 1);
	}
}

void ArenaApp::TextDirectionZ(TrecPointer<TControl> tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(f))
	{
		if (!focusOnModel)
		arena->UpdateDir(f, 2);
	}
}

void ArenaApp::TextLocationZ(TrecPointer<TControl> tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(f))
	{
		if (!focusOnModel)
		arena->UpdatePos(f, 2);
	}
}

void ArenaApp::OnUp(TrecPointer<TControl> tc, EventArgs ea)
{
	if (rotateMode)
		arena->Rotate(0.0, 0.1);
	else
		arena->Translate(0.1, DirectX::XMFLOAT3(0.0, 1.0, 0.0));
	UpdatePanelText();
}

void ArenaApp::OnDown(TrecPointer<TControl> tc, EventArgs ea)
{
	if (rotateMode)
		arena->Rotate(0.0, -0.1);
	else
		arena->Translate(-0.1, DirectX::XMFLOAT3(0.0, 1.0, 0.0));
	UpdatePanelText();
}

void ArenaApp::OnLeft(TrecPointer<TControl> tc, EventArgs ea)
{
	if (rotateMode)
		arena->Rotate(0.1, 0.0);
	else
		arena->Translate(0.1, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
	UpdatePanelText();
}

void ArenaApp::OnRight(TrecPointer<TControl> tc, EventArgs ea)
{
	if (rotateMode)
		arena->Rotate(-0.1, 0.0);
	else
		arena->Translate(-0.1, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
	UpdatePanelText();
}

void ArenaApp::OnNear(TrecPointer<TControl> tc, EventArgs ea)
{
	if (!rotateMode)
		arena->Translate(0.1, DirectX::XMFLOAT3(0.0, 0.0, 1.0));
	UpdatePanelText();
}

void ArenaApp::OnFar(TrecPointer<TControl> tc, EventArgs ea)
{
	if (!rotateMode)
		arena->Translate(-0.1, DirectX::XMFLOAT3(0.0, 0.0, 1.0));
	UpdatePanelText();
}

void ArenaApp::OnSetCameraRotate(TrecPointer<TControl> tc, EventArgs ea)
{
	rotateMode = true;
}

void ArenaApp::OnSetCameraTranslate(TrecPointer<TControl> tc, EventArgs ea)
{
	rotateMode = false;
}

void ArenaApp::OnSelectObject(TrecPointer<TControl> tc, EventArgs ea)
{
	if (!modelCollection.Get()) return;

	currentModel = modelCollection->GetModel(ea.arrayLabel);
}

void ArenaApp::OnToggleObjectAndCamera(TrecPointer<TControl> tc, EventArgs ea)
{
	focusOnModel = !focusOnModel;
	
	if (!camModToggleSign.Get())
		return;
	
	if (focusOnModel)
		camModToggleSign->getText(1)->setCaption(TString(L"Model"));
	else
		camModToggleSign->getText(1)->setCaption(TString(L"Camera"));

	UpdatePanelText();
}

void ArenaApp::OnGetDefaultObject(TrecPointer<TControl> tc, EventArgs ea)
{
	if (ea.arrayLabel >= 0 && ea.arrayLabel < basicModels.Size() && modelCollection.Get() && basicModels[ea.arrayLabel].Get())
	{
		TrecPointer<ArenaModel> newModel = TrecPointerKey::GetNewSelfTrecPointer<ArenaModel>(*basicModels[ea.arrayLabel].Get());
		modelCollection->AddModel(newModel);
	}
}
