#include "TFileTester.h"
#include <TFile.h>
#include <fstream>			// Use the C++ standard file header to test TFile against


const char* data = "Some Regular Data to add\n";
const char* data_win = "Some Regular Data to add\r";
const WCHAR* wData = L"Some Wide Data to add\n";
const WCHAR* wData_win = L"Some Wide Data to add";


TFileTester::TFileTester(const std::string& f1, const std::string& f2)
{
	file1 = f1;
	file2 = f2;
}

void TFileTester::RunTests(std::string& output)
{
	Test1(output);
	Test2(output);
}

std::string TFileTester::GetTestName()
{
	return std::string("TFile");
}

void TFileTester::Test1(std::string& output)
{
	TString fileName(file1.c_str());
	TFile file(fileName, TFile::t_file_open_existing | TFile::t_file_read);

	std::string encode = "File Encoding", seek = "File Seek", read = "Read";

	ExpectEquals(output, encode, static_cast<UINT>(fet_acsii), static_cast<UINT>(file.GetEncodingType()));
	ExpectEquals(output, seek, 0u, file.GetPosition());

	TString tData;

	file.ReadString(tData);

	std::string a_data = tData.GetRegString();

#ifdef WIN32
	ExpectEquals(output, read, data_win, a_data.c_str());
#else
	ExpectEquals(output, read, data, a_data.c_str());
#endif
	ExpectEquals(output, seek, tData.GetSize() + 1, file.GetPosition());

	file.Seek(5, 0);

	ExpectEquals(output, seek, 5u, file.GetPosition());

	file.ReadString(tData);
	a_data = tData.GetRegString();

#ifdef WIN32
	ExpectEquals(output, read, data_win + 5, a_data.c_str());
#else
	ExpectEquals(output, read, data + 5, a_data.c_str());
#endif
	ExpectEquals(output, seek, tData.GetSize() + 6, file.GetPosition());

	file.Close();
}

void TFileTester::Test2(std::string& output)
{
	TString fileName(file2.c_str());
	TFile file(fileName, TFile::t_file_open_existing | TFile::t_file_read);

	std::string encode = "File Encoding 2", seek = "File Seek 2", read = "Read 2";

	ExpectEquals(output, encode, static_cast<UINT>(fet_unicode_little), static_cast<UINT>(file.GetEncodingType()));
	ExpectEquals(output, seek, 0u, file.GetPosition());

	TString tData;

	file.ReadString(tData);

	WCHAR* buff = tData.GetBufferCopy();

#ifdef WIN32
	ExpectEquals(output, read, wData_win, buff);
#else
	ExpectEquals(output, read, wData, buff);
#endif
	ExpectEquals(output, seek, tData.GetSize() * 2 + 2, file.GetPosition());
	
	file.Seek(6, 0);

	ExpectEquals(output, seek, 6u, file.GetPosition());

	file.ReadString(tData);
	delete[] buff;
	buff = tData.GetBufferCopy();

	ExpectEquals(output, read, wData_win + 3, buff);
	ExpectEquals(output, seek, tData.GetSize() * 2 + 8, file.GetPosition());
	delete[] buff;
	buff = nullptr;
	file.Close();
}

void TFileTester::Before()
{
	std::ofstream f1;
	f1.open(file1.c_str(), std::fstream::out | std::fstream::trunc);
	
	for(UINT c = 0; c < 3; c++)
		f1.write(data, strlen(data));

	f1.close();
	TString tFile2(file2.c_str());
	TFile f2(tFile2, TFile::t_file_write | TFile::t_file_create_always);

	if (!f2.IsOpen())
	{
		int err = GetLastError();
		throw "ERROR! File failed to open";
	}

	for(UINT Rust = 0; Rust < 5; Rust++)
		f2.Write(wData, lstrlenW(wData) * sizeof(WCHAR));

	f2.Close();
}

void TFileTester::After()
{
}
