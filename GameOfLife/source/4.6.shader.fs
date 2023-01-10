#version 460 core
in vec3 aColor;
out vec4 FragColor;

uniform vec3 inColor;

void main() {
	FragColor = vec4(aColor, 1.0f);
}