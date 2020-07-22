#pragma once

#include <TObject.h>
#include <TFile.h>
#include <map>
#include <TMap.h>
#include "TrecCode.h"

typedef enum class LanguageDefaultStringEncoding
{
	ldse_acsii,
	ldse_unicode
}LanguageDefaultStringEncoding;

typedef enum class LanguageBlockType
{
	lbt_curly,
	lbt_indent,
	lbt_tokens
}LanguageBlockType;

typedef enum class RequireModuleHeader
{
	rmh_yes,
	rmh_no,
	rmh_no_in_default
} RequireModuleHeader;

typedef enum class LanguageInheritence
{
	li_mulitple,
	li_multiple_interface,
	li_single,
	li_not_supported
};

typedef enum class EnumImplementation
{
	ei_primitive,
	ei_class,
	ei_labelledUnion,
	ei_not_supported
};

// Message that running code can send back to the interpretor to know when it is time to stop, restart from the beginning, or some other action
typedef enum class InterpretorMessage
{
	im_run,
	im_break,		// Stops the interpretor without returning a value to the parent interpretor
	im_continue,	// Causes the interpretor to restart
	im_return		// Stops the interpretor and returns a value to the parant interpretor
}InterpretorMessage;

// 
typedef enum class InterpretorResource
{
	ir_none,
	ir_file,
	ir_string
}InterpretorResource;

// Used when filtering-out comments for simpler code parsing
typedef enum class CodeMode
{
	cm_reg,		// currently in regular code
	cm_sinStr,	// currently in a single-line String
	cm_mulStr,	// currently in a multi-line String
	cm_sinCom,	// currently in a single-line comment
	cm_mulCom	// currently in a multi-line comment
}CodeMode;

// Used when filtering out comments to know when a given string is being generated and where in the file that string begins
typedef struct DoubIndex
{
	int strInd;
	TString stringQ;
} DoubIndex;


class TLanguage :
	public TObject
{
	friend class TrecPointerKey;
public:	
	~TLanguage();
	static TrecPointer<TLanguage> getLanguage(TString& langName);
	static TrecPointer<TLanguage> getLanguage(TString& langName, TString& langVersion);

	UINT PreProcessFile(TrecPointer<TFile>& sourceFile);

	const TMap<TString>& GetPrimitiveTypes();


protected:
	static TrecPointer<TLanguage> getLanguage(TMap<TString>& langProps, TrecPointer<TLanguage> lang);


	TLanguage(TString& name, TString& version);


	// Preprocess Pipeline
	bool RunCommentFilter(TrecPointer<TFile> file, TString& newFileName);
	bool RunBlockFilter(TrecPointer<TFile> file, TString & newFileName);
	void UpdateDoubIndex(TString& buff, UINT index, DoubIndex& sinCom, DoubIndex& newLine, DoubIndex& mulCom,
		DoubIndex& mulComE, DoubIndex& sinStr, DoubIndex& mulStr);

	DoubIndex getNextSingleComment(TString& buff, UINT index);
	DoubIndex getNextNewline(TString& buff, UINT index);
	DoubIndex getNextMultiComment(TString& buff, UINT index);
	DoubIndex getNextMultiCommentEnd(TString& buff, UINT index);
	DoubIndex getNextSingleString(TString& buff, UINT index);
	DoubIndex getNextMultiString(TString& buff, UINT index);

	// Very Basic information about this particular language
	TString language;					// Name of the Language
	TString version;					// The version of the langauge
	TString statementEnd;				// Characters that signify the end of a statement in the language

	// Information used to help filter out comments during the comment filter stage. Also helps String Tags recognize strings
	TDataArray<TString> singleComment;				// Comments that end when the next line begins
	TDataArray<TString> startComment, endComment;   // Multi-line comments;
	TDataArray<TString> string, multiLineString;

	LanguageBlockType blockType;					// How TrecCode know how a block is defined
	TDataArray<TString> blockStarters;
	bool blockStartersAtBeginning;
	TDataArray<TString> blockEnders;
	
	// Information on how this language handles strings
	LanguageDefaultStringEncoding defaultencoding;	// What String encoding to assume if the programmer doesn't mark it
	TString switchMarker;							// Marker to mark the string as ti switch from the default encoding

	// External FIle resources, code defined outside the "current" code file
	bool externalFileName;							// Whether to use the value as a file name
	RequireModuleHeader requireModHeader;			// Whether the module requires module header and where
	bool moduleHeaderFirst;							// Does the language require the module header to be the first statement
	bool supportModuleBlock;						// Specifies that the language supports module blocks
	bool useForPublic;								// Specifies that resources in file are restricted to code outside the module unless explicitly permitted

	// Object Oriented configuration for the language
	LanguageInheritence inheritenceModel;
	EnumImplementation enumModel;
	UCHAR methodImplementation;

	TMap<TString> primitiveTypeMap;
};

