#version 330 core
// 片段着色器只需要一个输出变量，也就是一个大小为4的向量，用于定义计算的最后输出，可以使用out关键字声明输出值
out vec4 FragColor;

void main() { FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); }