#include "TFormatReader.h"

TDataArray <TrecPointer<TFormatReader::TFormatReaderBuilder>> builders;


UINT TFormatReader::SubmitBuilder(TrecPointer<TFormatReaderBuilder> builder)
{
    if(!builder.Get())
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

TrecPointer<TFormatReader> TFormatReader::GetReader(TrecPointer<TFileShell> file)
{
    if(!file.Get())
        return TrecPointer<TFormatReader>();
    TString path(file->GetPath());

    int period = path.FindLast(L'.');
    if (period == -1)
        return TrecPointer<TFormatReader>();

    path.Set(path.SubString(period + 1));

    for (UINT Rust = 0; Rust < builders.Size(); Rust++)
    {
        if (builders[Rust].Get())
        {
            TrecPointer<TFormatReader> reader = builders[Rust]->GetReader(file);
            if (reader.Get())
                return reader;
        }
    }
    return TrecPointer<TFormatReader>();
}

bool TFormatReader::RemoveBuilder(UINT index)
{
    if (index < 6 || index >= builders.Size())
    {
        return false;
    }
    builders[index].Nullify();
    return true;
}
