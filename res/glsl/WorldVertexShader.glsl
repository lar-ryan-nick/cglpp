#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;

//out vec4 ourColor;
out vec2 TexCoord;
out vec3 normalVec;
out vec3 fragmentPosition;


uniform mat4 model;
uniform mat4 vp;

void main() {
	gl_Position = vp * model * vec4(aPos, 1.0f);
	//ourColor = vec4(aColor, 1.0f);
	TexCoord = aTexCoord;
	normalVec = mat3(transpose(inverse(model))) * normal;
	fragmentPosition = vec3(model * vec4(aPos, 1.0f));
}
