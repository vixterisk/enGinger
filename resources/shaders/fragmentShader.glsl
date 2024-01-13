#version 330 core
out vec4 FragColor;

in vec4 outColor;
uniform float uniformColor; // we set this variable in the OpenGL code.

void main()
{
   float sin = sin(uniformColor) / 2.0f + 0.5f;
   FragColor = vec4(outColor.r * (1.0f - sin), outColor.g * 0.5f * sin, outColor.b * sin, outColor.a);
}