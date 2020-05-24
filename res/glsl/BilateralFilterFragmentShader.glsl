#version 330 core
const float RSIGMA = 0.3f;
const int BFK_SIZE = 7;

out float FragColor;

// BFK_SIZE * 2 + 1 = 15
uniform float kernel[BFK_SIZE * 2 + 1];
uniform sampler2D image;

in vec2 gCoord;

float normpdf(float v) {
	return 0.39894 * exp(-0.5f * v * v / (RSIGMA * RSIGMA)) / RSIGMA;
}

void main() {
	float color = texture(image, gCoord).r;
	vec2 texelSize = 1.0f / textureSize(image, 0);
	float final = 0.0f;
	float normalization = 0.0f;
	float rBase = normpdf(0.0f);
	for (int i = -BFK_SIZE; i <= BFK_SIZE; ++i) {
		for (int j = -BFK_SIZE; j <= BFK_SIZE; ++j) {
			vec2 texCoord = gCoord + vec2(i, j) * texelSize;
			if (texCoord.x < 0 || texCoord.y < 0 || texCoord.x > 1 || texCoord.y > 1) {
				continue;
			}
			float other = texture(image, texCoord).r;
			float factor = normpdf(other - color) * rBase * kernel[BFK_SIZE + i] * kernel[BFK_SIZE + j];
			normalization += factor;
			final += factor * other;
		}
	}
	FragColor = final / normalization;
}
