// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

int main() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS 必须

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "OpenGL Sandbox", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window); // 将窗口的上下文设置为当前线程的主上下文

  // 初始化 GLAD：加载所有 OpenGL 函数指针
  // gladLoadGLLoader 接受一个"函数地址查询函数"作为参数
  // glfwGetProcAddress 是 GLFW 提供的查询函数，根据函数名返回其在当前平台的地址
  // (GLADloadproc) 是 C 风格强制类型转换，将 glfwGetProcAddress 的类型转换为
  // GLAD 期望的函数指针类型 返回值为 0（false）表示初始化失败，必须在调用任何
  // OpenGL 函数之前完成此步骤
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // 每次循环迭代开始时检查GLFW是否被指示关闭。
  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents(); // 检查是否有任何事件被触发（例如键盘输入或鼠标移动事件），更新窗口状态，并调用相应的函数（就是通过回调方法来注册这些函数）
  }

  glfwTerminate();
  return 0;
}
