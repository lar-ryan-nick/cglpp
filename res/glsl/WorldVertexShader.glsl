#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;

//out vec4 ourColor;
out vec2 TexCoord;
out vec3 normalVec;
out vec3 fragmentPosition;

void main() {
	gl_Position = aPos;
	//ourColor = vec4(aColor, 1.0);
	TexCoord = aTexCoord;
	normalVec = normal;
	fragmentPosition = aPos.xyz;
}
