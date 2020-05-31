#version 330 core
/* Uses a linear ray march with two phases
 * 1. linear steps
 * 2. If hit in phase 1, binary search *steps* times to make hit location more accurate
 * Reference: https://lettier.github.io/3d-game-shaders-for-beginners/screen-space-reflection.html
 */
out vec3 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;

uniform mat4 view;
uniform mat4 projection;

// TODO: make these uniform
float maxDistance = 30.0f;
float resolution = 0.7f;
int steps = 20;
float thickness = 0.09f;

in vec2 gCoord;

vec2 viewToUV(vec4 viewSpace);
bool uvIsValid(vec2 uv);

void main() {
	bool valid = bool(texture(gPosition, gCoord).a);
	if (!valid) {
		FragColor = vec3(0.0f);
		return;
	}
	vec2 screenSpaceSize = textureSize(gPosition, 0);
	vec3 fragmentPosition = (view * texture(gPosition, gCoord)).xyz;
	vec3 unitFragmentPosition = normalize(fragmentPosition);
	vec3 normal = texture(gNormal, gCoord).xyz;
	normal = normalize(transpose(inverse(mat3(view))) * normal);
	vec3 pivot = normalize(reflect(unitFragmentPosition, normal));
	vec4 viewStart = vec4(fragmentPosition, 1.0f);
	vec4 viewEnd = vec4(fragmentPosition + pivot * maxDistance, 1.0f);
	vec2 uvStart = viewToUV(viewStart);
	vec2 uvEnd = viewToUV(viewEnd);
	vec2 startFrag = uvStart * screenSpaceSize;
	vec2 endFrag = uvEnd * screenSpaceSize;
	vec2 deltaVec = endFrag.xy - startFrag.xy;
	bool useX = abs(deltaVec.x) >= abs(deltaVec.y);
	float delta = clamp(resolution, 0.0f, 1.0f) * (useX ? abs(deltaVec.x) : abs(deltaVec.y));
	vec2 increment = deltaVec / max(delta, 0.001f);

	float search0 = 0;
	float search1 = 0;
	bool hit0 = false;
	float viewDistance = fragmentPosition.z;
	float depth = thickness;
	vec2 frag = startFrag;
	vec2 validUV = vec2(-1.0f, -1.0f);
	vec3 currFragPosition = vec3(0.0f);
	for (int i = 0; i < int(delta); ++i) {
		frag += increment;
		vec2 uv = frag / screenSpaceSize;
		if (!uvIsValid(uv)) {
			search0 = search1;
			continue;
		}
		bool valid = bool(texture(gPosition, uv).a);
		if (!valid) {
			search0 = search1;
			continue;
		}
		currFragPosition = (view * texture(gPosition, uv)).xyz;
		vec2 rayPortion = (frag - startFrag) / deltaVec;
		search1 = useX ? rayPortion.x : rayPortion.y;
		viewDistance = (viewStart.z * viewEnd.z) / mix(viewEnd.z, viewStart.z, search1);
		depth = currFragPosition.z - viewDistance;
		if (depth > 0 && depth < thickness) {
			validUV = uv;
			hit0 = true;
			break;
		}
		search0 = search1;
	}
	if (hit0) {
		search1 = search0 + ((search1 - search0) / 2);
		for (int i = 0; i < steps; ++i) {
			frag = mix(startFrag.xy, endFrag.xy, search1);
			vec2 uv = frag / screenSpaceSize;
			if (!uvIsValid(uv)) {
				float temp = search1;
				search1 = search1 + ((search1 - search0) / 2);
				search0 = temp;
				search0 = search1;
				continue;
			}
			bool valid = bool(texture(gPosition, uv).a);
			if (!valid) {
				float temp = search1;
				search1 = search1 + ((search1 - search0) / 2);
				search0 = temp;
				search0 = search1;
				continue;
			}
			currFragPosition = (view * texture(gPosition, uv)).xyz;
			viewDistance = (viewStart.z * viewEnd.z) / mix(viewEnd.z, viewStart.z, search1);
			depth = currFragPosition.z - viewDistance;
			if (depth > 0 && depth < thickness) {
				validUV = uv;
				search1 = search0 + ((search1 - search0) / 2);
			} else {
				float temp = search1;
				search1 = search1 + ((search1 - search0) / 2);
				search0 = temp;
				search0 = search1;
			}
		}	
	}
	float visibility = uvIsValid(validUV) ? 1.0f : 0.0f;
	/*
	visibility *= 1.0f - max(0.0f, dot(-unitFragmentPosition, pivot));
	visibility *= 1.0f - clamp(depth / thickness, 0.0f, 1.0f);
	visibility *= 1.0f - clamp(length(currFragPosition - fragmentPosition) / maxDistance, 0.0f, 1.0f);
	visibility = clamp(visibility, 0.0f, 1.0f);
	*/
	// Store UV coordinates for reflection
	FragColor = vec3(validUV, visibility);
}

vec2 viewToUV(vec4 viewSpace) {
	vec4 clipSpace = projection * viewSpace;
	clipSpace /= clipSpace.w;
	vec2 uvSpace = clipSpace.xy * 0.5f + 0.5f;
	return uvSpace;
}

bool uvIsValid(vec2 uv) {
	return uv.x >= 0.0f && uv.x <= 1.0f && uv.y >= 0.0f && uv.y <= 1.0f;
}
