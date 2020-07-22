#include "TShaderParser.h"

TShaderParser::TShaderParser(TShaderHost* ae, TString directory, TrecComPointer<ID3D11Device> dev)
{
	if (!ae)
		throw L"Error! Expected an initialized Shader Host!";
	if (!dev.Get())
		throw L"Error! Expected Graphics Device to be initialized!";
	shaderHost = ae;
	this->directory.Set(directory);

	shaderIndex = shaderHost->InitializeShader();
	device = dev;
	bsd.vertexFileSet = bsd.pixelFileSet = bsd.vertexFunctionSet = bsd.pixelFunctionSet = false;

	cbd.bufferSlot = 0;
	cbd.ModelBuffer = false;
	cbd.purpose = 0;
	cbd.size = 0;
	cbd.sp = static_cast<ShaderPhase>(0);

	desc = 0;

	shaderIDd = false;
}

TShaderParser::~TShaderParser()
{
}

bool TShaderParser::Obj(TString* v)
{
	if (!v)
		return false;
	if (!v->Compare(L"Buff"))
	{
		bufferDesc.push_back(desc);
		desc = 0;
		return true;
	}
	else if (!v->Compare(L"Const"))
	{
		TrecComPointer<ID3D11Buffer> buff;
		if (shaderHost->GetConstantBuffer(cbd.size, buff, device))
			return false;
		
		bool newConst = shaderHost->SetNewConstantBuffer(buff, cbd.bufferSlot, cbd.ModelBuffer, cbd.sp, shaderIndex, cbd.purpose);

		return newConst;
	}

	return false;
}

bool TShaderParser::Attribute(TrecPointer<TString> v, TString& e)
{
	// Quick Null check so we don't have to worry about it later down the line
	if (!v.Get())
		return false;

	if (!TString::Compare(e, L"|InputSlot"))
	{
		if (!v.Get())
			return false;
		return SetInputSlot(*v.Get());
	}
	else if (!TString::Compare(e, L"|BufferPurpose"))
	{
		if (!v.Get())
			return false;
		return SetBufferPurpose(*v.Get());
	}
	else if (!TString::Compare(e, L"|BufferWidth"))
	{
		if (!v.Get())
			return false;
		return SetDataWidth(*v.Get());
	}
	else if (!TString::Compare(e, L"|BufferSize"))
	{
		if (!v.Get())
			return false;
		return SetDataCount(*v.Get());
	}
	else if (!TString::Compare(e, L"|InputBufferCount"))
	{

	}
	else if (!TString::Compare(e, L"|PixelFile"))
	{
		TrecPointer<TString> sh_str = TrecPointerKey::GetNewTrecPointer<TString>(directory);
		sh_str->Append(L"\\");
		sh_str->Append(v.Get());
		bsd.pixelFile = sh_str;
		bsd.pixelFileSet = true;
	}
	else if (!TString::Compare(e, L"|VertexFile"))
	{
		TrecPointer<TString> sh_str = TrecPointerKey::GetNewTrecPointer<TString>(directory);
		sh_str->Append(L"\\");
		sh_str->Append(v.Get());
		bsd.vertexFile = sh_str;
		bsd.vertexFileSet = true;
	}
	else if (!TString::Compare(e, L"|PixelFunction"))
	{
		bsd.pixelFunction = v;
		bsd.pixelFunctionSet = true;
	}
	else if (!TString::Compare(e, L"|VertexFunction"))
	{
		bsd.vertexFunction = v;
		bsd.vertexFunctionSet = true;
	}

	else if (shaderIDd)
	{
		if (!TString::Compare(e, L"|DomainFile"))
		{
			TrecPointer<TString> sh_str = TrecPointerKey::GetNewTrecPointer<TString>(directory);
			sh_str->Append(L"\\");
			sh_str->Append(v.Get());
			shaderFile = sh_str;
			phase =ShaderPhase::sp_Domain;
		}
		else if (!TString::Compare(e, L"|DomainFunction"))
		{
			if (phase ==ShaderPhase::sp_Domain)
				return AddShaderToProgram(*v.Get());
		}
		else if (!TString::Compare(e, L"|ComputeFile"))
		{
			TrecPointer<TString> sh_str = TrecPointerKey::GetNewTrecPointer<TString>(directory);
			sh_str->Append(L"\\");
			sh_str->Append(v.Get());
			shaderFile = sh_str;
			phase =ShaderPhase::sp_Compute;
		}
		else if (!TString::Compare(e, L"|ComputeFunction"))
		{
			if (phase ==ShaderPhase::sp_Compute)
				return AddShaderToProgram(*v.Get());
		}
		if (!TString::Compare(e, L"|HullFile"))
		{
			TrecPointer<TString> sh_str = TrecPointerKey::GetNewTrecPointer<TString>(directory);
			sh_str->Append(L"\\");
			sh_str->Append(v.Get());
			shaderFile = sh_str;
			phase =ShaderPhase::sp_Hull;
		}
		else if (!TString::Compare(e, L"|HullFunction"))
		{
			if (phase ==ShaderPhase::sp_Hull)
				return AddShaderToProgram(*v.Get());
		}
		if (!TString::Compare(e, L"|GeometryFile"))
		{
			TrecPointer<TString> sh_str = TrecPointerKey::GetNewTrecPointer<TString>(directory);
			sh_str->Append(L"\\");
			sh_str->Append(v.Get());
			shaderFile = sh_str;
			phase =ShaderPhase::sp_Geometry;
		}
		else if (!TString::Compare(e, L"|GeometryFunction"))
		{
			if (phase ==ShaderPhase::sp_Geometry)
				return AddShaderToProgram(*v.Get());
		}
		else if (!TString::Compare(e, L"|TextureCount"))
		{
			return SetTextureCount(*v.Get());

		}
		else if (!TString::Compare(e, L"|ModelBuffer"))
		{
			if (!v->Compare(L"True"))
				cbd.ModelBuffer = true;
		}
		else if (!TString::Compare(e, L"|ConstantSize"))
		{
			int value = 0;
			if (!v->ConvertToInt(value))
			{
				cbd.size = value;
			}
			else
				return false;
		}
		else if (!TString::Compare(e, L"|ConstantShaderPhase"))
		{
			if (!v->Compare(L"Pixel") || !v->Compare(L"Fragment"))
			{
				cbd.sp =ShaderPhase::sp_Pixel;
			}
			else if (!v->Compare(L"Hull"))
			{
				cbd.sp =ShaderPhase::sp_Hull;
			}
			else if (!v->Compare(L"Domain"))
			{
				cbd.sp =ShaderPhase::sp_Domain;
			}
			else if (!v->Compare(L"Compute"))
			{
				cbd.sp =ShaderPhase::sp_Compute;
			}
			else if (!v->Compare(L"Geometry"))
			{
				cbd.sp = ShaderPhase::sp_Geometry;
			}
			else
			{
				cbd.sp = ShaderPhase::sp_Vertex;
			}

		}
		else if (!TString::Compare(e, L"|ConstantSlot"))
		{
			int value = 0;
			if (!v->ConvertToInt(value))
			{
				if (value >= 0 && value < 16)
				{
					cbd.bufferSlot = value;
					return true;
				}
			}
			return false;
		}
		else if (!TString::Compare(e, L"|ConstantPurpose"))
		{
			if (!v->Compare(L"Color"))
			{
				cbd.purpose = 1;
			}
			else if (!v->Compare(L"Camera"))
			{
				cbd.purpose = 2;
			}
			else if (!v->Compare(L"MVP"))
				cbd.purpose = 3;
			else if (!v->Compare(L"View"))
				cbd.purpose = 4;
			else if (!v->Compare(L"Model"))
				cbd.purpose = 5;
		}
	}

	return SetBasicShader();
}

bool TShaderParser::submitType(TString v)
{
	return false;
}

bool TShaderParser::submitEdition(TString v)
{
	return false;
}

bool TShaderParser::goChild()
{
	return false;
}

void TShaderParser::goParent()
{
}

UCHAR* TShaderParser::GetAnaGameType()
{
	return nullptr;
}

/*
* Method: ShaderParser
* Purpose:
* Parameters:
* Return:
*/
bool TShaderParser::AddShaderToProgram(TString& str)
{
	std::string cStr = str.GetRegString();

	int err = 0;

	switch (phase)
	{
	case ShaderPhase::sp_Domain:
		err = shaderHost->AddDomainShader(device, shaderIndex, *shaderFile.Get(), cStr.c_str());
		break;
	case ShaderPhase::sp_Compute:
		err = shaderHost->AddComputeShader(device, shaderIndex, *shaderFile.Get(), cStr.c_str());
		break;
	case ShaderPhase::sp_Hull:
		err = shaderHost->AddHullShader(device, shaderIndex, *shaderFile.Get(), cStr.c_str());
		break;
	case ShaderPhase::sp_Geometry:
		err = shaderHost->AddGeometryShader(device, shaderIndex, *shaderFile.Get(), cStr.c_str());
		break;
	}

	return !err;
}

/*
* Method: ShaderParser
* Purpose:
* Parameters:
* Return:
*/
bool TShaderParser::SetBasicShader()
{
	if (bsd.vertexFileSet && bsd.pixelFileSet && bsd.vertexFunctionSet && bsd.pixelFunctionSet && !shaderIDd)
	{
		std::string cStr2 = bsd.vertexFunction->GetRegString();
		std::string cStr = bsd.pixelFunction->GetRegString();
		
		
		int err = shaderHost->SetNewBasicShader(device, shaderIndex, *bsd.vertexFile.Get(), *bsd.pixelFile.Get(), cStr2.c_str(), cStr.c_str(), bufferDesc);

		shaderIDd = err == 0;
		return shaderIDd;
	}
	return true;
}

/*
* Method: ShaderParser
* Purpose: Sets the semantic purpose of a vertex buffer entry
* Parameters: TString & v - semantic buffer purpose in string form
* Return: bool - whether the string was a valid purpose
*/
bool TShaderParser::SetBufferPurpose(TString& t)
{
	if (!t.Compare(L"Position"))
	{
		desc = desc | 0b00000011;
	}
	else if (!t.Compare(L"Texture"))
	{
		desc = desc | 0b00000010;
	}
	else if (!t.Compare(L"Normal"))
	{
		desc = desc | 0b00000001;
	}
	else if (!t.Compare(L"Color"))
	{
		desc = desc | 0b00000000;
	}
	else if (!t.Compare(L"Binormal"))
	{
		desc = desc | static_cast<unsigned char>(4);
	}
	else if (!t.Compare(L"Blendindex"))
	{
		desc = desc | static_cast<unsigned char>(6);
	}
	else if (!t.Compare(L"Blendweight"))
	{
		desc = desc | static_cast<unsigned char>(5);
	}
	else if (!t.Compare(L"T Position"))
	{
		desc = desc | static_cast<unsigned char>(7);
	}
	else if (!t.Compare(L"Tangent"))
	{
		desc = desc | static_cast<unsigned char>(8);
	}
	else if (!t.Compare(L"Fog"))
	{
		desc = desc | static_cast<unsigned char>(9);
	}
	else if (!t.Compare(L"Tessfactor"))
	{
		desc = desc | static_cast<unsigned char>(10);
	}
	else return false;


	return true;
}

/*
* Method: ShaderParser - SetInputSlot
* Purpose: Determines the input slot of a constant buffer
* Parameters: TString & v - number in string form
* Return: bool - success, whether the string was a number
*/
bool TShaderParser::SetInputSlot(TString& v)
{
	int value = 0;
	if (!v.ConvertToInt(value))
	{
		if (value < 0 || value > 15)
			return false;

		unsigned short cValue = static_cast<unsigned short>(value);
		desc = desc + (cValue << 8);
		return true;
	}
	return false;
}

/*
* Method: ShaderParser - SetDataWidth
* Purpose: Sets the size of a field that a vertex buffer expects
* Parameters: TString & v - number in string form
* Return: bool - success, whether the string was a number
*/
bool TShaderParser::SetDataWidth(TString& v)
{
	int value = 0;
	if (!v.ConvertToInt(value))
	{
		if (value < 0 || value > 4)
			return false;

		unsigned char cValue = 0;
		if (value != 4)  // If it is 4, leave it 0 and let it be corrected later
			cValue = static_cast<unsigned char>(value);// Avoids conflict with Input Slot bits
		desc = desc + (cValue << 4);
		return true;
	}
	return false;
}

/*
* Method: ShaderParser - SetDataCount
* Purpose: Sets the number of a field that a vertex buffer expects
* Parameters: TString & v - number in string form
* Return: bool - success, whether the string was a number
*/
bool TShaderParser::SetDataCount(TString& v)
{
	int value = 0;
	if (!v.ConvertToInt(value))
	{
		if (value < 0 || value > 4)
			return false;

		unsigned short cValue = static_cast<unsigned short>(value);
		cValue = cValue << 12;
		desc = desc + cValue;
		return true;
	}
	return false;
}

/*
* Method: ShaderParser - SetTextureCount
* Purpose: Sets a texture count for the number of textures the shader expects
* Parameters: TString & v - number in string form
* Return: bool - success, whether the string was a number
*/
bool TShaderParser::SetTextureCount(TString& v)
{
	int value = 0;
	if (!v.ConvertToInt(value))
	{
		shaderHost->SetTextureCount(shaderIndex, value);
		return true;
	}
	return false;
}