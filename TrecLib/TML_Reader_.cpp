
#include "TML_Reader_.h"



/**
 * Method: TML_Reader_::TML_Reader_
 * Purpose: Constructor
 * Parameters: TrecPointer<TFile> tf - the TFile to use
 *			TrecPointer<Parser_> par - the parser to call upon
 * Returns: new TML Reader Object
 */
TML_Reader_::TML_Reader_(TrecPointer<TFile> ta, TrecPointer<Parser_> p) : ParseReader_(ta,p)
{

}

/*
* Method: TML_Reader_::~TML_Reader
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
TML_Reader_::~TML_Reader_()
{
}

/**
 * Method: TML_Reader_::read
 * Purpose: Reads the file anticipating a TML format
 * Parameters: int* - the line number an error occurs
 * Returns: bool - success
 */
bool TML_Reader_::read(int *l)
{
	// don't bother reading a file format 
	if (!materials)
		return false;
	TString line;

	//CFileException fileExp;

	//fileActualReader->Open((LPCTSTR)reader->GetFile()->GetFileName(), CFile::modeRead, &fileExp);
	unsigned char level = 1;
	int testLevel;
	int strLoc, lineNumber = 0;
	bool noError = true;


	while (ReadString(line))
	{
		lineNumber++;
		line.Trim();

		if (lineNumber == 1 && line.Find(L"TML") == -1)
			goto skipper;

		if (line.Find(L"-/") != -1)
			break;
		if (line.Find(L"-|Type:") != -1)
		{

		}
		if (line.Find(L"-|Version:"))
		{

		}
	}



	while (ReadString(line) && line.Find(L"->End") == -1)
	{
		lineNumber++;
		line.Trim();
		// First check to see if we need to go up or down
	skipper:
		testLevel = line.Find(L">");
		if (testLevel > level)
		{
			level = testLevel;
			noError = respond->goChild();
		}
		else if (testLevel < level && testLevel != -1)
		{
			UINT levelReduct = level - testLevel;
			level = testLevel;
			for(UINT c = 0; c < levelReduct;c++)
				respond->goParent();
		}

		if (!noError)
		{
			// respond somehow

			*l = lineNumber;
			return false;
		}

		// now parse appropriately
		if (line.Find(L"->") != -1) // it's there
		{
			TString enterable(line.SubString(line.Find(L"->") + 2));
			if (enterable.Find(L"#") != -1)
			{
				enterable.Set(enterable.SubString(0, enterable.Find(L"#")));
			}
			TString* tEnterable = new TString(&enterable);
			//TrecPointer<TString> holdString = tEnterable;
			noError = respond->Obj(tEnterable);
			if (noError)
				continue;
			else
			{
				*l = lineNumber;
				return false;
			}
		}
		strLoc = line.Find(L"-|");
		if (strLoc != -1 && line.Find(L":") > strLoc)
		{
			TString mainEnterable = line.SubString(line.Find(L"-|") + 1); // Should be +2 but TControls expect the '|'
			TString enterable = mainEnterable.SubString(0, mainEnterable.Find(L":"));

			mainEnterable.Set(mainEnterable.SubString(mainEnterable.Find(L":") + 1, mainEnterable.Find(L"#"))); // 1 added to avoid the ':' in it



			mainEnterable.Trim();
			//WCHAR* mainEntBuff = enterable.GetBuffer();
			//WCHAR* buffEnt = (WCHAR*)malloc((enterable.GetLength() + 1) * sizeof(enterable[0]));
			
			//enterable.ReleaseBuffer();

			TrecPointer<TString> tEnterable;
			tEnterable = TrecPointerKey::GetNewTrecPointer<TString>(&mainEnterable);
			//tEnterable.hold();
			noError = respond->Attribute(tEnterable, enterable);
			
			//tEnterable.clean();
			if (!noError)
			{
				*l = lineNumber;
				
				return false;
			}

			//noError = respond->AttValue(mainEnterable);

			if (!noError)
			{
				*l = lineNumber;
				//tEnterable.clean();
				return false;
			}
		}
	}
	*l = -1;
	return true;
}

/**
 * Method: TML_Reader_::ReadString
 * Purpose: Reads a single line in the file
 * Parameters: TString & line - the line to read
 * Returns: bool - whether materials are set
 */
bool TML_Reader_::ReadString(TString & line)
{
	if (!materials)
		return false;
	return tReader->ReadString(line);

}
