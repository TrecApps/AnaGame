#pragma once
#define _WINSOCKAPI_
#include <TObject.h>
#include <TFileShell.h>
#include "AnaSock.h"

/**
 * Class: SocketWall
 * Purpose: Provides a Wall to filter URLs that a given app is not trusted to use
 */
class SocketWall :
    public TObject
{
public:

    virtual bool IsPermitted(const TString& url) =0;
    virtual void Works(TString& error)=0;

};

/**
 * Enum Class: socket_wall_type
 * Purpose: Allows apps to specify the Socket Wall Type to use
 */
typedef enum class socket_wall_type
{
    all,    // Socket Wall that allows all requests
    block,  // Socket Wall that has a list of URL's to block (Allows anything not on the list)
    allow   // Socket Wall that has a list of URL's to allow (blocks anything not on the list)
}socket_wall_type;

/**
 * Class: AdminSocketWall
 * Purpose: Used to Allow All requests through (Used by Anagame Apps like Web-Tours)
 */
class AdminSocketWall : public SocketWall
{
public:
    virtual bool IsPermitted(const TString& url)override;
    virtual void Works(TString& error)override;

    AdminSocketWall();
};


/**
 * Class: BlockListSocketWall
 * Purpose: Holds a list of URLS and targets that Anagame should Block
 */
class BlockListSocketWall : public SocketWall
{
public:
    virtual bool IsPermitted(const TString& url);

    BlockListSocketWall(TrecPointer<TFileShell> list);
    virtual void Works(TString& error)override;

private:
    TDataArray<TString> list;
    TString e;
};

/**
 * Class: AllowListSocketWall
 * Purpose: Holds a list of URLS and targets that Anagame should Block
 */
class AllowListSocketWall : public SocketWall
{
public:
    virtual bool IsPermitted(const TString& url);

    AllowListSocketWall(TrecPointer<TFileShell> list);
    virtual void Works(TString& error)override;

private:
    TDataArray<TString> list;
    TString e;
};


/**
 * 
 */
TString _ANA_SOCK_DLL PrepareSocketWall(socket_wall_type type, TrecPointer<TFileShell>);

bool _ANA_SOCK_DLL IsUrlPermitted(const TString& url);