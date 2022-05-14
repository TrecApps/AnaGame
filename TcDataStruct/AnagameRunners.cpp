#include "AnagameRunners.h"
#include <TFile.h>
#include "RunOps.h"

static TDataMap<runner_op_code> opCodeMap;

static TDataArray<AnagameAssembly::AssemblyOp> opList;

void PrepOpCodeMap()
{
    if (opCodeMap.count())
        return;
    opCodeMap.addEntry(L"", runner_op_code::reserved);
    opList.push_back(&AnagameAssembly::Nothing);
    opCodeMap.addEntry(L"line", runner_op_code::line);
    opList.push_back(&AnagameAssembly::Line);
    opCodeMap.addEntry(L"statement", runner_op_code::statement);
    opList.push_back(&AnagameAssembly::Statement);

    opCodeMap.addEntry(L"add_ff", runner_op_code::add_ff);
    opCodeMap.addEntry(L"add_dd", runner_op_code::add_dd);
    opCodeMap.addEntry(L"add_ii", runner_op_code::add_ii);
    opCodeMap.addEntry(L"add_uu", runner_op_code::add_uu);
    opCodeMap.addEntry(L"add_ll", runner_op_code::add_ll);
    opCodeMap.addEntry(L"add_ulul", runner_op_code::add_ulul);
    opCodeMap.addEntry(L"add_oo", runner_op_code::add_oo);
    opList.push_back(&AnagameAssembly::AddFF);
    opList.push_back(&AnagameAssembly::AddDD);
    opList.push_back(&AnagameAssembly::AddII);
    opList.push_back(&AnagameAssembly::AddUU);
    opList.push_back(&AnagameAssembly::AddLL);
    opList.push_back(&AnagameAssembly::AddULUL);
    opList.push_back(&AnagameAssembly::AddOO);

    opCodeMap.addEntry(L"sub_ff", runner_op_code::sub_ff);
    opCodeMap.addEntry(L"sub_dd", runner_op_code::sub_dd);
    opCodeMap.addEntry(L"sub_ii", runner_op_code::sub_ii);
    opCodeMap.addEntry(L"sub_uu", runner_op_code::sub_uu);
    opCodeMap.addEntry(L"sub_ll", runner_op_code::sub_ll);
    opCodeMap.addEntry(L"sub_ulul", runner_op_code::sub_ulul);
    opCodeMap.addEntry(L"sub_oo", runner_op_code::sub_oo);
    opList.push_back(&AnagameAssembly::SubFF);
    opList.push_back(&AnagameAssembly::SubDD);
    opList.push_back(&AnagameAssembly::SubII);
    opList.push_back(&AnagameAssembly::SubUU);
    opList.push_back(&AnagameAssembly::SubLL);
    opList.push_back(&AnagameAssembly::SubULUL);
    opList.push_back(&AnagameAssembly::SubOO);

    opCodeMap.addEntry(L"mul_ff", runner_op_code::mul_ff);
    opCodeMap.addEntry(L"mul_dd", runner_op_code::mul_dd);
    opCodeMap.addEntry(L"mul_ii", runner_op_code::mul_ii);
    opCodeMap.addEntry(L"mul_uu", runner_op_code::mul_uu);
    opCodeMap.addEntry(L"mul_ll", runner_op_code::mul_ll);
    opCodeMap.addEntry(L"mul_ulul", runner_op_code::mul_ulul);
    opCodeMap.addEntry(L"mul_oo", runner_op_code::mul_oo);
    opList.push_back(&AnagameAssembly::MulFF);
    opList.push_back(&AnagameAssembly::MulDD);
    opList.push_back(&AnagameAssembly::MulII);
    opList.push_back(&AnagameAssembly::MulUU);
    opList.push_back(&AnagameAssembly::MulLL);
    opList.push_back(&AnagameAssembly::MulULUL);
    opList.push_back(&AnagameAssembly::MulOO);

    opCodeMap.addEntry(L"div_ff", runner_op_code::div_ff);
    opCodeMap.addEntry(L"div_dd", runner_op_code::div_dd);
    opCodeMap.addEntry(L"div_ii", runner_op_code::div_ii);
    opCodeMap.addEntry(L"div_uu", runner_op_code::div_uu);
    opCodeMap.addEntry(L"div_ll", runner_op_code::div_ll);
    opCodeMap.addEntry(L"div_ulul", runner_op_code::div_ulul);
    opCodeMap.addEntry(L"div_oo", runner_op_code::div_oo);
    opList.push_back(&AnagameAssembly::DivFF);
    opList.push_back(&AnagameAssembly::DivDD);
    opList.push_back(&AnagameAssembly::DivII);
    opList.push_back(&AnagameAssembly::DivUU);
    opList.push_back(&AnagameAssembly::DivLL);
    opList.push_back(&AnagameAssembly::DivULUL);
    opList.push_back(&AnagameAssembly::DivOO);

    opCodeMap.addEntry(L"mod_ii", runner_op_code::mod_ii);
    opCodeMap.addEntry(L"mod_uu", runner_op_code::mod_uu);
    opCodeMap.addEntry(L"mod_ll", runner_op_code::mod_ll);
    opCodeMap.addEntry(L"mod_ulul", runner_op_code::mod_ulul);
    opCodeMap.addEntry(L"mod_oo", runner_op_code::mod_oo);
    opList.push_back(&AnagameAssembly::ModII);
    opList.push_back(&AnagameAssembly::ModUU);
    opList.push_back(&AnagameAssembly::ModLL);
    opList.push_back(&AnagameAssembly::ModULUL);
    opList.push_back(&AnagameAssembly::ModOO);
}



ReturnObject AnagameRunner::GenerateRunners(TDataMap<TrecSubPointer<TVariable, AnagameRunner>>& runners, TrecPointer<TFileShell> file)
{
    PrepOpCodeMap();

    if (!file.Get())
    {
        ReturnObject ret;
        ret.errorMessage.Set(L"Can't Generate Runners: Null File Pointer Provided!");
        ret.returnCode = ret.ERR_INVALID_FILE_PARAM;
        return ret;
    }

    TFile input(file->GetPath(), TFile::t_file_open_existing | TFile::t_file_read);

    if (!input.IsOpen())
    {
        ReturnObject ret;
        ret.errorMessage.Format(L"Can't Generate Runners: File %ws Can't be opened!", file->GetName().GetConstantBuffer().getBuffer());
        ret.returnCode = ret.ERR_INVALID_FILE_PARAM;
        return ret;
    }

    TString line;

    TrecSubPointer<TVariable, AnagameRunner> runner;

    int i = 0;

    while (input.ReadString(line))
    {
        line.Trim();
        if (line.IsEmpty() || line.StartsWith(L'#'))
            continue;

        if (line.StartsWith(L"__proc__"))
        {
            if (runner.Get())
                runners.addEntry(runner->name, runner);
            runner = TrecPointerKey::GetNewSelfTrecSubPointer<TVariable, AnagameRunner>();

            line.Set(line.GetDelete(i, 0, 8).GetTrim());

            runner->name.Set(line);
            continue;
        }

        if (!runner.Get())
        {
            ReturnObject ret;
            ret.errorMessage.Set(L"Can't Generate Runners: Attempt to add content to an empty Runner!");
            ret.returnCode = ret.ERR_UNSUPPORTED_OP;
            return ret;
        }

        if (line.StartsWith(L"__variables__"))
        {
            line.Set(line.GetDelete(i, 0, 13).GetTrim());

            auto vars = line.split(L" ;");
            for (UINT Rust = 0; Rust < vars->Size(); Rust++)
            {
                runner->variableCites.push_back(vars->at(Rust));
            }
            continue;
        }

        auto ops = line.split(L' ');

        runner_op_code opCode;

        if (opCodeMap.retrieveEntry(ops->at(0), opCode))
        {
            UINT op1 = 0, op2 = 0;

            if (opCode == runner_op_code::statement)
            {
                if (ops->Size() == 1)
                    continue;
                op1 = runner->codeLines.push_back(ops->at(1)) - 1;
            }
            else
            {
                if (ops->Size() > 1)
                    TString::ConvertStringToUint(ops->at(1), op1);
                if (ops->Size() > 2)
                    TString::ConvertStringToUint(ops->at(2), op2);
            }
            runner->code.push_back(RunnerCode(opCode, op1, op2));
        }


    }

    if (runner.Get())
        runners.addEntry(runner->name, runner);

    return ReturnObject();
}

AnagameRunner::AnagameRunner()
{
    isBinary = isObject = false;
    currentCodeLines = currentLine = 0;
}

void AnagameRunner::SetCurrentStack(TrecPointer<BinaryStack> stack)
{
    binaryStack = stack;
}

void AnagameRunner::SetCurrentStack(TrecPointer<ObjectStack> stack)
{
    objectStack = stack;
}

ReturnObject AnagameRunner::Run()
{
    return ReturnObject();
}

void AnagameRunner::SetIntialVariables(TDataArray<TrecPointer<TVariable>>& params)
{
}

void AnagameRunner::SetCurrentLine(UINT line)
{
    this->currentLine = line;
}

void AnagameRunner::SetCurrentCodeLine(UINT line)
{
    currentCodeLines = line;
}

void AnagameRunner::AddOp(const RunnerCode& code)
{
    this->code.push_back(code);
}

void AnagameRunner::AddOp(const TString& str)
{
    for (UINT Rust = 0; Rust < this->strings.Size(); Rust++)
    {
        if (!str.Compare(this->strings[Rust]))
        {
            code.push_back(RunnerCode(runner_op_code::push_str, Rust));
            return;
        }
    }
    code.push_back(RunnerCode(runner_op_code::push_str, strings.push_back(str) - 1));
}

AnagameRunner::RunnerCode::RunnerCode()
{
    operand1 = operand2 = 0;
    op_code = 0;
}

AnagameRunner::RunnerCode::RunnerCode(const RunnerCode& copy)
{
    this->operand1 = copy.operand1;
    this->operand2 = copy.operand2;
    this->op_code = copy.op_code;
}

AnagameRunner::RunnerCode::RunnerCode(runner_op_code code, UINT op1, UINT op2)
{
    op_code = static_cast<BYTE>(code);
    operand1 = op1;
    operand2 = op2;
}
