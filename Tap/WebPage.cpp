#include "WebPage.h"
#include <TObjectVariable.h>
#include "TWindow.h"
#include <SSGenerator.h>
#include <DirectoryInterface.h>
#include "WebHandler.h"
#include "WebEventManager.h"
#include <TcInterpretor.h>




WebPage::WebPage(TrecPointer<DrawingBoard> board, TrecPointerSoft<TWindow> win): TPage(board)
{
	windowHandle = win;
	styles = TrecPointerKey::GetNewTrecPointer<TArray<styleTable>>();
}

TString WebPage::GetType()
{
	return TString(L"WebPage;") + TPage::GetType();
}

WebPage::~WebPage()
{
}

void WebPage::SetEnvironment(TrecPointer<TEnvironment> env)
{
	ThreadLock();
	environment = env;

	if (environment.Get())
	{
		// Set up access to the DOM
		environment->AddVariable(L"document", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TObjectVariable>(
			TrecPointerKey::GetTrecObjectPointer<TPage>(TrecPointerKey::GetTrecPointerFromSoft<TPage>(self))));

		TrecPointer<TWindow> actWin = TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle);

		assert(actWin.Get());

		// Set up Access to the BOM
		environment->AddVariable(L"window", TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TObjectVariable>(
			TrecPointerKey::GetTrecObjectPointer<TWindow>(actWin)));


		htmlBuilder = TrecPointerKey::GetNewTrecPointer<HtmlBuilder>(environment, this->drawingBoard);

		handler = TrecPointerKey::GetNewSelfTrecPointerAlt<EventHandler, WebHandler>(env, actWin->GetInstance());
	}
	ThreadRelease();	
}

bool WebPage::HandlesEvents()
{
	return false;
}

TString WebPage::PrepPage(TrecPointer<TFileShell> file_, TrecPointer<EventHandler> handler)
{
	if (!file_.Get())
		return L"Null File Pointer Provided!";

	TrecPointer<TFile> file = TrecPointerKey::GetNewTrecPointer<TFile>(file_->GetPath(), TFile::t_file_read | TFile::t_file_open_existing);

	if (!file->IsOpen())
	{
		return L"Could not Open File!";
	}
	if (!environment.Get())
	{
		return L"Environment Not Set Up";
	}
	if (!windowHandle.Get())
	{
		return L"Window Not Set Up";
	}
	if (file->GetFileName().EndsWith(L".html") || file->GetFileName().EndsWith(L".htm"))
	{
		if (!htmlBuilder.Get())
		{
			return L"HTML Builder Not Set Up";
		}
		TString res(htmlBuilder->BuildPage(file, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle)->GetWindowHandle(), styles));

		if (res.GetSize())
		{
			TrecPointer<TWindow> win = TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle);

			MessageBoxExW(win->GetWindowHandle(), res.GetConstantBuffer().getBuffer(), L"Error Creating WebPage", 0, 0);
			ThreadRelease();
			return L"Could Not Create Web Page!";
		}

		res.Set(SetUpCSS());
		if (!res.GetSize());
		res.Set(PrepScripts());

		TrecPointer<HtmlBody> hBody = htmlBuilder->RetrieveBody();

		rootNode = hBody.Get() ? hBody->RetrieveWebNode() : TrecSubPointer<TPage, TcWebNode>();

		if (!rootNode.Get())
		{
			TrecPointer<TWindow> win = TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle);

			MessageBoxExW(win->GetWindowHandle(), L"No Web Page Body Detected after attempted Generation!", L"Error Creating WebPage", 0, 0);
			ThreadRelease();
			return L"Could Not Detect Web Page Body";
		}
		TrecPointer<TWindow> win = TrecPointerKey::GetTrecPointerFromSoft<TWindow>(windowHandle);
		rootNode->PreCreate(TrecSubPointerSoft<TPage, TcWebNode>(), styles);
		D2D1_RECT_F tempArea = area;
		tempArea.bottom = 50000.0f;
		TDataArray<TcWebNode::FileRequest> fileRequests;
		UINT createResult = rootNode->CreateWebNode(tempArea, win->GetWindowEngine(), win->GetWindowHandle(),fileRequests, true, true);
		PrepFiles(fileRequests);
	}
	else if (file->GetFileName().EndsWith(L".tml"))
	{
		file->Close();
		anafaceFallBack = TrecPointerKey::GetNewSelfTrecSubPointer<TPage, AnafacePage>(drawingBoard);
		return anafaceFallBack->PrepPage(file_, handler);
	}
	return L"";
}

void WebPage::Draw(TrecPointer<TVariable> object)
{
	if (rootNode.Get())
	{
		TrecPointer<TVariable> object;
		rootNode->Draw(object);
	}
	else if (anafaceFallBack.Get())
		anafaceFallBack->Draw(object);
}

ag_msg void WebPage::OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
	if (rootNode.Get())
	{
		rootNode->OnRButtonUp(nFlags, point, mOut, cred);

		if (dynamic_cast<WebHandler*>(handler.Get()))
		{
			dynamic_cast<WebHandler*>(handler.Get())->SetCurrentMessageType(R_Message_Type::On_R_Button_Up);
			dynamic_cast<WebHandler*>(handler.Get())->HandleEvents(cred);
		}
	}
}

ag_msg void WebPage::OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
	if (rootNode.Get())
	{
		rootNode->OnRButtonDown(nFlags, point, mOut, cred);

		if (dynamic_cast<WebHandler*>(handler.Get()))
		{
			dynamic_cast<WebHandler*>(handler.Get())->SetCurrentMessageType(R_Message_Type::On_R_Button_Down);
			dynamic_cast<WebHandler*>(handler.Get())->HandleEvents(cred);
		}
	}
}

ag_msg void WebPage::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
	if (rootNode.Get())
	{
		rootNode->OnLButtonUp(nFlags, point, mOut, cred);

		if (dynamic_cast<WebHandler*>(handler.Get()))
		{
			dynamic_cast<WebHandler*>(handler.Get())->SetCurrentMessageType(R_Message_Type::On_L_Button_Up);
			dynamic_cast<WebHandler*>(handler.Get())->HandleEvents(cred);
		}
	}
}

ag_msg void WebPage::OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
	if (rootNode.Get())
	{
		rootNode->OnLButtonDown(nFlags, point, mOut, cred);

		if (dynamic_cast<WebHandler*>(handler.Get()))
		{
			dynamic_cast<WebHandler*>(handler.Get())->SetCurrentMessageType(R_Message_Type::On_L_Button_Down);
			dynamic_cast<WebHandler*>(handler.Get())->HandleEvents(cred);
		}
	}
}

ag_msg void WebPage::OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& cred)
{
	if (rootNode.Get())
	{
		rootNode->OnMouseMove(nFlags, point, mOut, cred);

		if (dynamic_cast<WebHandler*>(handler.Get()))
		{
			dynamic_cast<WebHandler*>(handler.Get())->SetCurrentMessageType(R_Message_Type::On_Hover);
			dynamic_cast<WebHandler*>(handler.Get())->HandleEvents(cred);
		}
	}
}

ag_msg void WebPage::OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& args)
{
	if (rootNode.Get())
	{
		rootNode->OnMouseMove(nFlags, point, mOut, args);

		if (dynamic_cast<WebHandler*>(handler.Get()))
		{
			dynamic_cast<WebHandler*>(handler.Get())->SetCurrentMessageType(R_Message_Type::On_LDoubleClick);
			dynamic_cast<WebHandler*>(handler.Get())->HandleEvents(args);
		}
	}
}

ag_msg void WebPage::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr)
{
	area = newLoc;
	auto win = TrecPointerKey::GetTrecPointerFromSoft<>(windowHandle);
	if (rootNode.Get() && win.Get())
	{
		TDataArray<TcWebNode::FileRequest> reqs;
		rootNode->CreateWebNode(newLoc, win->GetWindowEngine(), win->GetWindowHandle(), reqs);

		rootNode->OnResize(newLoc, nFlags, eventAr);

		if (dynamic_cast<WebHandler*>(handler.Get()))
		{
			dynamic_cast<WebHandler*>(handler.Get())->SetCurrentMessageType(R_Message_Type::On_Resized);
			dynamic_cast<WebHandler*>(handler.Get())->HandleEvents(eventAr);
		}
	}
}

ag_msg bool WebPage::OnDestroy()
{
	return ag_msg bool();
}

ag_msg bool WebPage::OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventID_Cred>& args)
{
	return ag_msg bool();
}


TrecPointer<TWebNode> WebPage::GetElementById(const TString& id)
{
	return TrecPointer<TWebNode>();
}

TDataArray<TrecPointer<TWebNode>> WebPage::GetElementsByTag(const TString& id)
{
	return TDataArray<TrecPointer<TWebNode>>();
}

TDataArray<TrecPointer<TWebNode>> WebPage::GetElementsByClass(const TString& id)
{
	return TDataArray<TrecPointer<TWebNode>>();
}

void WebPage::Open()
{
}

void WebPage::Write(const TString& markup)
{
}

void WebPage::Close()
{
}


/**
 * Method: WebPage::GetTitle
 * Purpose: Retirves the title of this web page
 * Parameters: void
 * Returns TString - the title of the web page
 *
 * Note: This value could be an empty String. It is up to the caller to check for that and handle it accordingly
 */
TString WebPage::GetTitle()
{
	ThreadLock();
	if (htmlBuilder.Get())
	{
		TrecPointer<HtmlHeader> head = htmlBuilder->RetrieveHeader();
		if (head.Get())
		{
			ThreadRelease();
			return head->RetrieveTitle();
		}
	}
	ThreadRelease();
	return TString();
}

void WebPage::PrepFiles(TDataArray<TcWebNode::FileRequest> req)
{
	if (!environment.Get())
		return;
	// To-Do: When Socket Support is semi-stable in Anagame, migrate this code to a series of Threads
	for (UINT Rust = 0; Rust < req.Size(); Rust++)
	{
		auto requester = req.at(Rust).requesterNode;
		auto file = environment->GetFile(req.at(Rust).fileRequest);
		if (file.Get() && requester.Get())
		{
			requester->AddFile(file);
		}
	}
}

TString WebPage::SetUpCSS()
{
	ThreadLock();
	TrecPointer<HtmlHeader> header = htmlBuilder->RetrieveHeader();

	TDataArray<TrecPointer<TFileShell>> cssFiles;

	if (header.Get())
	{
		TFile cssFile(environment->GetRootDirectory()->GetName() + L"/Anagame_Internal.css", TFile::t_file_share_write | TFile::t_file_open_always);
		if (cssFile.IsOpen())
		{
			cssFiles.push_back(TFileShell::GetFileInfo(cssFile.GetFilePath()));

			cssFile.WriteString(header->RetrieveCss());
			cssFile.Close();
		}

		UINT linkIndex = 0;
		for (TrecPointer<HtmlLink> link = header->GetLink(linkIndex++); link.Get(); link = header->GetLink(linkIndex++))
		{
			TString relation(link->getVariableValueStr(L"rel"));
			TString val(link->getVariableValueStr(L"href"));

			if (!relation.CompareNoCase(L"stylesheet"))
			{
				// To-Do: Manage retrieval of Stylesheets in a URL
				TString fullPath(directory + val);
				TFile tcssFile(fullPath, TFile::t_file_read | TFile::t_file_open_always);

				if (!tcssFile.IsOpen())
				{

				}

				if (tcssFile.IsOpen())
				{
					cssFiles.push_back(TFileShell::GetFileInfo(tcssFile.GetFilePath()));
					tcssFile.Close();
				}
			}
		}

	}

	// To Do: Add File held to manage default attributes
	TString defaultCssFile(GetDirectoryWithSlash(CentralDirectories::cd_Executable) + L"Resources\\Web-Tours\\DefaultStyles.css");
	cssFiles.push_back(TFileShell::GetFileInfo(defaultCssFile));

	// Now Read the CSS Files into a Stylesheet table

	for (UINT Rust = 0; Rust < cssFiles.Size(); Rust++)
	{
		if (!cssFiles[Rust].Get())
			continue;
		TrecPointer<TArray<styleTable>> tempStyles;
		TFile cssFile(cssFiles[Rust]->GetPath(), TFile::t_file_open_always | TFile::t_file_read);
		if (cssFile.IsOpen())
		{
			CSSGenerator gen(cssFile);
			if (gen.Parse())
			{
				tempStyles = gen.GetStyles();
				if (tempStyles.Get())
				{
					for (UINT Rust = 0; Rust < tempStyles->Count(); Rust++)
					{
						if (tempStyles->ElementAt(Rust).Get())
							styles->Add(tempStyles->ElementAt(Rust));
					}
				}
			}
		}
	}
	ThreadRelease();
	return TString();
}

TString WebPage::PrepScripts()
{
	if (!environment.Get() || !htmlBuilder.Get())
		return L"Error! Environment not set up for Script processing!";
	TString javaScipt(L"#$_New_JS_");

	TrecPointer<HtmlHeader> headers = htmlBuilder->RetrieveHeader();
	ReturnObject ret;
	if (headers.Get())
	{
		auto scripts = headers->GetScripts();

		for (UINT Rust = 0; Rust < scripts.Size(); Rust++)
		{
			auto fileLoc = scripts[Rust]->GetFile();
			if (!fileLoc.Get())
				continue;
			bool w = true;
			auto scriptVar = TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>( environment->GetVariable(javaScipt, w, env_var_type::evt_interpretor));
			if (!scriptVar.Get())
				continue;
			scriptVar->SetFile(fileLoc, ret, false);
			if (!ret.returnCode)
				scriptVar->PreProcess(ret);
			if (!ret.returnCode)
				scriptVar->Run();
			if (ret.returnCode)
				continue;
			auto vars = scriptVar->GetVariables();
			TDataEntry<TcVariableHolder> varHolder;
			for (UINT C = 0; vars.GetEntryAt(C, varHolder); C++)
			{
				environment->AddVariable(varHolder.key, varHolder.object.value);
			}
		}
	}
	return TString();
}

