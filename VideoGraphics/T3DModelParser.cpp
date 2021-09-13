#include "T3DModelParser.h"

const CHAR* _3_sem_V = "POSITION";
const CHAR* _3_sem_BN = "BINORMAL";
const CHAR* _3_sem_BW = "BLENDWEIGHT";
const CHAR* _3_sem_BI = "BLENDINDICES";
const CHAR* _3_sem_T = "TEXCOORD";
const CHAR* _3_sem_N = "NORMAL";
const CHAR* _3_sem_C = "COLOR";
const CHAR* _3_sem_PT = "POSITIONT";
const CHAR* _3_sem_TA = "TANGENT";
const CHAR* _3_sem_F = "FOG";
const CHAR* _3_sem_TS = "TESSFACTOR";

TDataArray<TrecPointer<T3DModelParserBuilder>> modelBuilders;

TVertex::TVertex()
{
	x = y = z = 0.0f;
}

TVertex::TVertex(const TVertex& orig)
{
	x = orig.x;
	y = orig.y;
	z = orig.z;
}

T4Vertex::T4Vertex()
{
	w = 0.0f;
}

T4Vertex::T4Vertex(const T4Vertex& orig) : TVertex(orig)
{
	w = orig.w;
}

T2Float::T2Float()
{
	x = y = 0.0f;
}

T2Float::T2Float(const T2Float& orig)
{
	x = orig.x;
	y = orig.y;
}

bool Is3DModelFormatSupported(TrecPointer<TFileShell> filePath)
{
	if(!filePath.Get() || filePath->IsDirectory())
		return false;

	int finalDot = filePath->GetPath().FindLast(L'.');
	if (finalDot == -1)
		return false;

	TString ext(filePath->GetPath().SubString(finalDot + 1));

	for (UINT Rust = 0; Rust < modelBuilders.Size(); Rust++)
	{
		if (modelBuilders[Rust]->FormatSupport(ext))
			return true;
	}
	return false;
}

TrecPointer<T3DModelParser> Get3DParser(TrecPointer<TFileShell> filepath, TrecPointer<TArenaEngine> engine)
{
	if(!Is3DModelFormatSupported(filepath) || !engine.Get())
		return TrecPointer<T3DModelParser>();
	TString fullPath(filepath->GetPath());

	TString ext(fullPath.SubString(fullPath.FindLast(L'.') + 1));

	for (UINT Rust = 0; Rust < modelBuilders.Size(); Rust++)
	{
		if (modelBuilders[Rust]->FormatSupport(ext))
			return modelBuilders[Rust]->GetParser(filepath, engine);
	}
	return TrecPointer<T3DModelParser>();
}

void Submit3DParserBuilder(TrecPointer<T3DModelParserBuilder> builder)
{
	if (builder.Get())
		modelBuilders.push_back(builder);
}

typedef enum class model_compiler_target{
	mct_vertex_3,
	mct_vertex_4,
	mct_normal_3,
	mct_texture_2
}model_compiler_target;

class ModelCompiler {
	friend class TDataArray<ModelCompiler>;
private:
	model_compiler_target mct;
	ModelCompiler() {
		this->mct = model_compiler_target::mct_normal_3;
	}
public:
	ModelCompiler(model_compiler_target mct) {
		this->mct = mct;
	}

	ModelCompiler(const ModelCompiler& copy) {
		this->mct = copy.mct;
	}

	bool hasReachedEnd(T3DModelParser* parser) {
		switch (mct) {
		case model_compiler_target::mct_normal_3:
			return parser->iNormal >= parser->normal.Size();
		case model_compiler_target::mct_texture_2:
			return parser->iTexture >= parser->textures.Size();
		case model_compiler_target::mct_vertex_3:
		case model_compiler_target::mct_vertex_4:
			return parser->iVertex4 >= parser->vertices4.Size();
		}
	}

	bool addNextVertex(T3DModelParser* parser, TDataArray<float>& vertexData) {

		if (hasReachedEnd(parser))
			return false;

		switch (mct) {
		case model_compiler_target::mct_normal_3:
			vertexData.push_back(parser->normal[parser->iNormal].x);
			vertexData.push_back(parser->normal[parser->iNormal].y);
			vertexData.push_back(parser->normal[parser->iNormal].z);
			parser->iNormal++;
			break;
		case model_compiler_target::mct_texture_2:
			vertexData.push_back(parser->textures[parser->iTexture].x);
			vertexData.push_back(parser->textures[parser->iTexture].y);
			parser->iTexture++;
			break;
		case model_compiler_target::mct_vertex_3:
			vertexData.push_back(parser->vertices4[parser->iVertex4].x);
			vertexData.push_back(parser->vertices4[parser->iVertex4].y);
			vertexData.push_back(parser->vertices4[parser->iVertex4].z);
			parser->iVertex4++;
			break;
		case model_compiler_target::mct_vertex_4:
			vertexData.push_back(parser->vertices4[parser->iVertex4].x);
			vertexData.push_back(parser->vertices4[parser->iVertex4].y);
			vertexData.push_back(parser->vertices4[parser->iVertex4].z);
			vertexData.push_back(parser->vertices4[parser->iVertex4].w);
			parser->iVertex4++;
		}

		return true;
	}
};

void T3DModelParser::Compile(TrecPointer<ArenaModel>& model, TString& err)
{
	ShaderKey shaderKey = model->GetShaderId();
	if (!shaderKey._default && shaderKey.card.id < 0)
	{
		err.Set(L"Model Provided does not have a shader set");
		return;
	}

	UINT inputCount = 0;
	D3D11_INPUT_ELEMENT_DESC* inputDesc = nullptr;
	if (shaderKey._default)
		inputDesc = engine->GetInputInfo(shaderKey.card.dID, inputCount);
	else inputDesc = engine->GetInputInfo(shaderKey.card.id, inputCount);

	if (!inputDesc || !inputCount)
	{
		err.Set(L"Failed to Get Vertex Input Data!");
		return;
	}

	TDataArray<float> vertices;
	Compile(inputDesc, inputCount, err, vertices);

	if (err.GetSize())
		return;


	model->SetVertexData(vertices);
	model->SetIndices(this->indices);
}


T3DModelParser::T3DModelParser()
{
	iVertex3 = iVertex4= iNormal= iTexture =0;
}

bool OutSync(T3DModelParser* modelParser, TDataArray<ModelCompiler>& modelComps) {
	bool hasFalse = false, hasTrue = false;

	for (UINT Rust = 0; Rust < modelComps.Size(); Rust++) {
		if (modelComps[Rust].hasReachedEnd(modelParser))
			hasTrue = true;
		else
			hasFalse = true;
	}

	return hasTrue && hasFalse;
}

void T3DModelParser::Compile(D3D11_INPUT_ELEMENT_DESC* desc, UINT count, TString& err, TDataArray<float>& vertices)
{
	iVertex3 = iVertex4 = iNormal = iTexture = 0;

	if (!desc)
	{
		err.Set(L"Input Descriptor param was null!");
		return;
	}

	TDataArray<ModelCompiler> modelComps;

	for (UINT Rust = 0; Rust < count; Rust++)
	{
		D3D11_INPUT_ELEMENT_DESC* curDesc = &desc[Rust];

		if (!strcmp(curDesc->SemanticName, _3_sem_V))
		{
			modelComps.push_back(ModelCompiler(curDesc->Format == DXGI_FORMAT_R32G32B32_FLOAT ? model_compiler_target::mct_vertex_3 : model_compiler_target::mct_vertex_4));
		}
		else if (!strcmp(curDesc->SemanticName, _3_sem_T))
		{
			modelComps.push_back(ModelCompiler(model_compiler_target::mct_texture_2));
		}
		else if (!strcmp(curDesc->SemanticName, _3_sem_N))
		{
			modelComps.push_back(ModelCompiler(model_compiler_target::mct_normal_3));
		}
	}

	bool complete = false;


	while (!complete && modelComps.Size())
	{
		for (UINT Rust = 0; Rust < modelComps.Size(); Rust++)
		{
			if (!modelComps[Rust].addNextVertex(this, vertices))
			{
				err.Set(L"Failed to Add Verticies!");
				return ;
			}
		}

		if (OutSync(this, modelComps))
		{
			err.Set(L"Data Quantity Out of Sync!");
			return;
		}
		complete = modelComps[0].hasReachedEnd(this);
	}
}

TrecPointer<ArenaModel> T3DModelParser::Compile(const ShaderProgram& program, TString& err)
{
	TDataArray<float> vertices;

	Compile(program.elements, program.elementCount, err, vertices);


	TrecPointer<ArenaModel> ret = TrecPointerKey::GetNewTrecPointer<ArenaModel>(engine);

	ret->SetVertexData(vertices);
	ret->SetIndices(this->indices);

	return ret;
}

