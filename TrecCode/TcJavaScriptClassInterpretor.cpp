#include "TcJavaScriptClassInterpretor.h"
#include <TAccessorVariable.h>

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

void TcJavaScriptClassInterpretor::ProcessStatements(ReturnObject& ret)
{
	PreProcess(ret);
	if (ret.returnCode)
		return;

	if (!statements.Size())
		return;

	// there must only be one constructor in a JavaScript class
	bool hasConstructor = false;
	
	for (UINT Rust = 0; Rust < statements.Size(); Rust++)
	{
		TrecPointer<CodeStatement> state = statements[Rust];

		while (state.Get())
		{
			TString startStatement(state->statement);
			int startParenth = startStatement.Find(L'(');

			if (startStatement.StartsWith(L"constructor", false, true) || startStatement.StartsWith(L"constructor("))
			{
				if (hasConstructor)
				{
					ret.returnCode = ret.ERR_IMPROPER_NAME;
					ret.errorMessage.Format(L"Error! Second constructor token found within the %ws class", className.GetConstantBuffer());
					return;
				}
				hasConstructor = true;
				if (startParenth == -1 || startStatement.SubString(11, startParenth).GetTrim().GetSize())
				{
					ret.returnCode = ret.ERR_UNEXPECTED_TOK;
					ret.errorMessage.Set(L"Unexpected token after 'constructor' keyword!");

					// To-Do: Add Stack code


					return;
				}

				ProcessMethod(L"constructor", state, ret, 0);
				if (ret.returnCode)
					return;

				state = state->next;
				continue;
			}

			UCHAR attributes = 0;
			bool inspectAtts = false;
			do {
				inspectAtts = false;
				if (startStatement.StartsWith(L"static", false, true))
				{
					attributes |= ATTRIBUTE_STATIC;
					startStatement.Delete(0, 6);
					startStatement.Trim();
					inspectAtts = true;
				}

				if (startStatement.StartsWith(L"const", false, true))
				{
					attributes |= ATTRIBUTE_CONST;
					startStatement.Delete(0, 5);
					startStatement.Trim();
					inspectAtts = true;
				}

				if (startStatement.StartsWith(L"get", false, true))
				{
					attributes |= ATTRIBUTE_GETTER;
					startStatement.Delete(0, 3);
					startStatement.Trim();
					inspectAtts = true;
				}

				if (startStatement.StartsWith(L"set", false, true))
				{
					attributes |= ATTRIBUTE_SETTER;
					startStatement.Delete(0, 3);
					startStatement.Trim();
					inspectAtts = true;
				}

			} while (inspectAtts);

			if (startParenth != -1)
			{
				TString name(startStatement.SubString(0, startParenth).GetTrim());

				if (!name.GetSize())
				{
					ret.returnCode = ret.ERR_IMPROPER_NAME;
					ret.errorMessage.Format(L"Error! No name provided to likely method in %ws class!", className.GetConstantBuffer());
					return;
				}

				CheckVarName(name,ret);
				if (ret.returnCode)
					return;
				ProcessMethod(name, state, ret, attributes);
			}
			else
			{
				// dealing with a regular variable
				auto pieces = startStatement.splitn(L"=", 2);

				TString name(pieces->at(0).GetTrim());

				if (attributes && !name.GetSize())
				{
					ret.returnCode = ret.ERR_IMPROPER_NAME;
					ret.errorMessage.Format(L"Error! Unexpected tokens where variable name was expected in %ws class!", className.GetConstantBuffer());
					return;
				}

				TrecPointer<TVariable> var;
				if (pieces->Size() == 2)
				{
					UINT parenth = 0, square = 0, index = state->statement.Find(L'=') + 1;
					ProcessExpression(parenth, square, index, state, ret);
					if (ret.returnCode)
						return;

					var = ret.errorObject;
				}

				TClassAttribute constructor;
				int i;
				constructor.name.Set(name.GetReplace(i, L";", L""));
				constructor.def = var;
				constructor.other = attributes;
				classData.AddAttribute(constructor);
			}

			state = state->next;
		}
	}
}

TClassStruct TcJavaScriptClassInterpretor::GetClassData()
{
	if (superName.GetSize())
		classData.AddParentClass(superName);
	else
		classData.AddParentClass(L"Object");
	return classData;
}

void TcJavaScriptClassInterpretor::ProcessMethod(const TString& methodName, TrecPointer<CodeStatement> state, ReturnObject& ro, UCHAR att)
{
	UINT parenth = 0, square = 0, index = state->statement.Find(L'(');
	ProcessFunctionExpression(parenth, square, index, state, ro);

	if (ro.returnCode)
		return;

	TrecPointer<TVariable> superConstructor;
	if (!methodName.Compare(L"constructor") && this->superName.GetSize())
	{
		TClassAttribute att = superData.GetAttributeByName(L"constructor");
		if (!att.name.Compare(L"constructor"))
		{
			superConstructor = att.def;
		}
	}

	if (dynamic_cast<TcJavaScriptInterpretor*>(superConstructor.Get()))
	{
		TClassAttribute super;
		super.name.Set(L"super");
		super.def = superConstructor;
		classData.AddAttribute(super);

		//block->variables.addEntry(L"super", TVariableMarker(false, superConstructor));
		// dynamic_cast<TJavaScriptInterpretor*>(superConstructor.Get())->variables.addEntry(L"this", TVariableMarker(false, ))
	}

	// Handle Get Vs Set
	if (att & ATTRIBUTE_GETTER)
	{
		// dealing with a getter
		TClassAttribute accessAtt = classData.GetAttributeByName(methodName);

		if (accessAtt.name.GetSize())
		{
			if (!accessAtt.def.Get() || accessAtt.def->GetVarType() != var_type::accessor)
			{
				ro.returnCode = ReturnObject::ERR_EXISTING_VAR;
				ro.errorMessage.Format(L"Error Setting Getter to %ws class, Attribute %ws already set as something other than a getter or a setter!", className.GetConstantBuffer(), methodName.GetConstantBuffer());
				return;
			}
		}
		else
		{
			// This attribute has not previously been encountered
			accessAtt.name.Set(methodName);
			accessAtt.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TAccessorVariable>();
		}

		auto g = dynamic_cast<TAccessorVariable*>(accessAtt.def.Get())->GetCGetter();
		if (g.Get())
		{
			ro.returnCode = ReturnObject::ERR_EXISTING_VAR;
			ro.errorMessage.Format(L"Error Setting Getter to %ws class, Attribute %ws already has a getter!", className.GetConstantBuffer(), methodName.GetConstantBuffer());
			return;
		}

		dynamic_cast<TAccessorVariable*>(accessAtt.def.Get())->SetGetter(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(ro.errorObject));

		accessAtt.other = att;
		classData.AddAttribute(accessAtt, true);
		return;
	}

	// Handle Get Vs Set
	if (att & ATTRIBUTE_SETTER)
	{
		// dealing with a getter
		TClassAttribute accessAtt = classData.GetAttributeByName(methodName);

		if (accessAtt.name.GetSize())
		{
			if (!accessAtt.def.Get() || accessAtt.def->GetVarType() != var_type::accessor)
			{
				ro.returnCode = ReturnObject::ERR_EXISTING_VAR;
				ro.errorMessage.Format(L"Error Setting Setter to %ws class, Attribute %ws already set as something other than a getter or a setter!", className.GetConstantBuffer(), methodName.GetConstantBuffer());
				return;
			}
		}
		else
		{
			// This attribute has not previously been encountered
			accessAtt.name.Set(methodName);
			accessAtt.def = TrecPointerKey::GetNewSelfTrecPointerAlt<TVariable, TAccessorVariable>();
		}

		auto g = dynamic_cast<TAccessorVariable*>(accessAtt.def.Get())->GetCSetter();
		if (g.Get())
		{
			ro.returnCode = ReturnObject::ERR_EXISTING_VAR;
			ro.errorMessage.Format(L"Error Setting Setter to %ws class, Attribute %ws already has a setter!", className.GetConstantBuffer(), methodName.GetConstantBuffer());
			return;
		}

		dynamic_cast<TAccessorVariable*>(accessAtt.def.Get())->SetSetter(TrecPointerKey::GetTrecSubPointerFromTrec<TVariable, TcInterpretor>(ro.errorObject));

		accessAtt.other = att;
		classData.AddAttribute(accessAtt, true);
		return;
	}


	TClassAttribute constructor;
	constructor.name.Set(methodName);
	constructor.def = ro.errorObject;
	constructor.other = att;
	classData.AddAttribute(constructor);
}
