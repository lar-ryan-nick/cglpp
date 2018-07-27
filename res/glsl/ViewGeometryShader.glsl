#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 20) out;

const vec4 INVALID_VEC(0.0f, 0.0f, 0.0f, -20000000000000.0f);

vec4 intersect(vec4 line1[2], vec4 line2[2]) {
	// "this" is vertical
	if (line1[1].x == line1[0].x) {
		if (line2[1].x != line2[0].x) {
			if (((line2[0].x <= line1[0].x && line1[0].x <= line2[1].x) || (line2[1].x <= line1[0].x && line1[0].x <= line2[0].x))) {
				float change = (line2[1].x - line2[0].x) * (line1[0].x - line2[0].x);
				float y = (line2[1].y - line2[0].y) / change + line2[0].y;
				if (line1[0].y <= y && y <= line1[1].y || line1[1].y <= y && y <= line1[0].x) {
					float z = (line2[1].z - line2[0].z) / change + line2[0].z;
					float w = (other.point2.getW() - other.point1.getW()) / change + other.point1.getW();
					return vec4(line1[0].x, y, z, w);
				}
			} else {
				return INVALID_VEC;
			}
		} else {
			// both lines could be vertical and overlap here may want to consider having a line segment to return
			return INVALID_VEC;
		}
	}
	float denominator = line2[1].y - line2[0].y - (line2[1].x - line2[0].x) * (line1[1].y - line1[0].y) / (line1[1].x - line1[0].x);
	// Line segments have the same slope
	if (denominator == 0) {
		// both lines could overlap here may want to consider having a line segment to return
		return INVALID_VEC;
	}
	float j = (line1[0].y - line2[0].y + (line1[1].y - line1[0].y) * (line2[0].x - line1[0].x) / (line1[1].x - line1[0].x)) / denominator;
	float i = (line2[0].x + (line2[1].x - line2[0].x) * j - line1[0].x) / (line1[1].x - line1[0].x);
	if (i > 0 && i < 1 && j > 0 && j < 1) {
		return vec4(line1[0].x + (line1[1].x - line1[0].x) * i, line1[0].y + (line1[1].y - line1[0].y) * i, line1[0].z + (line1[1].z - line1[0].z) * i, point1.getW() + (point2.getW() - point1.getW()) * i);
	}
	return INVALID_VEC;
}

void main() {    
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    EndPrimitive();
}  