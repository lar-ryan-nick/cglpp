#ifndef COLOR_H
#define COLOR_H

class Color {
	float red;
	float green;
	float blue;
	float alpha;
	void fixValues();
	public:
		Color(float r, float g, float b, float a = 1.0f);
		Color(int r, int g, int b, int a = 1);
		float getRed() const;
		float getGreen() const;
		float getBlue() const;
		float getAlpha() const;
};

#endif
