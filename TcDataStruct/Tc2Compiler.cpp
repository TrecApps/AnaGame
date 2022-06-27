#include "Tc2Compiler.h"

void Tc2Compiler::CompilerHolder::SetSelf(TrecPointer<CompilerHolder> self)
{
    if (self.Get() != this)
        throw L"Not Matching";
    holder = TrecPointerKey::GetSoftPointerFromTrec<>(self);
}

TrecPointer<Tc2Compiler> Tc2Compiler::CompilerHolder::GetCompilerByFile(const TString& fileName)
{
    TrecPointer<Tc2Compiler> ret;
    fileCompilers.retrieveEntry(fileName, ret);
    return ret;
}

TrecPointer<Tc2Compiler> Tc2Compiler::CompilerHolder::GetCompilerByModule(const TString& moduleName)
{
    TrecPointer<Tc2Compiler> ret;
    moduleCompilers.retrieveEntry(moduleName, ret);
    return ret;
}

TrecPointer<Tc2Compiler> Tc2Compiler::CompilerHolder::SetUpCompiler(const TString& str)
{
    TrecPointer<Tc2Compiler> ret;
    if (fileCompilers.retrieveEntry(str, ret) || moduleCompilers.retrieveEntry(str, ret))
        return ret;
    for (UINT Rust = 0; Rust < languages.Size(); Rust++)
    {
        bool useFile = false;
        TrecPointer<TFileShell> file = languages[Rust]->GetFile(str, this->rootDirectory, this->otherDirectories, useFile);
        if (file.Get())
        {
            ret = TrecPointerKey::GetNewTrecPointer<Tc2Compiler>(TrecPointerKey::GetTrecPointerFromSoft<>(holder), file, languages[Rust]);
            if (useFile)
                fileCompilers.addEntry(str, ret);
            else
                moduleCompilers.addEntry(str, ret);

            break;
        }
    }
    return ret;
}

bool Tc2Compiler::CompilerHolder::SubmitSourceFile(TrecPointer<TFileShell> file)
{
    for (UINT Rust = 0; Rust < languages.Size(); Rust++)
    {
        if (languages[Rust]->SupportsFile(file))
        {
            // To-Do: Add Compiler to compiler-map
            return true;
        }
    }
    return false;
}

void Tc2Compiler::CompilerHolder::Compile(TDataArray<CompileMessage>& messages)
{
    bool worked = true;
    TDataEntry<TrecPointer<Tc2Compiler>> compilerEntry;
    for (UINT Rust = 0; fileCompilers.GetEntryAt(Rust, compilerEntry); Rust++)
    {
        worked &= compilerEntry.object->PreCompile(messages);
    }
    for (UINT Rust = 0; moduleCompilers.GetEntryAt(Rust, compilerEntry); Rust++)
    {
        worked &= compilerEntry.object->PreCompile(messages);
    }

    for (UINT Rust = 0; fileCompilers.GetEntryAt(Rust, compilerEntry); Rust++)
    {
        worked &= compilerEntry.object->ParseStatementTypes(messages);
    }
    for (UINT Rust = 0; moduleCompilers.GetEntryAt(Rust, compilerEntry); Rust++)
    {
        worked &= compilerEntry.object->ParseStatementTypes(messages);
    }
    if (!worked)return;
}

Tc2Compiler::Tc2Compiler(TrecPointer<CompilerHolder> parent, TrecPointer<TFileShell> file, TrecPointer<TcLanguage> language)
{
    this->file = file;
    this->language = language;
    this->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parent);

    stage = 0;
}

bool Tc2Compiler::PreCompile(TDataArray<CompileMessage>& messages)
{
    stage = 0;
    preStatements.RemoveAll();
    TcPreStatement::GenerateStatements(this->preStatements, this->language->GetLanguageComponents(), file, messages);

    for(UINT Rust = 0; Rust < messages.Size(); Rust++)
        if(messages[Rust].isError)
            return false;

    stage = 1; 
    return true;
}

bool Tc2Compiler::ParseStatementTypes(TDataArray<CompileMessage>& messages)
{
    if(!stage)
        return false;
    language->GenerateRegularStatements(baseStatements, preStatements, messages);

    bool worked = true;
    for (UINT Rust = 0; Rust < messages.Size(); Rust++)
    {
        if (messages[Rust].isError)
        {
            worked = false;
            break;
        }
    }
    if (worked)
        stage = 2;
    return worked;
}

bool Tc2Compiler::PrepDependencies(TDataArray<CompileMessage>& messages)
{
    TDataArray<TString> modules = language->GetOtherModules(file);

    TrecPointer<CompilerHolder> holder = TrecPointerKey::GetTrecPointerFromSoft<>(parent);

    bool ret = true;

    for (UINT Rust = 0; Rust < modules.Size(); Rust++)
    {
        auto pieces = modules[Rust].splitn(L':', 2);
        TrecPointer<Tc2Compiler> compiler = holder->SetUpCompiler(pieces->at(1));

        if (!compiler.Get())
        {
            ret = false;
            CompileMessage message;
            message.file = file;
            message.isError = true;
            TString::ConvertStringToUint(pieces->at(0), message.line);
            message.message.Format(L"Could not find File/Module '%ws'!", pieces->at(1));
            messages.push_back(message);
        }
        else
        {
            this->otherCompilers.push_back(TrecPointerKey::GetSoftPointerFromTrec<>( compiler ));
        }
    }

    return ret;
}
