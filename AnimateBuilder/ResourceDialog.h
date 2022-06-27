#pragma once
#include <TDialog.h>


/**
 * Class: ResourceDialog
 * Purpsoe: Allows users to enter a name into a Dialog for use in a main application
 *
 * SuperClass: TDialog
 */
class ResourceDialog :
    public TDialog
{
public:
    ResourceDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TProcess> ins, TDialogMode mode);
    virtual ~ResourceDialog();
    virtual TString GetType()override;
    int CompileView(TrecComPointer<ID2D1Factory1> fact)override;
    bool OnDestroy() override;


};
