#include "TEnvironmentBuilder.h"

TMap<TEnvironmentBuilder> builders;

TEnvironmentBuilder::TEnvironmentBuilder()
{
}

TEnvironmentBuilder::~TEnvironmentBuilder()
{
}

bool TEnvironmentBuilder::SubmitBuilder(const TString& builderKey, TrecPointer<TEnvironmentBuilder> builder)
{
    if(!builderKey.GetSize() || !builder.Get())
        return false;

    builders.addEntry(builderKey, builder);
}
