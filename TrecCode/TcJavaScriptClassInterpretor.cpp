#include "TcJavaScriptClassInterpretor.h"

TcJavaScriptClassInterpretor::TcJavaScriptClassInterpretor(TrecSubPointer<TVariable, TcInterpretor> parentInterpretor, TrecPointer<TEnvironment> env):
	TcJavaScriptInterpretor(parentInterpretor, env)
{
}

void TcJavaScriptClassInterpretor::SetClassName(const TString& name)
{
	className.Set(name);
}

bool TcJavaScriptClassInterpretor::SetSuperClassName(const TString& name)
{
	bool ret = dynamic_cast<TcTypeInterpretor*>(parent.Get()) ?
		dynamic_cast<TcTypeInterpretor*>(parent.Get())->GetClass(name, superData) : false;
	if (ret)
	{
		superName.Set(name);
	}
	return ret;
}

TString TcJavaScriptClassInterpretor::GetJsClassName()
{
	return className;
}

void TcJavaScriptClassInterpretor::ProcessStatements(ReturnObject& ro)
{
}

TClassStruct TcJavaScriptClassInterpretor::GetClassData()
{
	if (superName.GetSize())
		classData.AddParentClass(superName);
	else
		classData.AddParentClass(L"Object");
	return classData;
}
