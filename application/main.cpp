#include <chrono>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

#include "clipboard.hpp"
#include "logger.hpp"

logger_t logger;

const auto handshake_str = R"("handshake")";

HHOOK hHook;
int c_count = 0;
DWORD c_last_time = 0;

std::string receive()
{
  uint32_t length = 0;
  std::cin.read(reinterpret_cast<char *>(&length), sizeof(length));
  logger.info(std::format("received message length : {}", length));
  
  std::string message(length, ' ');
  std::cin.read(message.data(), length);
  logger.info(std::format("received message content : {}", message));
  return message;
}

std::string receive_cway()
{
  char buf1[sizeof(uint32_t)];
  fread(buf1, sizeof(char), sizeof(uint32_t), stdin);
  uint32_t length = *reinterpret_cast<uint32_t *>(buf1);
  logger.info(std::format("received message length : {}", length));
  
  char buf2[length];
  std::fread(buf2, sizeof(char), length, stdin);
  std::string message(buf2, buf2 + length / sizeof(char));
  logger.info(std::format("received message content : {}", message));
  return message;
}

void send(const std::string & message)
{
  const uint32_t length = message.size();
  logger.info(std::format("sending message length : {}", length));
  std::cout.write(reinterpret_cast<const char *>(&length), sizeof(length));
  logger.info(std::format("sending message content : {}", message));
  std::cout.write(message.data(), length);
  std::cout.flush();
}

void send_cway(const std::string & message)
{
  const uint32_t length = message.size();
  std::fwrite(&length, sizeof(char), sizeof(uint32_t), stdout);
  logger.info(std::format("sending message length : {}", length));
  std::fwrite(message.data(), length, sizeof(char), stdout);
  logger.info(std::format("sending message content : {}", message));
  std::fflush(stdout);
}

void handshake()
{
  std::string message = receive();
  if (message == handshake_str)
    send(handshake_str);
  else
    throw std::runtime_error("failed handshake");
}

void main_1()
{
  POINT position;
  GetCursorPos(&position);
  logger.info(std::format("cursor position : {} {}", position.x, position.y));
  
  const std::string text = clipboard_text_get();
  logger.info(std::format("clipboard text : {}", text));
  
  std::ostringstream message_oss;
  message_oss << "{"
      << R"("text":")" << clipboard_text_get() << R"(")"
      << ","
      << R"("x":)" << position.x
      << ","
      << R"("y":)" << position.y
      << "}";
  const std::string message = message_oss.str();
  send_cway(message);
}


LRESULT CALLBACK KeyboardProc(const int nCode, const WPARAM wParam, const LPARAM lParam)
{
  if (nCode == HC_ACTION) {
    const auto * pKeyBoard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
      if (pKeyBoard->vkCode == 'C' && (GetAsyncKeyState(VK_LCONTROL) || GetAsyncKeyState(VK_RCONTROL))) {
        const DWORD currentTime = GetTickCount();
        if (currentTime - c_last_time <= GetDoubleClickTime()) {
          c_count++;
          if (c_count == 2) {
            c_count = 0;
            main_1();
          }
        } else {
          c_count = 1;
        }
        c_last_time = currentTime;
      }
    }
  }
  return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main()
{
  try {
    logger.ofs.open("browser-window-popup.log");
    logger.info("started");
    
    std::freopen(nullptr, "rb", stdin);
    if (std::ferror(stdin))
      throw std::runtime_error(std::strerror(errno));
    std::freopen(nullptr, "wb", stdout);
    if (std::ferror(stdout))
      throw std::runtime_error(std::strerror(errno));
    
    handshake();
    
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, nullptr, 0);
    
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    
    UnhookWindowsHookEx(hHook);
    
    logger.info("finished");
  }
  catch (std::exception const & e) {
    std::cerr << e.what() << '\n';
    logger.info(std::string("finished with error : ") + e.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
