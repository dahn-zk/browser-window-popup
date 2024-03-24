#ifndef CLIPBOARD_HPP
#define CLIPBOARD_HPP

#include <windows.h>
#include <string>

inline std::string clipboard_text_get()
{
  if (!OpenClipboard(nullptr))
    return "";
  
  const auto handle = GetClipboardData(CF_TEXT);
  if (handle == nullptr)
    return "";
  
  const auto text_pzs = static_cast<char *>(GlobalLock(handle));
  if (text_pzs == nullptr)
    return "";
  
  std::string text(text_pzs);
  
  GlobalUnlock(handle);
  
  CloseClipboard();
  
  return text;
}

#endif //CLIPBOARD_HPP
