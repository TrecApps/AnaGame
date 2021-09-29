#pragma once
#include <TVariable.h>


/**
 * Class: TDataDoc
 * Purpose: Represents an object that takes on specific functionality based off of the data it holds
 */
class TDataDoc :
    public TVObject
{
public:

    /**
     * Class: TDataDocBuilder
     * Purpose: Provides a means through which Specialized DataDoc Objects can be constructed
     */
    class TDataDocBuilder
    {
    public:
        /**
         * Method: TDataDocBuilder::GetDoc
         * Purpose: Retrieves the DataDoc Type based off of the provided Tstring Parameter
         * Parameters: const TString& type - the type of Data Doc to retrieve
         *              TrecPointer<TVariable> data - the variable to hold
         * Returns: TrecPointer<TDataDoc> - the Data Doc requested (or null if not supported)
         * 
         * Attributes: abstract
         */
        virtual TrecPointer<TDataDoc> GetDoc(const TString& type, TrecPointer<TVariable> data) = 0;
    };

    /**
     * Method: TDataDoc::SubmitBuilder
     * Purpose: Allows Builders to be added
     * Parameters: TrecPointer<TDataDocBuilder> builder - the builder to submit
     * Returns: UINT - the index of the Builder (to be used when unloading it)
     * 
     * Attributes: static
     */
    static UINT SubmitBuilder(TrecPointer<TDataDocBuilder> builder);


    /**
     * Method: TDataDoc::GetBuilder
     * Purpose: Retrieves the Builder for the Data Doc type
     * Parameters: const TString& type - type of data requested
     *             TrecPointer<TVariable> data - the data for the Doc Object to hold
     * Returns: TrecPointer<TDataDoc> - the Data Doc requested (or null if not supported)
     *
     * Attributes: static
     */
    static TrecPointer<TDataDoc> GetBuilder(const TString& type, TrecPointer<TVariable> data);


protected:

    /**
     * The variable being held
     */
    TrecPointer<TVariable> variable;
};

