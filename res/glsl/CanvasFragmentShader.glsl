#version 330 core
out vec4 fragColor;

in vec2 textureCoordinate;

uniform sampler2D image;

void main() {
	fragColor = texture(image, textureCoordinate);
}
