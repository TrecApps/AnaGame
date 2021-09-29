#include "TDataDoc.h"

TDataArray <TrecPointer<TDataDoc::TDataDocBuilder>> builders;

UINT TDataDoc::SubmitBuilder(TrecPointer<TDataDocBuilder> builder)
{
    if (!builder.Get())
        return 0;

    for (UINT Rust = 0; Rust < builders.Size(); Rust++)
    {
        if (!builders[Rust].Get())
        {
            builders[Rust] = builder;
            return Rust;
        }
    }

    return builders.push_back(builder) - 1;
}

TrecPointer<TDataDoc> TDataDoc::GetBuilder(const TString& type, TrecPointer<TVariable> data)
{

    for (UINT Rust = 0; Rust < builders.Size(); Rust++)
    {
        if (builders[Rust].Get())
        {
            TrecPointer<TDataDoc> reader = builders[Rust]->GetDoc(type, data);
            if (reader.Get())
                return reader;
        }
    }
    return TrecPointer<TDataDoc>();
}
