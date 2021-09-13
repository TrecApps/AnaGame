#include "TObjModelParser.h"


bool TObjModelParserBuilder::FormatSupport(const TString& extension)
{
    return extension.CompareNoCase(L"obj") == 0 || extension.CompareNoCase(L".obj");
}

TrecPointer<T3DModelParser> TObjModelParserBuilder::GetParser(TrecPointer<TFileShell> filepath, TrecPointer<TArenaEngine> engine)
{
    if (!engine.Get() || !filepath.Get() || !filepath->GetPath().EndsWith(".obj", true))
        return TrecPointer<T3DModelParser>();
    TrecSubPointer<T3DModelParser, TObjModelParser> parser = TrecPointerKey::GetNewTrecSubPointer<T3DModelParser, TObjModelParser>();
    parser->filePointer = filepath;
    parser->engine = engine;
    return TrecPointerKey::GetTrecPointerFromSub<>(parser);
}


void TObjModelParserBuilder::GetSupportedFormats(TDataArray<TString>& subFormats)
{
    subFormats.push_back(L"obj");
}


bool ProcessFloats(float* fMem, TrecPointer<TDataArray<TString>> details)
{
    for (UINT Rust = 0; Rust < 4 && Rust < details->Size(); Rust++)
    {
        if (details->at(Rust).ConvertToFloat(fMem[Rust]))
            return false;
    }
    return true;
}

TrecPointer<ArenaModel> TObjModelParser::Generate(const ShaderProgram& program, TString& err)
{

    TrecPointer<ArenaModel> ret;
    ReadData(err);
    if (err.GetSize())
        return ret;

    return this->Compile(program,err);
}

void TObjModelParser::Generate(TrecPointer<ArenaModel>& model, TString& err)
{
    err.Empty();
    if (!model.Get())
    {
        err.Set(L"Null Model passed!");
        return;
    }

    ReadData(err);
    if (err.GetSize())
        return ;

    Compile(model, err);

    
}

TString TObjModelParser::Save(TrecPointer<ArenaModel> model)
{
    indices.RemoveAll();
    textures.RemoveAll();
    vertices.RemoveAll();
    vertices4.RemoveAll();
    normal.RemoveAll();

    iNormal = iVertex3 = iVertex4 = iTexture = 0;

    if (!model.Get())
        return L"Null Model Pointer Provided!";

    TFile writer(filePointer->GetPath(), TFile::t_file_open_always | TFile::t_file_write);
    if (!writer.IsOpen())
        return L"Failed to Open OBJ File for writing";

    ShaderKey shaderKey = model->GetShaderId();

    UINT inputCount = 0;
    D3D11_INPUT_ELEMENT_DESC* inputDesc = nullptr;
    if (shaderKey._default)
        inputDesc = engine->GetInputInfo(shaderKey.card.dID, inputCount);
    else inputDesc = engine->GetInputInfo(shaderKey.card.id, inputCount);

    if (!inputDesc || !inputCount)
        return L"Failed to Get Vertex Input Data!";
    
    TDataArray<float> vertices = model->GetVertices();

    for (UINT Rust = 0; Rust < vertices.Size();)
    {
        for (UINT C = 0; C < inputCount; C++)
        {
            D3D11_INPUT_ELEMENT_DESC* iDesc = &inputDesc[C];

            if (!strcmp(iDesc->SemanticName, "POSITION"))
            {
                bool is3 = iDesc->Format == DXGI_FORMAT_R32G32B32_FLOAT;

                UINT cap = is3 ? 2 : 3;

                if (Rust + cap >= vertices.Size())
                    return L"Not enough Vertices for the Vertex field";
                T4Vertex vert;
                vert.x = vertices[Rust];
                vert.y = vertices[Rust + 1];
                vert.z = vertices[Rust + 2];
                vert.w = is3 ? 1.0f : vertices[Rust + 3];

                this->vertices4.push_back(vert);
                Rust += (cap + 1);
            }
            else if (!strcmp(iDesc->SemanticName, "NORMAL"))
            {
                if (Rust + 2 >= vertices.Size())
                    return L"Not enough Vertices for the Normal field";
                TVertex nVert;
                nVert.x = vertices[Rust];
                nVert.y = vertices[Rust + 1];
                nVert.z = vertices[Rust + 2];
                this->normal.push_back(nVert);
                Rust += 3;
            }
            else if (!strcmp(iDesc->SemanticName, "TEXCOORD"))
            {
                if (Rust + 1 >= vertices.Size())
                    return L"Not enough Vertices for the Texture field";
                T2Float nVert;
                nVert.x = vertices[Rust];
                nVert.y = vertices[Rust + 1];
                this->textures.push_back(nVert);
                Rust += 2;
            }
        }
    }

    this->indices = model->GetIndices();

    writer.WriteString(L"# Vertices\n");
    TString line;
    for (UINT Rust = 0; Rust < this->vertices4.Size(); Rust++)
    {
        T4Vertex v = vertices4[Rust];

        line.Format(L"v %f %f %f %f\n", v.x, v.y, v.z, v.w);
        writer.WriteString(line);
    }

    // Normals
    writer.WriteString(L"\n# Normals\n");
    for (UINT Rust = 0; Rust < this->normal.Size(); Rust++)
    {
        TVertex v = normal[Rust];

        line.Format(L"vn %f %f %f\n", v.x, v.y, v.z);
        writer.WriteString(line);
    }

    // Textures
    writer.WriteString(L"\n# Textures\n");
    for (UINT Rust = 0; Rust < this->textures.Size(); Rust++)
    {
        T2Float v = textures[Rust];

        line.Format(L"vt %f %f\n", v.x, v.y);
        writer.WriteString(line);
    }

    // To-Do: figure out indices

    return TString();
}

void TObjModelParser::ReadData(TString& err)
{
    TFile file(filePointer->GetPath(), TFile::t_file_open_existing | TFile::t_file_read);
    if (!file.IsOpen())
    {
        err.Set(L"Failed to open OBJ file!");
        return;
    }

    TString line;
    while (file.ReadString(line))
    {
        line.Trim();
        if (!line.GetSize() || line.StartsWith(L'#'))
            continue;

        float value[4] = { 0.0f,0.0f,0.0f,0.0f };

        if (line.StartsWith(L"vn"))
        {
            line.Set(line.SubString(2).GetTrim());
            auto nums = line.split(L' ');
            if (nums->Size() < 3)
            {
                err.Format(L"Insufficient verticies supplied for 'vn' field: '%ws'", line.GetConstantBuffer().getBuffer());
                return;
            }
            if (!ProcessFloats(value, nums))
            {
                err.Format(L"Non-Floating-point token detected for 'vn' field: '%ws'", line.GetConstantBuffer().getBuffer());
                return;
            }

            TVertex vertex3;
            vertex3.x = value[0];
            vertex3.y = value[1];
            vertex3.z = value[2];

            this->normal.push_back(vertex3);
            continue;
        }
        if (line.StartsWith(L"vt"))
        {
            line.Set(line.SubString(2).GetTrim());
            auto nums = line.split(L' ');
            if (nums->Size() < 1)
            {
                err.Format(L"Insufficient verticies supplied for 'vt' field: '%ws'", line.GetConstantBuffer().getBuffer());
                return;
            }
            if (!ProcessFloats(value, nums))
            {
                err.Format(L"Non-Floating-point token detected for 'vt' field: '%ws'", line.GetConstantBuffer().getBuffer());
                return;
            }

            T2Float vertex3;
            vertex3.x = value[0];
            vertex3.y = value[1];

            this->textures.push_back(vertex3);
            continue;
        }
        if (line.StartsWith(L"v"))
        {
            line.Set(line.SubString(1).GetTrim());
            auto nums = line.split(L' ');
            if (nums->Size() < 3)
            {
                err.Format(L"Insufficient verticies supplied for 'v' field: '%ws'", line.GetConstantBuffer().getBuffer());
                return;
            }
            value[3] = 1.0f;// Set default
            if (!ProcessFloats(value, nums))
            {
                err.Format(L"Non-Floating-point token detected for 'v' field: '%ws'", line.GetConstantBuffer().getBuffer());
                return;
            }

            T4Vertex vertex4;
            vertex4.x = value[0];
            vertex4.y = value[1];
            vertex4.z = value[2];
            vertex4.w = value[3];
            this->vertices4.push_back(vertex4);
            continue;
        }
        if (line.StartsWith(L"f"))
        {
            line.Set(line.SubString(1).GetTrim());

            auto nums = line.split(L' ');
            for (UINT Rust = 0; Rust < nums->Size(); Rust++)
            {
                UINT ind = 0;
                if (nums->at(Rust).Find(L'/') != 1)
                {
                    err.Format(L"Unsupported feature x/y for 'f' field detected. Anagame does not currently support this feature! '%ws'", line.GetConstantBuffer().getBuffer());
                    return;
                }
                if (!TString::ConvertStringToUint(nums->at(Rust), ind))
                {
                    err.Format(L"Non-numeric token detected for 'f' field: '%ws'", line.GetConstantBuffer().getBuffer());
                    return;
                }
                indices.push_back(ind);
            }
        }


    }
}
