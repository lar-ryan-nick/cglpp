#version 330 core
layout (location = 0) in vec2 position;

uniform mat4 projection;
uniform mat4 textureMapper;

out vec2 textureCoordinate;

void main() {
	vec4 pos = vec4(position, 0.0, 1.0);
	gl_Position = pos;
	textureCoordinate = vec2(textureMapper * pos);
}
