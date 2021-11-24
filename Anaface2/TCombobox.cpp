#include "TCombobox.h"
#include <TcInterpretor.h>
class TComboBoxUpdater : public TcInterpretor
{
public:

    TComboBoxUpdater(TrecSubPointer<TPage, TCombobox> c) : TcInterpretor(TrecSubPointer<TVariable, TcInterpretor>(), TrecPointer<TEnvironment>())
    {
        assert(c.Get());
        comboBox = c;
    }

    /**
     * Method: TcInterpreotr::CanCompile
     * Purpose: Whether the Compiler is capable of compiling it's statements into a more compact form
     * Parameters: void
     * Returns: COMPILE_TYPE - marker indicating the Compilation the Interpretor can do
     *
     * Attributes: abstract
     *
     * Note: If the Returned Value is 0, then the Interpretor will not Attempt to Compile and is expected to run the Statements AS IS
     */
    virtual COMPILE_TYPE CanCompile()
    {
        return 0;
    }

    /**
     * Method: TcInterpretor::Compile
     * Purpose: Attempts to Compile the Statements into more compact code according to the Specifications provided
     * Parameters: COMPILE_TYPE type - specifications for the code to be compiled
     *              ReturnObject& ret - allows method to report any error information encountered
     * Returns: void
     *
     * Attributes: abstract
     */
    virtual void Compile(COMPILE_TYPE type, ReturnObject& ret)
    {
        ret.returnCode = ret.ERR_UNSUPPORTED_OP;
        ret.errorMessage.Set(L"This Interpretor does not Process Compilation");
    }

    /**
     * Method: TcInterpretor::SetFile
     * Purpose: Sets the File Where Sourcecode is expected to be read
     * Parameters: TrecPointer<TFileShell> codeFile - file holding the intitial source code
     *              ReturnObject& ret - allows method to report any error information encountered
     *              bool isFirst - whether this is the first Interpretor called (true by default)
     * Returns: void
     *
     * Attributes: abstract
     */
    virtual void SetFile(TrecPointer<TFileShell> codeFile, ReturnObject& ret, bool isFirst = true)
    {
        ret.returnCode = ret.ERR_UNSUPPORTED_OP;
        ret.errorMessage.Set(L"This Interpretor does not Process Files");
    }

    /**
     * Method: TcInterpretor::Run
     * Purpose: Allows the Interpretor to run the Code As Is
     * Parameters: void
     * Returns: ReturnObject - information about the Run
     */
    virtual ReturnObject Run()
    {
        ReturnObject ret;
        if (!comboBox.Get())
        {
            ret.returnCode = ret.ERR_BROKEN_REF;
            ret.errorMessage.Set(L"Detached ComboBox detected!");
        }
        else
        {
            if (defaultVars.Size())
            {
                comboBox->SetCurrentContent(defaultVars[0]);
            }
        }
        return ret;
    }

    /**
     * Method: TcInterpretor::SetInitialVariables
     * Purpose: Allows Callers to Set Intial Variables, i.e. parameters in a function/method call
     * Parameters: TDataArray<TrecPointer<TVariable>>& params - the parameters to set when calling the function
     * Returns: void
     */
    virtual void SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
    {
        this->defaultVars = params;
    }

    /**
     * Method: TcInterpretor::PreProcess
     * Purpose: Allows Interpretors to do a brief scan of the code and id basic syntax errors
     * Parameters: ReturnObject& ret - info about issues detected
     * Returns: void
     */
    virtual void PreProcess(ReturnObject& ret) 
    {

    }


    /**
     * Method: TcInterpretor::ProcessIndividualStatement
     * Purpose: Allows an Interpretor to process a statement
     * Parameters: const TString& statement - the statemnt to process
     *              ReturnObject& ret - offers return information
     * Returns: void
     *
     * Attributes: abstract
     */
    virtual void ProcessIndividualStatement(const TString& statement, ReturnObject& ret)
    {
        ret.returnCode = ret.ERR_UNSUPPORTED_OP;
        ret.errorMessage.Set(L"This Interpretor does not Process Expressions");
    }

private:

    TrecSubPointer<TPage, TCombobox> comboBox;
};



TCombobox::TCombobox(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta) :TGadget(rt, ta)
{
    vars = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);
    dataLayout = TrecPointerKey::GetNewSelfTrecSubPointer<TPage, TDataLayout>(rt, ta);

    childHeight = 30;
}

TCombobox::~TCombobox()
{
}

bool TCombobox::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)
{
    TObjectLocker lock(&this->thread);
    TString valpoint;
    if (attributes.retrieveEntry(L"DefaultText", valpoint))
        attributes.addEntry(L"Caption", valpoint);

    if (attributes.retrieveEntry(L"SubHeight", valpoint))
        TString::ConvertStringToUint(valpoint, childHeight);

    UINT Rust = 0;
    TString boxAtt;
    for (boxAtt.Format(L"BoxEntry%d", Rust); attributes.retrieveEntry(boxAtt, valpoint); boxAtt.Format(L"BoxEntry%d", ++Rust))
    {
        vars->AppendValue(TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TString>(valpoint));
    }

    dataLayout->SetData(TrecPointerKey::GetTrecPointerFromSub<>(vars));

    valpoint.Format(L"%d", childHeight);
    dataLayout->AddAttribute(L"RowHeight", valpoint);

    TDataEntry<TString> entry;
    for (UINT Rust = 0; attributes.GetEntryAt(Rust, entry); Rust++)
    {
        if (entry.key.StartsWith(L"Sub"))
            dataLayout->AddAttribute(TString(L'|') + entry.key.SubString(4), entry.object);
    }
    D2D1_RECT_F subLoc = loc;
    subLoc.top = subLoc.bottom;
    subLoc.bottom = 1000.0f;
    dataLayout->onCreate(subLoc, d3d);
    TGadget::onCreate(loc, d3d);



    return false;
}

void TCombobox::Draw(TrecPointer<TVariable> object)
{
    if (!isActive)
        return;
    TGadget::Draw(currentVariable.Get() ? currentVariable : object);

    D2D1_POINT_2F leftpoint = D2D1::Point2F(DxLocation.left, DxLocation.top);
    D2D1_POINT_2F rightPoint = D2D1::Point2F(DxLocation.right, DxLocation.top);

    float midDx = DxLocation.left + ((DxLocation.right - DxLocation.left) / 2);

    D2D1_POINT_2F vertexPoint = D2D1::Point2F(midDx, DxLocation.bottom);

    brush->DrawLine(leftpoint, vertexPoint);
    brush->DrawLine(rightPoint, vertexPoint);
}

void TCombobox::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& args)
{
    TGadget::OnResize(newLoc, nFlags, eventAr, args);
    D2D1_RECT_F subLoc = newLoc;
    subLoc.top = subLoc.bottom;
    subLoc.bottom = 1000.0f;
    dataLayout->OnResize(subLoc, nFlags, eventAr, args);
}

void TCombobox::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>& args)
{
    if (IsContained(point, area) && this->isLeftClicked)
    {
        int flyOut = HasEvent(R_Message_Type::On_Flyout);
        if (flyOut != -1)
        {
            this->args.Reset();
            this->args.isClick = true;
            this->args.isLeftClick = true;
            this->args.point = point;
            this->args.eventType = R_Message_Type::On_Flyout;
            this->args.text = currentVariable.Get() ? currentVariable->GetString() : L"null";
            this->args.methodID = flyOut;
            args.push_back(this->args);
        }
        EventID_Cred cred;
        cred.eventType = R_Message_Type::On_Flyout;
        cred.control = TrecPointerKey::GetTrecPointerFromSub<>(dataLayout);
        cred.data = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TComboBoxUpdater>(
            TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TCombobox>(TrecPointerKey::GetTrecPointerFromSoft<>(self)));

        eventAr.push_back(cred);
    }
    TGadget::OnLButtonUp(nFlags, point, mOut, eventAr, args);
}

TrecSubPointer<TVariable, TContainerVariable> TCombobox::GetVariableList()
{
    return vars;
}

void TCombobox::SetCurrentContent(TrecPointer<TVariable> v)
{
    if (v.Get())
        currentVariable = v;
}
