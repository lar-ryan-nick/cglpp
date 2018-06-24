#version 330 core
out vec4 fragColor;

in vec2 textureCoordinates;

uniform vec4 backgroundColor;
uniform sampler2D image;

void main() {
	fragColor = vec4(1.0, 1.0f, 0.0f, 1.0f);//texture(image, textureCoordinates);
}
