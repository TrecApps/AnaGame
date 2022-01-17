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

class TComboBoxExtension : public TDataLayout
{
    friend class TCombobox;
protected:

    TrecSubPointerSoft<TPage, TCombobox> combobox;

public:

    TComboBoxExtension(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles) : TDataLayout(drawingBoard, styles)
    {

    }

    virtual void OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& eventAr)
    {
        bool isCurClick = this->isLeftClicked;
        TControl::OnLButtonUp(nFlags, point, mOut, eventAr);

        if (mOut == message_output::mo_negative)
            return;

        TString index(HasEvent(isCurClick ? R_Message_Type::On_Click : R_Message_Type::On_L_Button_Up));
        if (index.GetSize())
        {
            int r = 0; int c = 0;
            int iindex = -1;
            if (GetIndex(point, r, c))
            {
                iindex = ConvertCoordinates(r, c);

                if (isCurClick && dynamic_cast<TContainerVariable*>(var.Get()))
                {
                    EventID_Cred cred(R_Message_Type::On_Click, TrecPointerKey::GetTrecPointerFromSoft<>(self));
                    cred.data = dynamic_cast<TContainerVariable*>(this->var.Get())->GetValueAt(iindex);
                    eventAr.push_back(cred);
                }
            }
            eventAr.at(eventAr.Size() - 1).args->arrayLabel = iindex;
        }
        int r = 0; int c = 0;

        if (isCurClick && GetIndex(point, r, c))
        {
            int iindex = ConvertCoordinates(r, c);

            EventID_Cred cred(R_Message_Type::On_sel_change, TrecPointerKey::GetTrecPointerFromSoft<>(self));
            cred.args = TrecPointerKey::GetNewTrecPointer<EventArgs>(this->args);
            if (dynamic_cast<TContainerVariable*>(this->var.Get()))
            {

                auto updater = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TComboBoxUpdater>(TrecPointerKey::GetSubPointerFromSoft(combobox));
                TDataArray<TrecPointer<TVariable>> vars;
                vars.push_back(dynamic_cast<TContainerVariable*>(this->var.Get())->GetValueAt(iindex));
                updater->SetIntialVariables(vars);

                cred.data = TrecSubToTrec(updater);
            }


            index.Set(HasEvent(R_Message_Type::On_sel_change));
            if (index.GetSize())
            {

                int iindex = ConvertCoordinates(r, c);

                this->args.Reset();
                this->args.arrayLabel = iindex;
                this->args.eventType = R_Message_Type::On_sel_change;
                this->args.isClick = true;
                this->args.isLeftClick = true;
                this->args.methodID = index;
                this->args.point = point;
                this->args.positive = true;
                this->args.type = L'\0';

                cred.args = TrecPointerKey::GetNewTrecPointer<EventArgs>(this->args);
                
            }


            eventAr.push_back(cred);
        }

        if (isCurClick)
        {
            
        }

    }
};

TCombobox::TCombobox(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta) :TGadget(rt, ta)
{
    vars = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, TContainerVariable>(ContainerType::ct_array);
    auto extPage = TrecPointerKey::GetNewSelfTrecPointerAlt<TPage, TComboBoxExtension>(rt, ta);
    dataLayout = TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TDataLayout>(extPage);

    childHeight = 30;
}

TCombobox::~TCombobox()
{
}

bool TCombobox::onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d)
{
    TObjectLocker lock(&this->thread);

    dynamic_cast<TComboBoxExtension*>(dataLayout.Get())->combobox = TrecPointerKey::GetSoftSubPointerFromSoft<TPage, TCombobox>(self);

    TString valpoint;
    if (attributes.retrieveEntry(L"DefaultText", valpoint))
    {
        attributes.addEntry(L"Caption", valpoint);
        defaultText.Set(valpoint);
    }
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

void TCombobox::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr)
{
    TGadget::OnResize(newLoc, nFlags, eventAr);
    D2D1_RECT_F subLoc = newLoc;
    subLoc.top = subLoc.bottom;
    subLoc.bottom = 1000.0f;
    dataLayout->OnResize(subLoc, nFlags, eventAr);
}

void TCombobox::OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>& eventAr)
{
    if (IsContained(point, area) && this->isLeftClicked)
    {
        TString flyOut = HasEvent(R_Message_Type::On_Flyout);
        if (flyOut.GetSize())
        {
            this->args.Reset();
            this->args.isClick = true;
            this->args.isLeftClick = true;
            this->args.point = point;
            this->args.eventType = R_Message_Type::On_Flyout;
            this->args.text = currentVariable.Get() ? currentVariable->GetString() : L"null";
            this->args.methodID = flyOut;
            EventID_Cred cred(R_Message_Type::On_Flyout, TrecPointerKey::GetTrecPointerFromSoft<>(self));
            cred.args = TrecPointerKey::GetNewTrecPointer<EventArgs>(this->args);

            eventAr.push_back(cred);
        }
        EventID_Cred cred;
        cred.eventType = R_Message_Type::On_Flyout;
        cred.control = TrecPointerKey::GetTrecPointerFromSub<>(dataLayout);
        cred.data = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TComboBoxUpdater>(
            TrecPointerKey::GetTrecSubPointerFromTrec<TPage, TCombobox>(TrecPointerKey::GetTrecPointerFromSoft<>(self)));

        eventAr.push_back(cred);
    }
    TGadget::OnLButtonUp(nFlags, point, mOut, eventAr);
}

TrecSubPointer<TVariable, TContainerVariable> TCombobox::GetVariableList()
{
    return vars;
}

bool TCombobox::InjectChildTemplate(TrecPointer<TPage> page)
{
    return dataLayout->InjectChildTemplate(page);
}

bool TCombobox::SupportsChildTemplateInjection()
{
    return true;
}

TrecPointer<TVariable> TCombobox::GetCurrentVariable()
{
    return currentVariable;
}

void TCombobox::SetCurrentContent(TrecPointer<TVariable> v)
{
    if (v.Get())
    {
        currentVariable = v;
    }
    if (text.Get())
        text->SetText(v.Get() ? v->GetString() : defaultText);
}
