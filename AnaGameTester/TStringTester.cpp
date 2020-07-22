#include "TStringTester.h"

void TStringTester::RunTests(std::string& output)
{
	TestFind(output);
	TestInsert(output);
	TestAppendChar(output);
	TestFormet(output);
	TestDelete(output);
	TestLower(output);
	TestUpper(output);
	TestReverse(output);
	TestTrim(output);
	TestReplace(output);
}

std::string TStringTester::GetTestName()
{
	return std::string("TString");
}

void TStringTester::Before()
{
}

void TStringTester::After()
{
}

void TStringTester::TestFind(std::string& output)
{
	TString str(L"Star Wars Revenge of the Sith is the best movie!");
	std::string findStr("Find");
	ExpectEquals(output, findStr, static_cast<UINT>(48), str.GetSize());

	ExpectEquals(output, findStr, 4, static_cast<long long>(str.Find(L' ')));
	ExpectEquals(output, findStr, 41, static_cast<long long>(str.FindLast(L' ')));
	ExpectEquals(output, findStr, 9, static_cast<long long>(str.Find(L' ', 5)));
	ExpectEquals(output, findStr, 36, static_cast<long long>(str.FindLast(L' ', 40)));
	
	TString str2(L"'Why do you insist on torturing me?' asked the character!");
	TString s(L's');
	ExpectEquals(output, findStr, 38, static_cast<long long>(str2.FindOutOfQuotes(s)));
}

void TStringTester::TestInsert(std::string& output)
{
	TString spin(L"This is how you eat  Trooper!");
	TString spin2(L"hor: Love and Thunder");

	spin.Insert(20, TString(L"spinach"));
	spin2.Insert(0, L'T');


	std::string insertStr("Insert");
	
	WCHAR* cop = spin.GetBufferCopy();

	ExpectEquals(output, insertStr, L"This is how you eat spinach Trooper!", cop);

	delete[] cop;
	cop = spin2.GetBufferCopy();

	ExpectEquals(output, insertStr, L"Thor: Love and Thunder", cop);
	delete[] cop;
	cop = nullptr;
}

void TStringTester::TestAppendChar(std::string& output)
{
	TString app1(L"Orange c");

	app1.AppendChar(L'a');
	app1.AppendChar(L't');

	WCHAR* cop = app1.GetBufferCopy();

	std::string appStr("AppendChar");

	ExpectEquals(output, appStr, L"Orange cat", cop);
	delete[] cop;

	app1.Append(TString(L" wants to go outside!"));
	cop = app1.GetBufferCopy();

	ExpectEquals(output, appStr, L"Orange cat wants to go outside!", cop);
	delete[] cop;

}

void TStringTester::TestFormet(std::string& output)
{
	TString form;
	std::string formStr("Format");

	form.Format(TString(L"%d"), 93);
	WCHAR* cop = form.GetBufferCopy();
	ExpectEquals(output, formStr, L"93", cop);
	delete[] cop;

	form.Format(TString(L"|%20d|"), 93);
	cop = form.GetBufferCopy();
	ExpectEquals(output, formStr, L"|                  93|", cop);
	delete[] cop;

	form.Format(TString(L"|%-20d|"), 93);
	cop = form.GetBufferCopy();
	ExpectEquals(output, formStr, L"|93                  |", cop);
	delete[] cop;

	form.Format(TString(L"|%020d|"), 93);
	cop = form.GetBufferCopy();
	ExpectEquals(output, formStr, L"|00000000000000000093|", cop);
	delete[] cop;

	form.Format(TString(L"|%,d|"), 10000000);
	cop = form.GetBufferCopy();
	ExpectEquals(output, formStr, L"|10,000,000|", cop);
	delete[] cop;

	form.Format(TString(L"|%x|"), 93);
	cop = form.GetBufferCopy();
	ExpectEquals(output, formStr, L"|5d|", cop);
	delete[] cop;

	form.Set(L"A Couple numbers: ");
	form.AppendFormat(TString(L"%d %d %d"), 34, 293847, 1237);
	cop = form.GetBufferCopy();
	ExpectEquals(output, formStr, L"A Couple numbers: 34 293847 1237", cop);
	delete[] cop;
}

void TStringTester::TestDelete(std::string& output)
{
	TString del(L"My crazy cat!");
	std::string delStr("Delete");

	del.Delete(2, 6);
	WCHAR* cop = del.GetBufferCopy();

	ExpectEquals(output, delStr, L"My cat!", cop);
	delete[] cop;
}

void TStringTester::TestLower(std::string& output)
{
	TString low(L"So Why Do We HAVE CAPS?");
	TString lowered = low.GetLower();
	std::string lowStr("Lowercase");

	WCHAR* cop = lowered.GetBufferCopy();

	ExpectEquals(output, lowStr, L"so why do we have caps?", cop);
	delete[] cop;
}

void TStringTester::TestUpper(std::string& output)
{
	TString up(L"I'm not yelling!");
	TString uppered = up.GetUpper();
	std::string upStr("Uppercase");

	WCHAR* cop = uppered.GetBufferCopy();

	ExpectEquals(output, upStr, L"I'M NOT YELLING!", cop);
	delete[] cop;
}

void TStringTester::TestReverse(std::string& output)
{
	TString rev1(L"repoort");
	TString rev2(L"!repoort");
	TString rev3(L"edoc htiw detixe saH");

	std::string revStr("Reverse");

	TString _rev1 = rev1.GetReverse();
	TString _rev2 = rev2.GetReverse();
	TString _rev3 = rev3.GetReverse();

	WCHAR* cop = _rev1.GetBufferCopy();
	ExpectEquals(output, revStr, L"trooper", cop);
	delete[] cop;

	cop = _rev2.GetBufferCopy();
	ExpectEquals(output, revStr, L"trooper!", cop);
	delete[] cop;

	cop = _rev3.GetBufferCopy();
	ExpectEquals(output, revStr, L"Has exited with code", cop);
	delete[] cop;

}

void TStringTester::TestTrim(std::string& output)
{
	TString trim1(L" trimmed  ");
	TString trim2(L" \t\n trimmed \t\t\t");
	std::string trimStr("Trim");

	TString lTrim1 = trim1.GetTrimLeft();
	TString rTrim1 = trim1.GetTrimRight();
	TString _trim1 = trim1.GetTrim();

	TString lTrim2 = trim2.GetTrimLeft();
	TString rTrim2 = trim2.GetTrimRight();
	TString _trim2 = trim2.GetTrim();

	WCHAR* cop = lTrim1.GetBufferCopy();
	ExpectEquals(output, trimStr, L"trimmed  ", cop);
	delete[] cop;

	cop = rTrim1.GetBufferCopy();
	ExpectEquals(output, trimStr, L" trimmed", cop);
	delete[] cop;

	cop = _trim1.GetBufferCopy();
	ExpectEquals(output, trimStr, L"trimmed", cop);
	delete[] cop;

	cop = lTrim2.GetBufferCopy();
	ExpectEquals(output, trimStr, L"trimmed \t\t\t", cop);
	delete[] cop;

	cop = rTrim2.GetBufferCopy();
	ExpectEquals(output, trimStr, L" \t\n trimmed", cop);
	delete[] cop;

	cop = _trim2.GetBufferCopy();
	ExpectEquals(output, trimStr, L"trimmed", cop);
	delete[] cop;
}

void TStringTester::TestReplace(std::string& output)
{
	TString rep1(L"Do You have a %c? I have an orange %c!");
	TString rep2(L"Loki Laufeyson wants to rule Asgard!");

	int times = 0;
	std::string repStr("Replace");

	TString _rep1 = rep1.GetReplace(times, TString(L"%c"), TString(L"cat"));
	WCHAR* cop = _rep1.GetBufferCopy();
	ExpectEquals(output, repStr, L"Do You have a cat? I have an orange cat!", cop);
	ExpectEquals(output, repStr, 2, static_cast<long long>(times));
	delete[] cop;

	TString _rep2 = rep2.GetReplace(times, TString(L"Laufey"), TString(L"Odin"));
	cop = _rep2.GetBufferCopy();
	ExpectEquals(output, repStr, L"Loki Odinson wants to rule Asgard!", cop);
	ExpectEquals(output, repStr, 1, static_cast<long long>(times));
	delete[] cop;
}
