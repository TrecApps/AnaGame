#include "Tc2Compiler.h"

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

Tc2Compiler::Tc2Compiler(TrecPointer<CompilerHolder> parent, TrecPointer<TFileShell> file, TrecPointer<TcLanguage> language)
{
    this->file = file;
    this->language = language;
    this->parent = TrecPointerKey::GetSoftPointerFromTrec<>(parent);
}
