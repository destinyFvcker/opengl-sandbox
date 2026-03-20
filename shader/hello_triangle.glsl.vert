// GLSL看起来类似于C语言，每个着色器都以声明其版本开始，GLSL的版本号与OpenGL的版本号一致
#version 330 core // 明确指出我们使用的是核心配置文件功能
// 使用in关键字在顶点着色器之中声明所有输入的顶点属性，还特别通过layout设置输出变量的位置
layout(location = 0) in vec3 aPos;

// 这里实际上对输入数据没有进行任何处理，只是将其直接传递给着色器的输出，gl_Posision这个变量实际上是预定义的
void main() { gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); }