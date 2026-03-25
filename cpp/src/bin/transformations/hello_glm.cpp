// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include "shader_s.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
      glfwCreateWindow(800, 600, "Load Texture", nullptr, nullptr);
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

  // -----------------------------------------------------------------------------
  // binding vertexes ▲
  float vertices[] = {
      // positions(3) // colors(3) // texture coords(2)
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };

  // 再次通知OpenGL行的顶点格式
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // 这个函数专门用于将用户定义的数据复制到当前绑定缓冲区
  // - 第一个参数：缓冲区类型
  // - 第二个参数：要传递给缓冲区的数据大小（字节）
  // - 第三个参数：想要发送的实际数据
  // - 第四个参数：希望显卡如何管理给定的数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  unsigned int indices[] = {
      0, 1, 3, // 第一个三角形
      1, 2, 3, // 第二个三角形
  };
  // 这个函数专门用于将用户定义的数据复制到当前绑定缓冲区
  // - 第一个参数：缓冲区类型
  // - 第二个参数：要传递给缓冲区的数据大小（字节）
  // - 第三个参数：想要发送的实际数据
  // - 第四个参数：希望显卡如何管理给定的数据
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glBindVertexArray(0);

  // -----------------------------------------------------------------------------
  // prepare textures 🧻
  // 该函数首先接受一个图像文件的位置作为输入。然后它期望你提供三个整数作为其第二个、第三个和第四个参数，stb_image.h
  // 会用这些参数填充生成的图像的宽度、高度和颜色通道数。我们需要图像的宽度和高度以便之后生成纹理。
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("./asset/container.jpg", &width, &height, &nrChannels, 0);

  if (!data) {
    std::cout << "Failed to load texture" << std::endl;
    exit(-1);
  }

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // 1>:指定纹理目标；将其设置为GL_TEXTURE_2D意味此操作将在当前绑定的纹理对象上以相同的目标生成纹理
  // 2>:指定要创建纹理的mipmap层级。如果想要手动设置每个mipmap层级就可以设定这个参数，但是现在我们就将其保持在基础层级0
  // 3>:告诉OpenGL希望以何种格式存储纹理，因为图像只有RGB值，这里就使用RGB值来存储
  // 4/5>:设置生成纹理的宽度和高度，因为在记载图像的是偶就已经存储了这些值，使用相应的变量即可
  // 6>:应该始终为0(历史遗留问题)
  // 7/8>:指定源图像的格式和数据类型，这里我们以RGB值加载图像并将其存储为字符（字节）
  // 9>:最后一个参数是实际的图像数据
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  // 当调用g1TexImage2D后，当前绑定的纹理对象现在附带了纹理图像。然而，目前它只加载了纹理图像的基础层级，
  // 如果我们想要使用mipmaps，就必须手动指定所有不同的图像（通过不断递增第二个参数），或者，
  // 我们可以在生成纹理后调用glGenerateMipmap，这将自动为当前绑定的纹理生成所有所需的mipmaps。
  glGenerateMipmap(GL_TEXTURE_2D);

  // 办完了这一切之后，我们就可以释放图像内存了:
  stbi_image_free(data);

  Shader ourShader("./shader/hello_texture.glsl.vert",
                   "./shader/hello_texture.glsl.frag");
  ourShader.use();
  ourShader.setInt("ourTexture", 0);

  // 每次循环迭代开始时检查GLFW是否被指示关闭。
  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
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

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    ourShader.use();
    glBindVertexArray(
        VAO); // seeing as we only have a single VAO there's no need to bind it
              // every time, but we'll do so to keep things a bit more organized
    // 1. 指定我们想要绘制的模式
    // 2. 想要绘制的元素数量
    // 3. 索引的类型
    // 4. 指定EBO之中的偏移量
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}