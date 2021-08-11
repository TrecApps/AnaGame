#include "T3DModelParser.h"

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

TrecPointer<T3DModelParser> Get3DParser(TrecPointer<TFileShell> filepath)
{
	if(!Is3DModelFormatSupported(filepath))
		return TrecPointer<T3DModelParser>();
	TString fullPath(filepath->GetPath());

	TString ext(fullPath.SubString(fullPath.FindLast(L'.') + 1));

	for (UINT Rust = 0; Rust < modelBuilders.Size(); Rust++)
	{
		if (modelBuilders[Rust]->FormatSupport(ext))
			return modelBuilders[Rust]->GetParser(filepath);
	}
	return TrecPointer<T3DModelParser>();
}

void Submit3DParserBuilder(TrecPointer<T3DModelParserBuilder> builder)
{
	if (builder.Get())
		modelBuilders.push_back(builder);
}
