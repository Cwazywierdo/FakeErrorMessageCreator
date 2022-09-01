#include "CreateShortcut.h"

HRESULT CreateShortcutFile(std::wstring savePath, std::wstring title, std::wstring message, std::wstring iconPath)
{
    HRESULT hrCoInit = CoInitialize(NULL);
    HRESULT hrCoCreate = NULL;
    HRESULT hres = NULL;
    IShellLink* pLink = nullptr;
    IPersistFile* pFile = nullptr;

    if (SUCCEEDED(hrCoInit)) {
        hrCoCreate = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_ALL, IID_IShellLink, (LPVOID*)&pLink);
    }

    // if Shell link instance successfully created...
    if (SUCCEEDED(hrCoCreate))
    {
        // set shortcut target
        WCHAR targetPath[MAX_PATH];
        GetModuleFileNameW(NULL, targetPath, MAX_PATH);
        hres = pLink->SetPath(targetPath);
    }

    // if the target is set successfully...
    if (SUCCEEDED(hres))
    {
        // set shortcut arguments
        std::wstring args = L"\"" + title + L"\" \"" + message + L"\"";
        hres = pLink->SetArguments(args.c_str());
    }

    // if the arguments are set successfully...
    if (SUCCEEDED(hres))
    {
        // set shortcut icon
        hres = pLink->SetIconLocation(iconPath.c_str(), 0);
    }

    // if the arguments are set successfully...
    if (SUCCEEDED(hres))
    {
        // create file reference from shortcut
        hres = pLink->QueryInterface(IID_IPersistFile, (LPVOID*)&pFile);
    }

    // if the Shell Link interface was successfully created...
    if (SUCCEEDED(hrCoCreate)) {
        pLink->Release();
    }

    // if shortcut file reference created successfully...
    if (SUCCEEDED(hres))
    {
        // save the shortcut file
        pFile->Save(savePath.c_str(), TRUE);
        pFile->Release();
    }
    
    if(SUCCEEDED(hrCoInit))
        CoUninitialize();
    
    // return a successful response, or the first error that was encountered
    return hres;
}