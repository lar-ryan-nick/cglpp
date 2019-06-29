#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;

out float gl_ClipDistance[8];

out vec2 texCoord;
out vec3 normalVec;
out vec3 fragmentPosition;

uniform mat4 model;
uniform mat4 vp;
uniform vec4 clipPlane[8];
uniform int numPlanes;

void main() {
	gl_Position = vp * model * vec4(aPos, 1.0f);
	for (int i = 0; i < numPlanes; i++) {
		gl_ClipDistance[i] = dot(gl_Position, clipPlane[i]);
	}
	texCoord = aTexCoord;
	normalVec = mat3(transpose(inverse(model))) * normal;
	fragmentPosition = vec3(model * vec4(aPos, 1.0f));
}
