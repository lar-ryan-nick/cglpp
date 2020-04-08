#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in ivec4 boneIndicies;
layout (location = 4) in vec4 boneWeights;

uniform mat4 lightVP;
uniform mat4 model;
uniform mat4 boneTransforms[100];

void main() {
	mat4 boneTransform = boneTransforms[boneIndicies[0]] * boneWeights[0];
	boneTransform += boneTransforms[boneIndicies[1]] * boneWeights[1];
	boneTransform += boneTransforms[boneIndicies[2]] * boneWeights[2];
	boneTransform += boneTransforms[boneIndicies[3]] * boneWeights[3];
	vec4 position = model * boneTransform * vec4(aPos, 1.0f);
	gl_Position = lightVP * position;
}
