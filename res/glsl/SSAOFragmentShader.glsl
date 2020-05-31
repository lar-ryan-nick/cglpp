#version 330 core
out float FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;

uniform sampler2D noise;
// tile noise texture over screen, based on screen dimensions divided by noise size
uniform vec3 kernel[64];
uniform float screenWidth;
uniform float screenHeight;
uniform mat4 view;
uniform mat4 projection;

in vec2 gCoord;

const float bias = 0.025f;
const float radius = 0.5f;

void main() {
	bool exists = bool(texture(gPosition, gCoord).a);
	if (!exists) {
		FragColor = 0.0f;
		return;
	}
	vec2 noiseScale = vec2(screenWidth / 4.0, screenHeight / 4.0);
	vec3 fragPos = (view * vec4(texture(gPosition, gCoord).xyz, 1.0f)).xyz;
	vec3 normal = texture(gNormal, gCoord).xyz;
	normal = normalize(transpose(inverse(mat3(view))) * normal);
	vec3 randomVec = texture(noise, gCoord * noiseScale).xyz;
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	float occlusion = 0.0;
	for(int i = 0; i < 64; ++i) {
		// get sample position
		vec3 sample = TBN * kernel[i]; // from tangent to view-space
		sample = fragPos + sample * radius; 
		vec4 offset = vec4(sample, 1.0);
		offset = projection * offset; // from view to clip-space
		offset /= offset.w; // perspective divide
		offset.xy = offset.xy * 0.5 + 0.5; // transform to range 0.0 - 1.0
		vec4 sampleViewPosition = view * vec4(texture(gPosition, offset.xy).xyz, 1.0f); 
		float sampleDepth = sampleViewPosition.z;
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck; 
	} 
	occlusion = 1.0 - occlusion / 64;
	FragColor = occlusion;
}
