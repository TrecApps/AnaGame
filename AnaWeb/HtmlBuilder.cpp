#include "pch.h"
#include "HtmlBuilder.h"

HtmlBuilder::HtmlBuilder(TrecPointer<TEnvironment> env)
{
    if (!env.Get())
        throw L"Null Environment passed";
    TrecPointer<TFileShell> fileLoc = env->GetRootDirectory();
    if (!fileLoc.Get())
        throw L"Null File Directory used in environment";

    if (!fileLoc->IsDirectory())
        throw L"Rot Directory in environment is not a directory";

    directory = fileLoc;
}

TString HtmlBuilder::BuildPage(TrecPointer<TFile> file)
{
    TString ret;

    if (!file.Get())
    {
        ret.Set(L"Null File Pointer passed!");
        return ret;
    }
    if (!file->IsOpen())
    {
        ret.Set(L"File Pointer was Closed!");
        return ret;
    }

    /// First, try to get the top line. In a well-formed HTML document, this usually says something like
    /// '<!DOCTYPE html>', however, we are not treating this as mandatoryand are prepared for the document to go straight to the <head>
    /// or <body> tags
    
    // marker to warn if we reached the end of file
    bool done = false;
    do
    {
        // Get the line
        if (!file->ReadString(topLine))
        {
            done = true; // end of file
            break;
        }
        topLine.Trim();
    } while (topLine.GetSize() == 0);

    // if done, then we reached the end of file before getting any data, so simply return
    if (done) return ret;

    // Check to see if we have a header block. Ideally, we shouldn't at this point, but not an error if we do
    if (topLine.StartsWith(L"<") && topLine.GetLower().Find(L"head") != -1)
    {
        // Create the header object
        header = TrecPointerKey::GetNewTrecPointer<HtmlHeader>(environment);

        // Have the header process the HTML
        ret.Set(header->ProcessHtml(file, topLine));
        // If an error occures (string is not empty), return to caller
        if (ret.GetSize()) return ret;
    }

    // Now check the body
    if (topLine.StartsWith(L"<") && topLine.GetLower().Find(L"body") != -1)
    {
        body = TrecPointerKey::GetNewTrecPointer<HtmlBody>(environment);
        ret.Set(body->ProcessHtml(file, topLine));
        if (ret.GetSize()) return ret;
    }

    // Not wanting to override the topLine, use a temp variable for the rest fo the inspection
    TString line;

    // Ideally, we should be coming up on the head block. Repeat the process as above with the variable string
    do
    {
        if (!file->ReadString(line))
        {
            done = true;
            break;
        }
        line.Trim();
    } while (line.GetSize() == 0);

    if (done) return ret;

    if (line.StartsWith(L"<") && line.GetLower().Find(L"head") != -1)
    {
        header = TrecPointerKey::GetNewTrecPointer<HtmlHeader>(environment);
        ret.Set(header->ProcessHtml(file, line));
        if (ret.GetSize()) return ret;
    }

    // Just in case we get the body first (or the header was detected as 'topLine'
    if (line.StartsWith(L"<") && line.GetLower().Find(L"body") != -1)
    {
        body = TrecPointerKey::GetNewTrecPointer<HtmlBody>(environment);
        ret.Set(body->ProcessHtml(file, line));
        if (ret.GetSize()) return ret;
    }


    // Ideally, we should get the body tag 
    do
    {
        if (!file->ReadString(line))
        {
            done = true;
            break;
        }
        line.Trim();
    } while (line.GetSize() == 0);

    if (done) return ret;

    // Just in case the head block was provided last
    if (line.StartsWith(L"<") && line.GetLower().Find(L"head") != -1)
    {
        header = TrecPointerKey::GetNewTrecPointer<HtmlHeader>(environment);
        ret.Set(header->ProcessHtml(file, line));
        if (ret.GetSize()) return ret;
    }

    if (line.StartsWith(L"<") && line.GetLower().Find(L"body") != -1)
    {
        body = TrecPointerKey::GetNewTrecPointer<HtmlBody>(environment);
        ret.Set(body->ProcessHtml(file, line));
        if (ret.GetSize()) return ret;
    }

    return ret;
}