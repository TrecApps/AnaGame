#include "pch.h"
#include "HtmlBody.h"

HtmlBody::HtmlBody(TrecPointer<TEnvironment> env, TrecPointer<DrawingBoard> board)
{
    this->env = env;
    this->board = board;
}

TString HtmlBody::ProcessHtml(TrecPointer<TFile> file, const TString& data, HWND win)
{
    if (!file.Get() || file->IsOpen())
    {
        return L"File Not available!";
    }
    TString nodeData(data);
    TString lineData;
    while (file->ReadString(lineData))
    {
        int endTag = lineData.Find(L"</body>");

        if (endTag != -1)
        {
            // Reset the file for further process ing (in case the head block came after the body block)
            file->Seek(file->GetPosition() - (static_cast<unsigned long long>(lineData.GetSize()) - (endTag + 7)), 0);
        }

        nodeData.Append(lineData);
    }

    bodyContents = TrecPointerKey::GetNewSelfTrecSubPointer<TSliceManager, TStringSliceManager>(nodeData);

    rootNode = TrecPointerKey::GetNewSelfTrecPointer<TWebNode>(board);
    UINT start = 0;
    if(rootNode->ProcessHtml(*bodyContents.Get(), start, win))
        return L"Error Processing Web Nodes!";
    return TString();
}


TrecPointer<TWebNode> HtmlBody::RetrieveWebNode()
{
    return rootNode;
}