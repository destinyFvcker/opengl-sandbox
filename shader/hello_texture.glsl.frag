#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main() {
  // 为了对纹理的颜色进行采样，使用GLSL内置的texture函数，第一个参数是纹理采样器，第二个参数是相应的纹理坐标
  FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
}