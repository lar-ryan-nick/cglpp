#version 330 core
out vec4 fragColor;

in vec2 textureCoordinates;

uniform vec4 backgroundColor;
uniform sampler2D image;

void main() {
	fragColor = texture(image, textureCoordinates);
}
