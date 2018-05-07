#ifndef SIZE_H
#define SIZE_H

class Size {
	int width;
	int height;
	public:
		Size(int w = 0, int h = 0);
		int getWidth();
		int getHeight();
};

#endif
