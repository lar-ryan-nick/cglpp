#version 330 core
layout (location = 0) in vec2 position;

out vec2 gCoord;

void main() {
	vec4 pos = vec4(position, 0.0f, 1.0f);
	gl_Position = pos;
	gCoord = position / 2.0f + 0.5f;
}
