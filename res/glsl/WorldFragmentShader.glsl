#version 330 core
// min number of supported sampler objects per fragment shader is 16
const int MAX_SHADOW_CASCADES = 8;
const int BLOCKER_SAMPLE_COUNT = 3;

struct DirectionalLight {
	vec3 direction;
	vec3 color;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	float diameter;
};

struct PointLight {
	vec3 position;
	vec3 color;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec3 color;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gTexCoord;
uniform sampler2D gAmbient;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecular;

uniform sampler2D ssao;

uniform mat4 vp;

uniform vec3 viewPosition;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform float cascadeEnds[MAX_SHADOW_CASCADES];
uniform mat4 lightVP[MAX_SHADOW_CASCADES];
uniform sampler2DArray cascadedShadowMap;

in vec2 gCoord;

out vec4 FragColor;

vec3 viewDirection;

vec3 fragmentPosition;
vec3 normalVec;
vec2 texCoord;

float projectedDepth;
vec4 fragmentLightPositions[MAX_SHADOW_CASCADES];

float calculateShadow(out int shadowCascade);
vec3 calculateDirectionalLight(DirectionalLight light, vec3 ambient, vec3 diffuse, vec3 specular);
vec3 calculatePointLight(PointLight light, vec3 ambient, vec3 diffuse, vec3 specular);
vec3 calculateSpotLight(SpotLight light, vec3 ambient, vec3 diffuse, vec3 specular);

void main() {
	bool exists = bool(texture(gPosition, gCoord).a);
	if (!exists) {
		FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		return;
	}
	fragmentPosition = texture(gPosition, gCoord).xyz;

	vec4 projectedPosition = vp * vec4(fragmentPosition, 1.0f);
	projectedDepth = projectedPosition.z;

	normalVec = texture(gNormal, gCoord).xyz;
	//texCoord = texture(gTexCoord, gCoord).xy;
	for (int i = 0; i < MAX_SHADOW_CASCADES; ++i) {
		fragmentLightPositions[i] = lightVP[i] * vec4(fragmentPosition, 1.0f);
	}
	viewDirection = normalize(viewPosition - fragmentPosition);

	vec3 ambient = texture(gAmbient, gCoord).rgb;
	vec3 diffuse = texture(gDiffuse, gCoord).rgb;
	vec3 specular = texture(gSpecular, gCoord).rgb;

	float ambientOcclusion = texture(ssao, gCoord).r;

	ambient *= ambientOcclusion;

	vec3 result = calculateDirectionalLight(directionalLight, ambient, diffuse, specular);
	//result += calculateSpotLight(spotLight, ambient, diffuse, specular);
	FragColor = vec4(result, 1.0f);
}

float calculateShadow(out int shadowCascade) {
	shadowCascade = -1;
	for (int i = 0; i < MAX_SHADOW_CASCADES; ++i) {
		if (projectedDepth < cascadeEnds[i]) {
			shadowCascade = i;
			break;
		}
	}
	if (shadowCascade == -1) {
		return 0.0f;
	}
	vec4 fragPosLightSpace = fragmentLightPositions[shadowCascade];
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	float currentDepth = projCoords.z;
	// get depth of current fragment from light's perspective
	if (currentDepth > 1.0f) {
		return 0.0f;
	}

	// Blocker search
	vec2 texelSize = 1.0f / textureSize(cascadedShadowMap, 0).xy;
	vec3 shadowMapCoords = vec3(projCoords.xy, float(shadowCascade));
	float depthSum = 0;
	int numBlockers = 0;
	for (int i = -BLOCKER_SAMPLE_COUNT; i <= BLOCKER_SAMPLE_COUNT; ++i) {
		for (int j = -BLOCKER_SAMPLE_COUNT; j <= BLOCKER_SAMPLE_COUNT; ++j) {
			vec3 off = vec3(i, j, 0.0f);
			float sampleDepth = texture(cascadedShadowMap, shadowMapCoords + off * vec3(texelSize, 1.0f)).r;
			if (sampleDepth < currentDepth) {
				depthSum += sampleDepth;
				++numBlockers;
			}
		}
	}
	if (numBlockers == 0) {
		return 0.0f;
	}
	float blockerDepth = depthSum / float(numBlockers);

	float penumbraSize = (currentDepth - blockerDepth) * directionalLight.diameter / blockerDepth;
	int filterSize = min(int(penumbraSize), 5);
	int percentageCloserShadow = 0;
	//float closestDepth = texture(cascadedShadowMap, shadowMapCoords).r; 
	vec3 lightDirection = normalize(-directionalLight.direction);
	float bias = max(0.05 * (1.0 - dot(normalVec, lightDirection)), 0.005);
	for (int i = -filterSize; i <= filterSize; ++i) {
		for (int j = -filterSize; j <= filterSize; ++j) {
			vec3 off = vec3(i, j, 0.0f);
			float pcfDepth = texture(cascadedShadowMap, shadowMapCoords + off * vec3(texelSize, 1.0f)).r;
			if (currentDepth - bias > pcfDepth) {
				++percentageCloserShadow;
			}
		}
	}
	return float(percentageCloserShadow) / pow(2 * filterSize + 1, 2);
	//return 0.0f;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 ambient, vec3 diffuse, vec3 specular) {
	vec3 lightDirection = normalize(-light.direction);
	float diff = max(dot(normalVec, lightDirection), 0.0f);
	vec3 reflectDirection = reflect(-lightDirection, normalVec);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32.0f);
	int cascadeNum = -1;
	float shadow = calculateShadow(cascadeNum);
	/*
	if (cascadeNum == 0) {
		light.color = vec3(1.0f, 0.0f, 0.0f);
	} else if (cascadeNum == 1) {
		light.color = vec3(0.0f, 1.0f, 0.0f);
	} else if (cascadeNum == 2) {
		light.color = vec3(0.0f, 0.0f, 1.0f);
	} else if (cascadeNum == 3) {
		light.color = vec3(1.0f, 1.0f, 0.0f);
	}
	*/
	return light.color * (light.ambientStrength * ambient + (1.0f - shadow) * (light.diffuseStrength * diff * diffuse + light.specularStrength * spec * specular));
}

vec3 calculatePointLight(PointLight light, vec3 ambient, vec3 diffuse, vec3 specular) {
	vec3 difference = light.position - fragmentPosition;
	vec3 lightDir = normalize(difference);
	float diff = max(dot(normalVec, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normalVec);
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), 32.0f);
	float dist = length(difference);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	ambient *= attenuation * light.color * light.ambientStrength;
	diffuse *= attenuation * light.color * light.diffuseStrength * diff;
	specular *= attenuation * light.color * light.specularStrength * spec;
	int cascadeNum;
	float shadow = calculateShadow(cascadeNum);
	return ambient + (1.0f - shadow) * (diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 ambient, vec3 diffuse, vec3 specular) {
	vec3 difference = light.position - fragmentPosition;
	vec3 lightDir = normalize(difference);
	float theta = dot(lightDir, normalize(-light.direction));
	float distance = length(difference);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	ambient *= attenuation * light.color * light.ambientStrength;
	if (theta > light.outerCutOff) {
		float epsilon = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
		float diff = max(dot(normalVec, lightDir), 0.0);
		diffuse *= intensity * attenuation * light.color * light.diffuseStrength * diff;
		vec3 reflectDir = reflect(-lightDir, normalVec);
		float spec = pow(max(dot(viewDirection, reflectDir), 0.0), 32.0f);
		specular *= intensity * attenuation * light.color * light.specularStrength * spec;
		int cascadeNum;
		float shadow = calculateShadow(cascadeNum);
		return ambient + (1.0f - shadow) * (diffuse + specular);
	}
	return ambient;
}
