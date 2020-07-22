#pragma once
#include "TrecLib.h"
#include "TString.h"

/*
* enum FileEncodingType
* used to track what type of encoding the file is using
*/
typedef enum class FileEncodingType
{
	fet_unknown,          // We do not know the encoding of this file
	fet_acsii,            // The encoding is acsii
	//fet_acsii16,          // The encoding is acsii-16
	fet_unicode,          // The encoding is Unicode
	fet_unicode_little,   // The encoding is Unicode - little endian
	fet_unicode8,         // The encoding is Unicode-8
	fet_unicode32,        // The encoding is Unicode-32
	fet_unicode32_little, // The encoding is Unicode-32 - little endian
	fet_unicode7          // The Encoding is UTF-7
}FileEncodingType;

/*
 * Class: TFile
 * Provides AnaGame's implementation of the File Class
*/
class _TREC_LIB_DLL TFile : public TObject
{
public:

	/**
	 * Flag for basic reading
	 */
	static const UINT t_file_read = GENERIC_READ;
	/**
	 * Flag for basic writing
	 */
	static const UINT t_file_write = GENERIC_WRITE;
	/**
	 * Allow other programs to delete the file
	 */
	static const UINT t_file_share_delete = 0x00000400;
	/**
	 * Allow other programs to read the file
	 */
	static const UINT t_file_share_read = 0x00000100;
	/**
	 * Allow other programs to write to the file
	 */
	static const UINT t_file_share_write = 0x00000200;
	/**
	 * Always create the file, even it it already exists
	 */
	static const UINT t_file_create_always = (CREATE_ALWAYS << 16);
	/**
	 * Creates a file or fails if file currently exists
	 */
	static const UINT t_file_create_new = (CREATE_NEW << 16);
	/**
	 * Opens a file, creating if it does not currently exist
	 */
	static const UINT t_file_open_always = (OPEN_ALWAYS << 16);
	/**
	 * Opens a file, or fails if file does not currently exist
	 */
	static const UINT t_file_open_existing = (OPEN_EXISTING << 16);
	/**
	 * Opens an existing file and truncates it, failing if file does not exist (needs "t_file_write")
	 */
	static const UINT t_file_truncate_existing = TRUNCATE_EXISTING << 16;

	/*
	* Method: TFile::TFile
	* Purpose: Default Constrictor
	* Parameters: void
	* Returns: void
	*/
	TFile();
	/*
	* Method: TFile::TFile
	* Purpose: Constructor
	* Parameters: LPCTSTR lpszFileName - the File name to open
	*			UINT nOpenFlags - flags that specify the open status of the file
	* Returns: void
	*/
	TFile(const TString& lpszFileName,
		UINT nOpenFlags);
	/*
	* Method: TFile::~TFile
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	~TFile();


	/*
	* Method: TFile::Open
	* Purpose: Opens the File
	* Parameters: LPCTSTR lpszFileName - The File to open
	*			UINT nOpenFlags - Flags to open the file
	* Returns: bool - success or failure to open file
	*/
	bool Open(const TString& lpszFileName,
		UINT nOpenFlags);


	// Methods Imported from CArchive

	/**
	 * Method: TFile::ReadString
	 * Purpose: Reads a line in a file into a String, taking into account the file encoding, stopping at the next line
	 * Parameters: TString& rString - the String to read into
	 * Returns: bool - success of reading
	 */
	BOOL ReadString(TString& rString);
	/**
	 * Method: TFile::ReadString
	 * Purpose: Reads a line in a file into a String, taking into account the file encoding, stopping at the next line
	 *			Or when the specificed number of characters are read
	 * Parameters: TString& rString - the String to read into
	 *				UINT nMax - max number of characters to read
	 * Returns: bool - success of reading
	 */
	UINT ReadString(TString& rString, UINT nMax);

	/**
	 * Method: TFile::ReadString
	 * Purpose: Reads a line in a file into a String, taking into account the file encoding, stopping at the specified chara
	 * Parameters: TString& rString - the String to read into
	 *				WCHAR chara - the character to stop at
	 * Returns: bool - success of reading
	 */
	UINT ReadString(TString& rString, WCHAR chara);

	/*
	* Method: TFile::WriteString
	* Purpose: Wrties a string to the file
	* Parameters: LPCTSTR lpsz - the Stringt to write
	* Returns: void
	*/
	void WriteString(const TString& lpsz);
	/*
	* Method: TFile::IsOpen
	* Purpose: Reports whether the File is open or not
	* Parameters: void
	* Returns: bool - is the file open
	*/
	bool IsOpen();
	/*
	* Method: TFile::SetEncoding
	* Purpose: Sets the encoding of the File (if not already set)
	* Parameters: FileEncodingType fet - the Encoding type to do
	* Returns: bool - whether the file encoding was set
	*/
	bool SetEncoding(FileEncodingType fet);
	/*
	* Method: TFile::GetFileDirectory
	* Purpose: Retrieves the directory of the file
	* Parameters: void
	* Returns: TString - the directory path of the file
	*/
	TString GetFileDirectory();

	/*
	* Method: TFile::GetAnaGameType
	* Purpose: Retrieves the AnaGame type
	* Parameters: void
	* Returns: UCHAR* - the AnaGame type
	*
	* Note: DEPRICATED
	*/
	virtual UCHAR* GetAnaGameType() override;

	/**
	 * Method: TFile::GetFileExtension
	 * Purpose: Retrievs the extension of the file
	 * Parameters: voif
	 * Returns: TString - the file extension detected
	 */
	TString GetFileExtension();

	/**
	 * Method: TFile::Close
	 * Purpose: Closes the file
	 * Parameters: void
	 * Returns: void
	 */
	void Close();
	/**
	 * Method: TFile::Flush
	 * Purpose: Flushes the file's internal buffer
	 * Parameters: void
	 * Returns: void
	 */
	void Flush();
	/**
	 * Method: TFile::GetFileName
	 * Purpose: Gets the name of the file
	 * Parameters: void
	 * Returns: TString - the name of the file (minus the path)
	 */
	TString GetFileName();
	/**
	 * Method: TFile::GetFilePath
	 * Purpose: Retrievs the fill path of the file
	 * Parameters: void
	 * Returns: TString - the path of the file
	 */
	TString GetFilePath();
	/**
	 * Method: TFile::GetFileTitle
	 * Purpose: Retirevs the "title" of the file
	 * Parameters: void
	 * Returns: TString - the title of the file
	 */
	TString GetFileTitle();
	/**
	 * Method: TFile::GetLength
	 * Purpose: Retirevs the current size of the file
	 * Parameters: void
	 * Returns: ULONGLONG - length of the file
	 */
	ULONGLONG GetLength();
	/**
	 * Method: TFile::GetPosition
	 * Purpose: Retrievs the current position of the file pointer
	 * Parameters: void
	 * Returns: ULONGLONG -  the current position of the file pointer
	 */
	ULONGLONG GetPosition();

	/**
	 * Method: TFile::Read
	 * Purpose: Reads a given number of bytes
	 * Parameters: void* buffer - pointer to the buffer
	 *				UINT count - number of bytes to read
	 * Returns: UINT - number of bytes read
	 */
	UINT Read(void* buffer, UINT count);
	/**
	 * Method: TFile::Seek
	 * Purpose: Sets the file's seek point according to the users specification
	 * Parameters: LONGLONG offset - the offset for the seek point
	 *				 UINT from - ofset from where? (0 for beginning, 1 for current point, 2 for end)
	 * Returns: ULONGLONG -  the position of the file's seek point
	 */
	ULONGLONG Seek(LONGLONG offset, UINT from);
	/**
	 * Method: TFile::SeekToBegin
	 * Purpose: Sets the files seek point to the beginning
	 * Parameters: void
	 * Returns: void
	 */
	void SeekToBegin();
	/**
	 * Method: TFile::SeekToEnd
	 * Purpose: Sets the seek to the end of the file
	 * Parameters: void
	 * Returns: ULONGLONG - the current seek of the file
	 */
	ULONGLONG SeekToEnd();
	/**
	 * Method: TFile::Write
	 * Purpose: Writes a set of bytes to the file
	 * Parameters: const void* buffer - pointer to bytes
	 *				UINT count - the number of bytes to write
	 * Returns: void
	 */
	void Write(const void* buffer, UINT count);

	/**
	 * Method: TFile::GetEncodingType
	 * Purpose: Retriev the file encoding mode
	 * Parameters: void
	 * Returns: FileEncodingType - the type of encoding the file is set to use
	 */
	FileEncodingType GetEncodingType();

protected:
	/*
	* Method: TFile::DeduceEncodingType
	* Purpose: Attempts to determine the encoding type of the file if possible
	* Parameters: void
	* Returns: FileEncodingType - the File Encoding type deduced
	*/
	FileEncodingType DeduceEncodingType();

	/**
	 * Method: TFile::ConvertFlags
	 * Purpose: Used internally by the class to convert TFile:: flags into Windows Crreate flags
	 * Parameters: UINT& input - the flags to convert
	 *				 UINT& open - basic open flags to send to Windows
	 *				 UINT& security - share attributes to send to windows
	 *				 UINT& creation - create instruction flags for Windows
	 * Returns: void
	 */
	void ConvertFlags(UINT& input, UINT& open, UINT& security, UINT& creation);

private:
	TString filePath;
	FileEncodingType fileEncode;
	HANDLE fileHandle;
	ULONGLONG position;
};

