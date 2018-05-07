#include "size.h"

Size::Size(int w, int h) {
	width = w;
	height = h;
}

int Size::getWidth() {
	return width;
}

int Size::getHeight() {
	return height;
}
