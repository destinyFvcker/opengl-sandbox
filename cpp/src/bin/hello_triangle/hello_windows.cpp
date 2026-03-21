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

  int fbWidth = 0;
  int fbHeight = 0;
  glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
  glViewport(0, 0, fbWidth, fbHeight);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // -----------------------------------------------------------------------------
  // start working ⬇️

  // 每次循环迭代开始时检查GLFW是否被指示关闭。
  while (!glfwWindowShouldClose(window)) {
    // render
    // ------

    // 设置清屏颜色（RGBA）：这里是偏青色背景，alpha=1.0 表示完全不透明
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // 清除颜色缓冲区，用上面设置的颜色填充整个屏幕
    //
    // 假如不这样做:
    // 1. 上一帧残留会留在屏幕上，造成拖影 / 脏画面。
    // 2. 你这帧没覆盖到的区域会显示旧内容，而不是你期望的背景色。
    // 3. 每帧的渲染起点不一致，画面结果会不稳定、难调试。
    glClear(GL_COLOR_BUFFER_BIT);

    // 交换前后缓冲区，将前一帧刚刚渲染好的画面显示到窗口上
    // 1.你先在后台缓冲区（back buffer）画图。
    // 2.调用glfwSwapBuffers(window)后，后台缓冲区变成前台显示（front-buffer）。
    //
    // 这样可以避免闪烁和撕裂感，是双缓冲渲染的关键一步。
    glfwSwapBuffers(window);
    glfwPollEvents(); // 检查是否有任何事件被触发（例如键盘输入或鼠标移动事件），更新窗口状态，并调用相应的函数（就是通过回调方法来注册这些函数）
  }

  glfwTerminate();
  return 0;
}
