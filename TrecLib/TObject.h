#pragma once
#include <Windows.h>
#include <new>
#include <memory>
#include "TrecLib.h"
#include <cassert>

#include "AnagameDef.h"

#define AG_THREAD_LOCK ThreadLock();
#define RETURN_THREAD_UNLOCK ThreadRelease(); return 

extern UCHAR TArrayType[];
extern UCHAR TDataArrayType[];
extern UCHAR TMapType[];

class TString;

class _TREC_LIB_DLL TObjectLocker
{
public:
	TObjectLocker(CRITICAL_SECTION* section);
	~TObjectLocker();

private:
	CRITICAL_SECTION* section;
};

/*
* Class: TObject
* Purpose: Serves as the base object for the AnaGame platform
*/
class _TREC_LIB_DLL TObject 
{
public:
	/*
	* Method: TObject::TObject
	* Purpose: Default constructor for the TObject
	* Parameters: void
	* Returns: void
	*/
	TObject();
	/*
	* Method: TObject::~TObject
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	virtual ~TObject();

	/*
	* Method: TObject::GetAnaGameType
	* Purpose: Retrieves the AnaGame type
	* Parameters: void
	* Returns: UCHAR* - the AnaGame type represenation
	*
	* Note: deprecated
	*/
	virtual UCHAR* GetAnaGameType();
	/**
	 * Method: TObject::getVariableValueStr
	 * Purpose: Returns the String value of a TObjects given variable, meant for databinding
	 * Parameters: TString& varName - the name of the variable to seek
	 * Returns: TString - the variable value in String form, or an empty string if variable does not exist
	 * 
	 * Attributes: virtual
	 */
	virtual TString getVariableValueStr(const TString& varName);
	/**
	 * Method: TObject::toString
	 * Purpose: Returns a string representation of this object
	 * Parameters: void
	 * Returns: TString - repreetnation of this object
	 * 
	 * Attributes: virtual
	 */
	virtual TString toString();

	/**
	 * Method: TObject::GetType()
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 * 
	 * Note: This method is provided to allow interpretors to allow scripts to access methods of Anagame Objects.
	 *   subclasses should report their type first, then the parent clss type and seperate it with a SemiColon
	 * 
	 * Attributes: virtual
	 */
	virtual TString GetType();

	/**
	 * Method: ProcessPointer
	 * Purpose: Intended to be used to ensure that only TObjects are valid (this version takes in a TObject)
	 * Parameters: TObject* obj - the pointer to the Object
	 * Returns: TObject* - the parameter
	 *
	 * Attributes: static
	 */
	static TObject* ProcessPointer(TObject* obj);

	/**
	 * Method: ProcessPointer
	 * Purpose: Intended to be used to ensure that only TObjects are valid (this version takes in a void)
	 * Parameters: void* obj - the pointer to memory
	 * Returns: TObject* - null since void cannot be converted to TObject
	 *
	 * Attributes: static
	 */
	static TObject* ProcessPointer(void* obj);

	/**
	 * Method: ProcessPointer
	 * Purpose: Intended to be used to ensure that only TObjects are valid (this version takes in an int)
	 * Parameters: int* obj - the pointer to memory
	 * Returns: TObject* - null since void cannot be converted to TObject
	 *
	 * Attributes: static
	 */
	static TObject* ProcessPointer(int* obj);

	/**
	 * Method: ProcessPointer
	 * Purpose: Intended to be used to ensure that only TObjects are valid (this version takes in a float)
	 * Parameters: float* obj - the pointer to memory
	 * Returns: TObject* - null since float cannot be converted to TObject
	 *
	 * Attributes: static
	 */
	static TObject* ProcessPointer(float* obj);

protected:
	
	/**
	 * Method: TObject::ThreadLock
	 * Purpose: Allows any Object that can claim TObject as a type to restrict access to the Current Thread being called
	 * Parameters: void
	 * Returns: bool - whether the locking mechanism worked
	 * 
	 * Note: In order for this method and ThreadRelease to work properly, you must hold on to the boo that is returned and pass it into ThreadRelease.
	 *		Since methods can call each other, Only the first method called should be the one that actually unlocks the object
	 */
	void ThreadLock() const;

	/**
	 * Method: TObject::ThreadRelease
	 * Purpose: Allows the TObject to release any thread that may have previously been restricted, assuming that true is passed
	 * Parameters: bool key - whether the unlocking mechanism should actually proceed
	 * Returns: void
	 */
	void ThreadRelease()const;

	/**
	 * Critical Section
	 */
	mutable CRITICAL_SECTION thread;

	mutable UCHAR threadCounter;

	/**
	 * Whether we are currently in a critical section
	 */
	mutable bool isInSection;
};






/*
* Function: boolToString
* Purpose: returns string representations of the bool
* Parameters: bool val -  the bool to convert
* Returns: WCHAR* string representations
*/
 _TREC_LIB_DLL WCHAR* boolToString(bool);
