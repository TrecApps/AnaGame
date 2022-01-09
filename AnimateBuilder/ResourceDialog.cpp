#include "ResourceDialog.h"
#include <DirectoryInterface.h>

ResourceDialog::ResourceDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TProcess> ins, TDialogMode mode) :
    TDialog(name, winClass, style, parent, commandShow, ins, mode)
{
}

ResourceDialog::~ResourceDialog()
{
}

TString ResourceDialog::GetType()
{
    return TString(L"ResourceDialog;") + TDialog::GetType();
}

int ResourceDialog::CompileView(TrecComPointer<ID2D1Factory1> fact)
{
    TObjectLocker lock(&thread);

    TString file = GetDirectoryWithSlash(CentralDirectories::cd_Executable);

    file.Append(L"Resources\\NewResource.json");

    TrecPointer<TPage::EventHandler> eh;

	int returnable = TWindow::CompileView(file, eh);

	if (returnable)
	{
		ThreadRelease();
		return returnable;
	}
	if (!mainPage.Get())
	{
		ThreadRelease();
		return 10;
	}

    return 0;
}

bool ResourceDialog::OnDestroy()
{
	TObjectLocker lock(&thread);
	bool ret = TDialog::OnDestroy();
    return ret;
}
