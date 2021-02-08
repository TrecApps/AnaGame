#include "TObject.h"
#include "TString.h"
#include "TThread.h"

UCHAR TObjectType[] = { 1, 0b10000000 };

WCHAR str_true[] = L"true";
WCHAR str_false[] = L"false";


/*
* Method: TObject::TObject
* Purpose: Default constructor for the TObject
* Parameters: void
* Returns: void
*/
TObject::TObject()
{
	//sys_Type = new LPCTSTR((LPCTSTR)"SYS_TOBJECT");
	thread = 0;
}

/*
* Method: TObject::~TObject
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
TObject::~TObject()
{
	
	thread = 0;
}

/*
* Method: TObject::GetAnaGameType
* Purpose: Retrieves the AnaGame type
* Parameters: void
* Returns: UCHAR* - the AnaGame type represenation 
*
* Note: deprecated
*/
UCHAR * TObject::GetAnaGameType()
{
	return TObjectType;
}

/**
 * Method: TObject::getVariableValueStr
 * Purpose: Returns the String value of a TObjects given variable, meant for databinding
 * Parameters: TString& varName - the name of the variable to seek
 * Returns: TString - the variable value in String form, or an empty string if variable does not exist
 */
TString TObject::getVariableValueStr(const TString & varName)
{
	return TString();
}

/**
 * Method: TObject::toString
 * Purpose: Returns a string representation of this object
 * Parameters: void
 * Returns: TString - repreetnation of this object
 */
TString TObject::toString()
{
	return TString();
}

/**
 * Method: TObject::GetType
 * Purpose: Returns a String Representation of the object
 * Parameters: void
 * Returns: TString - representation of the object type
 *
 * Note: This method is provided to allow interpretors to allow scripts to access methods of Anagame Objects.
 *   subclasses should report their type first, then the parent clss type and seperate it with a SemiColon
 */
TString TObject::GetType()
{
	return TString(L"TObject");
}

TObject* TObject::ProcessPointer(TObject* obj)
{
	return obj;
}

TObject* TObject::ProcessPointer(void* obj)
{
	return nullptr;
}

TObject* TObject::ProcessPointer(int* obj)
{
	return nullptr;
}

TObject* TObject::ProcessPointer(float* obj)
{
	return nullptr;
}

/**
 * Method: TObject::ThreadLock
 * Purpose: Allows any Object that can claim TObject as a type to restrict access to the Current Thread being called
 * Parameters: void
 * Returns: bool - whether the locking mechanism worked
 *
 * Note: In order for this method and ThreadRelease to work properly, you must hold on to the boo that is returned and pass it into ThreadRelease.
 *		Since methods can call each other, Only the first method called should be the one that actually unlocks the object
 */
bool TObject::ThreadLock() const
{
	DWORD curThread = GetCurrentThreadId();
	if (thread)
	{
		// Object is already locked by a thread. Check to see if it is locked by this thread. if it is
		// Return false to let the caller know that the ReleaseMethod shoudl do nothing. Otherwise, sleep until unlocked
		if (thread == curThread)
			return false;
		// Go to sleep until this object unlocks it from the owner thread
		TThread::Suspend(this);
	}

	thread = curThread;

	for (UINT Rust = 0; Rust < 10; Rust++);

	if (thread != curThread)
		TThread::Suspend(this);
	thread = curThread;

	return true;
}

/**
 * Method: TObject::ThreadRelease
 * Purpose: Allows the TObject to release any thread that may have previously been restricted, assuming that true is passed
 * Parameters: bool key - whether the unlocking mechanism should actually proceed
 * Returns: void
 */
void TObject::ThreadRelease(bool key) const
{
	if (!key)
		return;
	thread = 0;
	TThread::Resume(this);
}

/*
* Function: boolToString 
* Purpose: returns string representations of the bool
* Parameters: bool val -  the bool to convert
* Returns: WCHAR* string representations
*/
WCHAR* boolToString(bool val)
{
	if (val)
		return str_true;
	else
		return str_false;
}