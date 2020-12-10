#pragma once
#include "TJavaScriptInterpretor.h"
class TJavaScriptClassInterpretor :
    public TJavaScriptInterpretor
{public:
    /**
     * Method: TJavaScriptInterpretor::TInterpretor
     * Purpose: Constructor
     * Parameters: TrecPointer<TInterpretor> parentInterpretor - the Interpretor that created this interpretor (use null if this is a root)
     * Returns: New TInterpretor Object
     */
    TJavaScriptClassInterpretor(TrecSubPointer<TVariable, TInterpretor> parentInterpretor, TrecPointer<TEnvironment> env);
};

