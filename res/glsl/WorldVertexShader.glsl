#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in ivec4 boneIndicies;
layout (location = 4) in vec4 boneWeights;

out float gl_ClipDistance[8];

out vec2 texCoord;
out vec3 normalVec;
out vec3 fragmentPosition;

uniform mat4 model;
uniform mat4 vp;
uniform mat4 boneTransforms[100];
uniform vec4 clipPlane[8];
uniform int numPlanes;

void main() {
	mat4 boneTransform = boneTransforms[boneIndicies[0]] * boneWeights[0];
	boneTransform += boneTransforms[boneIndicies[1]] * boneWeights[1];
	boneTransform += boneTransforms[boneIndicies[2]] * boneWeights[2];
	boneTransform += boneTransforms[boneIndicies[3]] * boneWeights[3];
	vec4 position = model * boneTransform * vec4(aPos, 1.0f);
	gl_Position = vp * position;
	for (int i = 0; i < numPlanes; i++) {
		gl_ClipDistance[i] = dot(gl_Position, clipPlane[i]);
	}
	texCoord = aTexCoord;
	normalVec = normalize(mat3(transpose(inverse(model))) * mat3(boneTransform) * normal);
	fragmentPosition = position.xyz;
}
