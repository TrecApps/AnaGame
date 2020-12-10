#include "TJavaScriptClassInterpretor.h"

TJavaScriptClassInterpretor::TJavaScriptClassInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor, TrecPointer<TEnvironment> env)
	: TJavaScriptInterpretor(parentInterpretor, env)
{
}
