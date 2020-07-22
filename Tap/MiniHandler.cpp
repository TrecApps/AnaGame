#include "MiniHandler.h"
#include <DirectoryInterface.h>

MiniHandler::MiniHandler(TrecPointer<TControl> m, TrecPointer<TControl> o, TrecPointer<TControl> e, TrecPointer<TInstance> i): EventHandler(i)
{
	instance = i;
	mainUI = m;
	outputUI = o;
	explorerUI = e;

	window = instance->GetMainWindow();
}

MiniHandler::~MiniHandler()
{
}

TrecPointer<TArenaEngine> MiniHandler::GetArenaEngine()
{
	return TrecPointer<TArenaEngine>();
}

TrecPointer<Page> MiniHandler::GetMainPage()
{
	return mainPage;
}

void MiniHandler::Initialize(TrecPointer<Page> p)
{
}

RECT MiniHandler::Get3DArea()
{
	return RECT();
}

void MiniHandler::OnSave()
{
	TFile saver(filePath, TFile::t_file_write | TFile::t_file_create_always);

	TString initialSearch(GetDirectory(CentralDirectories::cd_Documents));

	if (!saver.IsOpen())
	{
		OPENFILENAMEW fileInfo;
		ZeroMemory(&fileInfo, sizeof(fileInfo));

		fileInfo.lStructSize = sizeof(OPENFILENAMEW);
		fileInfo.hwndOwner = this->window->GetWindowHandle();
		fileInfo.hInstance = this->window->GetInstance()->GetInstanceHandle();
		fileInfo.lpstrFilter = nullptr;
		fileInfo.lpstrInitialDir = initialSearch.GetConstantBuffer();
		fileInfo.lpstrFile = new WCHAR[255];
		fileInfo.nMaxFile = 230;
		
		bool gotName = false;
		if (gotName = GetSaveFileNameW(&fileInfo))
		{
			filePath.Set(fileInfo.lpstrFile);
		}
		
		delete[] fileInfo.lpstrFile;
		if (!gotName) return;

		saver.Open(filePath, TFile::t_file_write | TFile::t_file_create_always);
		if (!saver.IsOpen()) return;
	}


	OnSave(saver);
	saver.Close();
}

void MiniHandler::OnLoad()
{
	OPENFILENAMEW fileInfo;
	ZeroMemory(&fileInfo, sizeof(fileInfo));

	TString initialSearch(GetDirectory(CentralDirectories::cd_Documents));

	fileInfo.lStructSize = sizeof(OPENFILENAMEW);
	fileInfo.hwndOwner = this->window->GetWindowHandle();
	fileInfo.hInstance = this->window->GetInstance()->GetInstanceHandle();
	fileInfo.lpstrFilter = nullptr;
	fileInfo.lpstrInitialDir = initialSearch.GetConstantBuffer();
	fileInfo.lpstrFile = new WCHAR[255];
	fileInfo.nMaxFile = 230;

	bool gotName = false;
	if (gotName = GetOpenFileNameW(&fileInfo))
	{
		filePath.Set(fileInfo.lpstrFile);
	}

	TFile saver(filePath, TFile::t_file_write | TFile::t_file_create_always);
	if (!saver.IsOpen()) return;

	OnLoad(saver);
	saver.Close();
}

void MiniHandler::OnShow()
{
	if (mainPage.Get())
	{
		mainPage->SetMiniHandler(TrecPointerKey::GetSubPointerFromSoft<EventHandler, MiniHandler>(self));
		mainPage->SetAnaface(mainControl);
	}
	if (outputPane.Get())
	{
		outputPane->SetMiniHandler(TrecPointerKey::GetSubPointerFromSoft<EventHandler, MiniHandler>(self));
		outputPane->SetAnaface(outputControl);
	}
	if (explorerPane.Get())
	{
		explorerPane->SetMiniHandler(TrecPointerKey::GetSubPointerFromSoft<EventHandler, MiniHandler>(self));
		explorerPane->SetAnaface(explorerControl);
	}
}

void MiniHandler::onHide()
{
}

TString MiniHandler::GetFilePath()
{
	return filePath;
}

void MiniHandler::Draw()
{
	//if (mainPage.Get())
	//	mainPage->Draw();
	//if (outputPane.Get())
	//	outputPane->Draw();
	//if (explorerPane.Get())
	//	explorerPane->Draw();
}

void MiniHandler::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnRButtonUp(nFlags, point, mOut, cred);
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont2;
		HandleEvents(cred);
		return;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnRButtonUp(nFlags, point, mOut, cred);
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont3;
		HandleEvents(cred);
		return;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnRButtonUp(nFlags, point, mOut, cred);
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			return;
		HandleEvents(cred);
		return;
	}
}

void MiniHandler::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnLButtonDown(nFlags, point, mOut, cred, TrecPointer<TFlyout>());
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont2;
		HandleEvents(cred);
		return;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnLButtonDown(nFlags, point, mOut, cred, TrecPointer<TFlyout>());
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont3;
		HandleEvents(cred);
		return;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnLButtonDown(nFlags, point, mOut, cred, TrecPointer<TFlyout>());
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			return;
		HandleEvents(cred);
		return;
	}
}

void MiniHandler::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnRButtonDown(nFlags, point, mOut, cred);
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont2;
		HandleEvents(cred);
		return;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnRButtonDown(nFlags, point, mOut, cred);
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont3;
		HandleEvents(cred);
		return;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnRButtonDown(nFlags, point, mOut, cred);
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			return;
		HandleEvents(cred);
		return;
	}
}

void MiniHandler::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnMouseMove(nFlags, point, mOut, cred, TrecPointer<TFlyout>());
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont2;
		HandleEvents(cred);
		return;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnMouseMove(nFlags, point, mOut, cred, TrecPointer<TFlyout>());
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont3;
		HandleEvents(cred);
		return;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnMouseMove(nFlags, point, mOut, cred, TrecPointer<TFlyout>());
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			return;
		HandleEvents(cred);
		return;
	}
}

void MiniHandler::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnLButtonDblClk(nFlags, point, mOut, cred);
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont2;
		HandleEvents(cred);
		return;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnLButtonDblClk(nFlags, point, mOut, cred);
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont3;
		HandleEvents(cred);
		return;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnLButtonDblClk(nFlags, point, mOut, cred);
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			return;
		HandleEvents(cred);
		return;
	}
}

void MiniHandler::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnLButtonUp(nFlags, point, mOut, cred, TrecPointer<TFlyout>());
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont2;
		HandleEvents(cred);
		return;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnLButtonUp(nFlags, point, mOut, cred, TrecPointer<TFlyout>());
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont3;
		HandleEvents(cred);
		return;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnLButtonUp(nFlags, point, mOut, cred, TrecPointer<TFlyout>());
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			return;
		HandleEvents(cred);
		return;
	}
}

bool MiniHandler::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, cred);
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont2;
		HandleEvents(cred);
		return true;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, cred);
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			goto cont3;
		HandleEvents(cred);
		return true;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, cred);
		if (*mOut == messageOutput::negative || *mOut == messageOutput::negativeUpdate)
			return false;
		HandleEvents(cred);
		return true;
	}
	return false;
}

void MiniHandler::OnResize(D2D1_RECT_F newSize)
{
	if (mainPage.Get() && mainControl.Get())
	{
		mainPage->SetArea(mainControl->getLocation());
	}

	if (outputControl.Get() && outputPane.Get())
		outputPane->SetArea(outputControl->getLocation());

	if (explorerPane.Get() && explorerControl.Get())
		explorerPane->SetArea(explorerControl->getLocation());
}

void MiniHandler::SetSelf(TrecPointer<EventHandler> s)
{
	EventHandler::SetSelf(s);
	auto tempPoint = TrecPointerKey::GetTrecSubPointerFromTrec<EventHandler, MiniHandler>(s);
	if (!tempPoint.Get())
		throw L"Error! SetSelf Operation encountered an incorrect type!";
	self = TrecPointerKey::GetSoftSubPointerFromSub<EventHandler, MiniHandler>(tempPoint);
}

void MiniHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

void MiniHandler::OnSave(TFile&)
{
}

void MiniHandler::OnLoad(TFile&)
{
}

bool MiniHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	if(!message.Get())
		return false;
	return message->GetHandlerType() == handler_type::handler_type_generic;
}

bool MiniHandler::InitializeControls()
{
	return false;
}
