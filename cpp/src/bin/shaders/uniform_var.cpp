// clang-format off
#include "utils.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

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

  glfwMakeContextCurrent(window);

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

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO); // ⚠️ ← VAO 开始"录制"

  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0); // ⚠️ ← VAO 停止录制（解绑）

  // ---------------------------------------------------------------
  // prepare shader

  int compileSuccess;
  char infoLog[512];

  // vertex shader
  std::string vert_shader = readFile("./shader/hello_triangle.glsl.vert");
  const char *vert_shader_src = vert_shader.c_str();
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vert_shader_src, nullptr);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileSuccess);
  if (!compileSuccess) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // fragment shader
  std::string frag_shader = readFile("./shader/uniform_var.glsl.frag");
  const char *frag_shader_src = frag_shader.c_str();
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &frag_shader_src, nullptr);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileSuccess);
  if (!compileSuccess) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // shader program
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // 每次循环迭代开始时检查GLFW是否被指示关闭。
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

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

    // ⚠️
    // 假如要set
    // uniform变量的话，因为它隐式地操作当前激活的程序，所以要先进行绑定
    glUseProgram(shaderProgram);

    float timeValue = glfwGetTime(); // 获取当前时间，从glfwInit开始计时
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // 实现呼吸效果

    // ⚠️
    // 注意，这里在查找之前不需要先调用glUseProgram，要查找的程序对象通过参数传入
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    if (vertexColorLocation ==
        -1) { // 假如返回 -1，则说明实际上并未找到对应位置
      std::cout << "ERROR::SHADER::FRAGMENT::UNIFORM_VAR NOT FOUND"
                << std::endl;
      exit(-1);
    }
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    // now render the triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents(); // 检查是否有任何事件被触发（例如键盘输入或鼠标移动事件），更新窗口状态，并调用相应的函数（就是通过回调方法来注册这些函数）
  }

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}