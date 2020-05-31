#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in ivec4 boneIndicies;
layout (location = 5) in vec4 boneWeights;

uniform mat4 model;
uniform mat4 vp;
uniform mat4 boneTransforms[100];

out vec2 texCoord;
out vec3 normalVec;
out vec3 tangentVec;
out vec3 fragmentPosition;

void main() {
	mat4 boneTransform = boneTransforms[boneIndicies[0]] * boneWeights[0];
	boneTransform += boneTransforms[boneIndicies[1]] * boneWeights[1];
	boneTransform += boneTransforms[boneIndicies[2]] * boneWeights[2];
	boneTransform += boneTransforms[boneIndicies[3]] * boneWeights[3];
	vec4 worldPosition = model * boneTransform * vec4(aPos, 1.0f);
	gl_Position = vp * worldPosition;
	texCoord = aTexCoord;
	normalVec = normalize(mat3(transpose(inverse(model * boneTransform))) * normal);
	tangentVec = normalize(mat3(transpose(inverse(model * boneTransform))) * tangent);
	fragmentPosition = worldPosition.xyz;
}
