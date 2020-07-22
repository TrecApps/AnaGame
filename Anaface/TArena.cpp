
#include "TArena.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

/*
* Method: TArena::TArena 
* Purpose: Constructor
* Parameters: TrecPointer<DrawingBoard> rt - the render target to draw to
*				TrecPointer<TArray<styleTable>> st - list of Anaface styles
*				HWND h - the window to attach to
*				CAMERA_TYPE type - type of camera to start as
* Returns: void
*/
TArena::TArena(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> st, HWND h, CAMERA_TYPE type):TControl(rt, st,false)
{
	cameraType = type;
	windowHandle = h;
	instanceHandle = GetModuleHandle(nullptr);
	viewport = NULL;
	projector = DirectX::XMMatrixPerspectiveFovLH(0.25f*DirectX::XM_PI, 800.0f / 600.0f, 1.0f, 1000.0f);

	location_3 = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	//location = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	//rotX = rotY = 0.0f;

	direction_3 = DirectX::XMFLOAT3(0.001f, 0.0f, 0.0f);
	up_3 = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	RefreshVectors();
	//camera = DirectX::XMMatrixLookToLH(location, direction, up);
	lookTo = true;
}

/*
* Method: TArena::~TArena
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
TArena::~TArena()
{
	if (viewport)
		delete viewport;
}

/*
* Method: TArena::onCreate
* Purpose: Sets up Arena specific attributes
* Parameters: RECT r - the location on screen where arena is to show
* Returns: bool - ignore
*/
bool TArena::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	TControl::onCreate(r,d3d);

	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|EngineID"));

	float aspect = static_cast<float>(r.right - r.left) / static_cast<float>(r.bottom - r.top);

	projector = DirectX::XMMatrixPerspectiveFovLH(0.25f*DirectX::XM_PI, aspect,1.0f, 1000.0f);
	
	viewport = new D3D11_VIEWPORT;
	viewport->TopLeftX = r.left;
	viewport->TopLeftY = r.top;
	viewport->Height = r.bottom - r.top;
	viewport->Width = r.right - r.left;
	viewport->MinDepth = 0.0f;
	viewport->MaxDepth = 1.0f;

	if (valpoint.Get() && d3d.Get())
	{
		arenaEngine = TrecPointerKey::GetNewTrecPointer<TArenaEngine>(d3d, *valpoint.Get());//   ArenaEngine::GetArenaEngine(*(valpoint.Get()), windowHandle, GetModuleHandle(nullptr));
		if (!arenaEngine.Get())
			return false;
	}

	valpoint = attributes.retrieveEntry(TString(L"|CameraType"));
	if (valpoint.Get())
	{
		if (!valpoint->Compare(L"LookAt"))
			lookTo = false;
	}

	valpoint = attributes.retrieveEntry(TString(L"|StartingDirection"));
	if (valpoint.Get())
	{
		TrecPointer<TDataArray<TString>> numbers = valpoint->split(TString(L","));
		if (numbers.Get() && numbers->Size() > 2)
		{
			float numbers3[3] = { 0.0f, 0.0f, 0.0f };

			TString curString = numbers->at(0);
			if (curString.GetSize())
			{
				if (!curString.ConvertToFloat(numbers3[0]))
					direction_3.x = numbers3[0];
			}
			
			curString = numbers->at(1);
			if (curString.GetSize())
			{
				if (!curString.ConvertToFloat(numbers3[1]))
					direction_3.y = numbers3[1];
			}

			curString = numbers->at(2);
			if (curString.GetSize())
			{
				if (!curString.ConvertToFloat(numbers3[2]))
					direction_3.z = numbers3[2];
			}

			IF_IS_LOOK_TO(lookTo)
			{
				float maxVal = max(abs(direction_3.x), max(abs(direction_3.y), abs(direction_3.z)));
				if (maxVal > 1.0f)
				{
					direction_3.x /= maxVal;
					direction_3.y /= maxVal;
					direction_3.z /= maxVal;
				}
			}
		}
	}


	valpoint = attributes.retrieveEntry(TString(L"|StartingLocation"));
	if (valpoint.Get())
	{
		TrecPointer<TDataArray<TString>> numbers = valpoint->split(TString(L","));
		if (numbers.Get() && numbers->Size() > 2)
		{
			float numbers3[3] = { 0.0f,0.0f,0.0f };

			TString curString = numbers->at(0);
			if (curString.GetSize())
			{
				if (!curString.ConvertToFloat(numbers3[0]))
					location_3.x = numbers3[0];
			}

			curString = numbers->at(1);
			if (curString.GetSize())
			{
				if (!curString.ConvertToFloat(numbers3[1]))
					location_3.y = numbers3[1];
			}
			curString = numbers->at(2);
			if (curString.GetSize())
			{
				if (!curString.ConvertToFloat(numbers3[2]))
					location_3.z = numbers3[2];
			}
		}
	}

	valpoint = attributes.retrieveEntry(TString(L"|Up"));
	if (valpoint.Get())
	{
		TrecPointer<TDataArray<TString>> numbers = valpoint->split(TString(L","));
		if (numbers.Get() && numbers->Size() > 2)
		{
			float numbers3[3] = { up_3.x, up_3.y,up_3.z };
			for (UINT c = 0; c < 3; c++)
			{
				TString curString = numbers->at(c);
				if (curString.GetSize())
				{
					if (curString.ConvertToFloat(numbers3[c]))
					{
						if (c == 1)
							numbers3[c] = 1.0f;
						else
							numbers3[c] = 0.0f;
					}
				}
			}

		}
	}
	RefreshVectors();

	return arenaEngine.Get() && viewport;

	
}

/*
* Method: TArena::getEngine
* Purpose: Retrieves the engine the Arena control is attached to
* Parameters: void
* Returns: TrecPointer<ArenaEngine> - the engine arena control is attached to
*/
TrecPointer<TArenaEngine> TArena::getEngine()
{
	return arenaEngine;
}


/*
* Method: TArena::onDraw
* Purpose: Draws the Control as well as activates the 3D engine to draw
* Parameters: void
* Returns: void
*/
void TArena::onDraw(TObject* obj)
{
	//TControl::onDraw();
	if (content1.Get())
		content1->onDraw(TControl::location);
	
	/*if (renderTarget)
	{
		ID2D1SolidColorBrush* brush = nullptr;
		
		if (SUCCEEDED(renderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f,0.0f,0.0f,0.0f), &brush)))
		{
			D2D1_RECT_F df;
			if(convertCRectToD2DRect(&snip, &df))
				renderTarget->FillRectangle(&df, brush);
			brush->Release();
		}
	}*/

	if (arenaEngine.Get() && viewport)
	{
		arenaEngine->RenderScene(projector,camera, *viewport);
	}

	if (text1.Get())
		text1->onDraw(TControl::location, obj);
	if (border1.Get())
		border1->onDraw(TControl::location);
	for (int c = 0; c < children.Count(); c++)
	{
		children.ElementAt(c)->onDraw(obj);
	}
}




/*
* Method: TArena::GetAnaGameType
* Purpose: Retrieves the AnaGame type for the TArena class
* Parameters: void
* Returns: UCHAR* - AnaGame representation of the type
*/
UCHAR * TArena::GetAnaGameType()
{
	return nullptr;
}



/*
 * Method: TArena::Resize
 * Purpose: Resizes the control upon the window being resized
 * Parameters: D2D1_RECT_F& r - the new location for the control
 * Returns: void
 */
void TArena::Resize(D2D1_RECT_F& r)
{
	TControl::Resize(r);
	if (!viewport)
		return;
	viewport->TopLeftX = r.left;
	viewport->TopLeftY = r.top;
	viewport->Height = r.bottom - r.top;
	viewport->Width = r.right - r.left;
}
