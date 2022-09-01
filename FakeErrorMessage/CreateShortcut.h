#pragma once

#include <shobjidl.h>
#include <shlguid.h>
#include <string>

HRESULT CreateShortcutFile(std::wstring path, std::wstring title, std::wstring message, std::wstring iconPath);
