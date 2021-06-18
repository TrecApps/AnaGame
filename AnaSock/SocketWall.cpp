#include "SocketWall.h"


TrecPointer<SocketWall> theWall;

#define PREFIX_URL_COUNT 4

TString prefixUrls[PREFIX_URL_COUNT] = {
    TString(L"http:/"),
    TString(L"https:/"),
    TString(L"file:/"),
    TString(L"ssh:/")
};

TString GetBaseUrl(const TString& url)
{
    TString c_url(url);

    while (c_url.Replace(L"//", L"/"));

    for (UINT Rust = 0; Rust < PREFIX_URL_COUNT; Rust++)
    {
        if (c_url.StartsWith(prefixUrls[Rust], true))
        {
            c_url.Delete(0, prefixUrls[Rust].GetSize());
        }
    }

    return c_url.SubString(0, c_url.Find(L'/'));
}



bool AdminSocketWall::IsPermitted(const TString& url)
{
    return true;
}

void AdminSocketWall::Works(TString& error)
{
    error.Empty();
}

AdminSocketWall::AdminSocketWall()
{
}


bool BlockListSocketWall::IsPermitted(const TString& url)
{
    TString c_url(GetBaseUrl(url));


    for (UINT Rust = 0; Rust < this->list.Size(); Rust++)
    {
        if (!c_url.Compare(list[Rust]))
        {
            return false;
        }
    }

    return true;
}


BlockListSocketWall::BlockListSocketWall(TrecPointer<TFileShell> list)
{
}

void BlockListSocketWall::Works(TString& error)
{
    error.Set(e);
}

bool AllowListSocketWall::IsPermitted(const TString& url)
{
    TString c_url(GetBaseUrl(url));


    for (UINT Rust = 0; Rust < this->list.Size(); Rust++)
    {
        if (!c_url.Compare(list[Rust]))
        {
            return true;
        }
    }

    return false;
}

AllowListSocketWall::AllowListSocketWall(TrecPointer<TFileShell> list)
{
}

void AllowListSocketWall::Works(TString& error)
{
    error.Set(e);
}

TString PrepareSocketWall(socket_wall_type type, TrecPointer<TFileShell> info)
{
    TString ret;
    switch (type)
    {
    case socket_wall_type::all:
        theWall = TrecPointerKey::GetNewTrecPointerAlt<SocketWall, AdminSocketWall>();
        break;
    case socket_wall_type::allow:
        theWall = TrecPointerKey::GetNewTrecPointerAlt<SocketWall, AllowListSocketWall>(type);
        theWall->Works(ret);
        break;
    case socket_wall_type::block:
        theWall = TrecPointerKey::GetNewTrecPointerAlt<SocketWall, BlockListSocketWall>(type);
        theWall->Works(ret);
    }

    if (ret.GetSize())
        theWall.Nullify();
    return ret;
}

bool IsUrlPermitted(const TString& url)
{
    if(!theWall.Get())
        return false;
    return theWall->IsPermitted(url);
}
