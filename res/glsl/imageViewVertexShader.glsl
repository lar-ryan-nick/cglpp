#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out vec2 textureCoordinates;

uniform vec4 clipRect;
uniform mat4 model;
uniform mat4 parentModel;
uniform mat4 projection;

void main() {
	gl_Position = model * vec4(position, 0.0, 1.0);
	textureCoordinates = texCoord;
	gl_ClipDistance[0] = gl_Position.x - clipRect.x;
	gl_ClipDistance[1] = gl_Position.y - clipRect.y;
	gl_ClipDistance[2] = clipRect.z - gl_Position.x;
	gl_ClipDistance[3] = clipRect.w - gl_Position.y;
	gl_Position = projection * parentModel * gl_Position;
}
