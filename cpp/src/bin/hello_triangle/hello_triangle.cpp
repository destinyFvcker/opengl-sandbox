// clang-format off
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include "utils.h"
#include <iostream>
#include <string>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  (void)window;
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
      glfwCreateWindow(800, 600, "Hello Triangle", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }

  int fbWidth = 0;
  int fbHeight = 0;
  glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
  glViewport(0, 0, fbWidth, fbHeight);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO); // ⚠️ ← VAO 开始"录制"

  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // 这个函数专门用于将用户定义的数据复制到当前绑定缓冲区
  // - 第一个参数：缓冲区类型
  // - 第二个参数：要传递给缓冲区的数据大小（字节）
  // - 第三个参数：想要发送的实际数据
  // - 第四个参数：希望显卡如何管理给定的数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // readFile 返回 std::string，避免了 C 风格 malloc/free 的手动内存管理。
  std::string vert_shader = readFile("./shader/hello_triangle.glsl.vert");
  // c_str() 提供只读 C 字符串视图，供 OpenGL 这种 C API 使用。
  // 注意：只要 simple_shader 还活着，这个指针就是有效的。
  const char *vert_shader_src = vert_shader.c_str();

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  // glShaderSource 的第三个参数类型是 const char* const*，所以这里传地址。
  // - 第一个参数：需要编译的着色器对象
  // - 第二个参数：指定我们传递的源代码字符串数量
  // - 第三个参数：顶点着色器的实际源代码
  glShaderSource(vertexShader, 1, &vert_shader_src, nullptr);
  glCompileShader(vertexShader);

  int compileSuccess;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileSuccess);
  if (!compileSuccess) {
    // 这里的第三个参数是一个指向实际写入了多少个字符的number的指针：GLsizei*
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // 片段着色器的编译过程实际上和顶点着色器类似
  std::string frag_shader = readFile("./shader/hello_triangle.glsl.frag");
  const char *frag_shader_src = frag_shader.c_str();

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragmentShader, 1, &frag_shader_src, nullptr);
  glCompileShader(fragmentShader);

  // 现在这两个着色器都已经编译完成，剩下的任务就是将这两个着色器对象链接成一个可以用于渲染的程序。
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // 告诉OpenGL如何解释顶点数据
  // @1.
  // 第一个参数指定了我们要配置的顶点属性。请记住，我们在顶点着色器中使用 layout
  // (location = 0) 指定了位置顶点属性的位置。这将顶点属性的位置设置为
  // 0，因为我们想将数据传递给这个顶点属性，所以我们传入O。
  // @2.
  // 下一个参数指定了顶点属性的大小。顶点属性是一个 vec3，因此由 3 个值组成。
  // @3.
  // 第三个参数指定了数据的类型，即 GL_FLOAT（GLSL 中的 vec* 由浮点值组成）。
  // @4.
  // 第四个参数指定了是否要对数据进行归一化。如果我们输入的是整数数据类型（int、byte）并且将此参数设置为
  // GL_TRUE，那么整数数据在转换为浮点数时会被归一化到0（或-1对于有符号数据）和1之间。
  // 对于我们的情况，这并不相关，因此我们将此参数设置为GL_FALSE。
  // @5.
  // 第五个参数称为步长（stride），它告诉我们连续顶点属性之间的间隔。由于下一个位置数据正好位于浮点数大小的
  // 3倍位置，我们指定该值为步长。请注意，由于我们知道数组是紧密打包的（下一个顶点属性值之间没有空隙），
  // 我们也可以将步长指定为0，让OpenGL自动计算步长（这仅在数据紧密打包时有效）。
  // 每当有多个顶点属性时，我们必须仔细定义每个顶点属性之间的间隔。
  // @6.
  // 最后一个参数的类型是void*，因此需要那个奇怪的强制转换。这是指顶点属性数据在缓冲区中的起始偏移量。
  // 由于位置数据位于数据数组的起始位置，因此该值就是0。
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  // 启用顶点属性，将顶点属性的位置作为其参数，它们默认都是禁用的
  glEnableVertexAttribArray(0);

  glBindVertexArray(0); // ⚠️ ← VAO 停止录制（解绑）

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

    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(
        VAO); // seeing as we only have a single VAO there's no need to bind it
              // every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindVertexArray(0); // no need to unbind it every time

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteShader(vertexShader);
  glDeleteBuffers(1, &VBO);
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
