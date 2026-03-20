#include "utils.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

std::string readFile(const char *path) {
  // C++ 文件流：以二进制模式打开，避免不同平台换行符转换。
  std::ifstream file(path, std::ios::binary);
  if (!file) {
    std::fprintf(stderr, "Failed to read shader file! CWD may by wrong.\n");
    std::exit(1);
  }

  // 把整个文件内容流式拷贝到字符串缓冲区中。
  std::ostringstream buffer;
  // file.rdbuf() 返回底层流缓冲区，<< 会一直读到 EOF。
  buffer << file.rdbuf();
  // 返回 std::string，内存由对象自己管理，不需要手动 free。
  return buffer.str();
}
