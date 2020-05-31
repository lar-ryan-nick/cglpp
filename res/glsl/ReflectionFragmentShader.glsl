#version 330 core
const int HALF_FILTER_SIZE = 3;
out vec4 fragColor;

in vec2 gCoord;

uniform sampler2D gSpecular;
uniform sampler2D scene;
uniform sampler2D ssr;

void main() {
	float reflectivity = texture(gSpecular, gCoord).a;
	vec4 sceneColor = texture(scene, gCoord);
	vec4 totalReflectionColor = vec4(0.0f);
	int count = 0;
	vec2 texelSize = 1.0f / textureSize(ssr, 0);
	for (int i = -HALF_FILTER_SIZE; i <= HALF_FILTER_SIZE; ++i) {
		for (int j = -HALF_FILTER_SIZE; j <= HALF_FILTER_SIZE; ++j) {
			float visibility = 1.0f;
			vec2 off = vec2(i, j);
			vec2 texCoord = gCoord + off * texelSize;
			if (texCoord.x < 0 || texCoord.x > 1 || texCoord.y < 0 || texCoord.y > 1) {
				visibility *= 0.0;
			}
			vec3 ssrCoord = texture(ssr, texCoord).xyz;
			visibility *= clamp(ssrCoord.z, 0.0f, 1.0f);
			vec2 reflectionUV = ssrCoord.xy;
			vec4 reflectionColor = texture(scene, reflectionUV);
			totalReflectionColor += mix(sceneColor, reflectionColor, visibility * reflectivity);
		}
	}
	fragColor = totalReflectionColor / pow(HALF_FILTER_SIZE * 2 + 1, 2);
}
