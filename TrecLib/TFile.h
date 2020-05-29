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

	static const UINT t_file_read = GENERIC_READ;
	static const UINT t_file_write = GENERIC_WRITE;
	static const UINT t_file_share_delete = 0x00000400;
	static const UINT t_file_share_read = 0x00000100;
	static const UINT t_file_share_write = 0x00000200;
	static const UINT t_file_create_always = (CREATE_ALWAYS << 16);
	static const UINT t_file_create_new = (CREATE_NEW << 16);
	static const UINT t_file_open_always = (OPEN_ALWAYS << 16);
	static const UINT t_file_open_existing = (OPEN_EXISTING << 16);
	static const UINT t_file_truncate_existing = TRUNCATE_EXISTING << 16;

	static const UCHAR t_file_include_terminator = 0b00000001;

	TFile();
	TFile(const TString& lpszFileName,
		UINT nOpenFlags);
	~TFile();

	// Methods that override CFile's Methods
	bool Open(const TString& lpszFileName,
		UINT nOpenFlags);


	// Methods Imported from CArchive
	
	BOOL ReadString(TString& rString);
	UINT ReadString(TString& rString, UINT nMax);

	UINT ReadString(TString& rString, WCHAR chara);

	UINT ReadString(TString& rString, const TString& chars, UCHAR flags);
	
	void WriteString(const TString& lpsz);
	bool IsOpen()const;
	bool SetEncoding(FileEncodingType fet);
	TString GetFileDirectory();
	virtual UCHAR* GetAnaGameType() override;

	TString GetFileExtension();

	void Close();
	void Flush();
	TString GetFileName() const;
	TString GetFilePath() const;
	TString GetFileTitle()const;
	ULONGLONG GetLength()const;
	ULONGLONG GetPosition()const;

	UINT Read(void* buffer, UINT count);
	ULONGLONG Seek(LONGLONG offset, UINT from);
	void SeekToBegin();
	ULONGLONG SeekToEnd();
	void Write(const void* buffer, UINT count);

	FileEncodingType GetEncodingType();

protected:
	FileEncodingType DeduceEncodingType();

	void ConvertFlags(UINT& input, UINT& open, UINT& security, UINT& creation);

private:
	TString filePath;
	FileEncodingType fileEncode;
	HANDLE fileHandle;
	ULONGLONG position;
};

