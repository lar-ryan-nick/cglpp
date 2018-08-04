#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 100) out;

struct Vertex {
	vec4 vertex;
	int next1;
	int next2;
	bool exiting;
};

// must be the same as MAX_VERTICIES in cgl::View
const int MAX_CLIP_VERTICIES = 50;

in vec2 TexCoord[3];

out vec2 texCoord;
out vec4 color;

uniform vec2 clipRegion[MAX_CLIP_VERTICIES];
uniform int numClipVerticies;

bool clipContains(vec2 position) {
	vec2 insidePoint;
	for (int i = 0; i < numClipVerticies; i++) {
		insidePoint += clipRegion[i];
	}
	insidePoint /= numClipVerticies;
	for (int i = 0; i < numClipVerticies; i++) {
		vec3 diff = vec3(clipRegion[(i + 1) % numClipVerticies] - clipRegion[i], 0.0f);
		vec3 insideSide = cross(vec3(insidePoint - clipRegion[i], 0.0f), diff);
		vec3 querySide = cross(vec3(position - clipRegion[i], 0.0f), diff);
		if (sign(insideSide.z) != sign(querySide.z)) {
			return false;
		}
	}
	return true;
}

bool sourceContains(vec2 position) {
	vec2 insidePoint;
	for (int i = 0; i < 3; i++) {
		insidePoint += gl_in[i].gl_Position.xy / gl_in[i].gl_Position.w;
	}
	insidePoint /= 3;
	for (int i = 0; i < 3; i++) {
		vec3 diff = vec3(gl_in[(i + 1) % 3].gl_Position.xy / gl_in[(i + 1) % 3].gl_Position.w - gl_in[i].gl_Position.xy / gl_in[i].gl_Position.w, 0.0f);
		vec3 insideSide = cross(vec3(insidePoint - gl_in[i].gl_Position.xy / gl_in[i].gl_Position.w, 0.0f), diff);
		vec3 querySide = cross(vec3(position - gl_in[i].gl_Position.xy / gl_in[i].gl_Position.w, 0.0f), diff);
		if (sign(insideSide.z) != sign(querySide.z)) {
			return false;
		}
	}
	return true;
}

bool intersectAt(vec4 line1[2], vec2 line2[2], out vec4 intersect) {
	vec3 homLine1 = cross(line1[0].xyw, line1[1].xyw);
	vec3 homLine2 = cross(vec3(line2[0].xy, 1.0), vec3(line2[1].xy, 1.0));
	vec3 intersection = cross(homLine1, homLine2);
	if (intersection.z == 0.0) {
		return false;
	}
	intersection *= line1[0].w / intersection.z;
	vec3 tempLine = line1[1].xyz * line1[0].w / line1[1].w;
	if (min(line1[0].x, tempLine.x) < intersection.x && intersection.x < max(line1[0].x, tempLine.x)) {
		return false;
	}
	intersect = vec4(intersection.xy, (tempLine.z - line1[0].z) / (tempLine.x - line1[0].x) * (intersection.x - line1[0].x) + line1[0].z, intersection.z);
	return true;
}

int findIntersect(Vertex intersections[2 * MAX_CLIP_VERTICIES], vec4 intersect) {
	for (int i = 0; i < intersections.length(); i++) {
		if (intersect == intersections[i].vertex) {
			return i;
		}
	}
	// should never get to this line
	return 0;
}

void main() {
	/*
	float temp = gl_in[1].gl_Position.y - gl_in[1].gl_Position.z / gl_in[0].gl_Position.z * gl_in[0].gl_Position.y;
	float a = (TexCoord[2].x - gl_in[2].gl_Position.y / temp * (TexCoord[1].x - gl_in[1].gl_Position.z / gl_in[0].gl_Position.z * TexCoord[0].x) - gl_in[2].gl_Position.z / gl_in[0].gl_Position.z * (TexCoord[0].x - gl_in[0].gl_Position.y / temp * (TexCoord[1].x - gl_in[1].gl_Position.z / gl_in[0].gl_Position.z * TexCoord[0].x))) / (gl_in[2].gl_Position.x + gl_in[2].gl_Position.y / temp * (-gl_in[1].gl_Position.x + gl_in[1].gl_Position.z / gl_in[0].gl_Position.z * gl_in[0].gl_Position.x) + gl_in[2].gl_Position.z / gl_in[0].gl_Position.z * (-gl_in[0].gl_Position.y / temp * (-gl_in[1].gl_Position.x + gl_in[1].gl_Position.z / gl_in[0].gl_Position.z * gl_in[0].gl_Position.x)) - gl_in[0].gl_Position.x);
	float b = (TexCoord[1].x - a * gl_in[1].gl_Position.x - gl_in[1].gl_Position.z / gl_in[0].gl_Position.z * (TexCoord[0].x - a * gl_in[0].gl_Position.x)) / (gl_in[1].gl_Position.y - gl_in[1].gl_Position.z / gl_in[0].gl_Position.z * gl_in[0].gl_Position.y);
	float c = (TexCoord[0].x - b * gl_in[0].gl_Position.y - a * gl_in[0].gl_Position.x) / gl_in[0].gl_Position.z;
	float d = (TexCoord[2].y - gl_in[2].gl_Position.y / temp * (TexCoord[1].y - gl_in[1].gl_Position.z / gl_in[0].gl_Position.z * TexCoord[0].y) - gl_in[2].gl_Position.z / gl_in[0].gl_Position.z * (TexCoord[0].y - gl_in[0].gl_Position.y / temp * (TexCoord[1].y - gl_in[1].gl_Position.z / gl_in[0].gl_Position.z * TexCoord[0].y))) / (gl_in[2].gl_Position.x + gl_in[2].gl_Position.y / temp * (-gl_in[1].gl_Position.x + gl_in[1].gl_Position.z / gl_in[0].gl_Position.z * gl_in[0].gl_Position.x) + gl_in[2].gl_Position.z / gl_in[0].gl_Position.z * (-gl_in[0].gl_Position.y / temp * (-gl_in[1].gl_Position.x + gl_in[1].gl_Position.z / gl_in[0].gl_Position.z * gl_in[0].gl_Position.x)) - gl_in[0].gl_Position.x);
	float e = (TexCoord[1].y - d * gl_in[1].gl_Position.x - gl_in[1].gl_Position.z / gl_in[0].gl_Position.z * (TexCoord[0].y - d * gl_in[0].gl_Position.x)) / (gl_in[1].gl_Position.y - gl_in[1].gl_Position.z / gl_in[0].gl_Position.z * gl_in[0].gl_Position.y);
	float f = (TexCoord[0].y - e * gl_in[0].gl_Position.y - d * gl_in[0].gl_Position.x) / gl_in[0].gl_Position.z;
	mat3 textureMapper = mat3(a, d, 0.0f, b, e, 0.0f, c, f, 0.0f);
	*/
	Vertex clipVerticies[MAX_CLIP_VERTICIES];
	for (int i = 0; i < numClipVerticies; i++) {
		clipVerticies[i] = Vertex(vec4(clipRegion[i], 0.0f, 1.0f), -1, -1, false);
	}
	Vertex sourceVerticies[3];
	for (int i = 0; i < sourceVerticies.length(); i++) {
		sourceVerticies[i] = Vertex(gl_in[i].gl_Position, -1, -1, false);
	}
	// max number of intersections between a triangle and an N-sided shape is 2N
	Vertex intersections[2 * MAX_CLIP_VERTICIES];
	int numIntersections = 0;
	for (int i = 0; i < numClipVerticies; i++) {
		vec4 intersects[3];
		int numIntersects = 0;
		vec2 side[2];
		side[0] = clipRegion[i];
		side[1] = clipRegion[(i + 1) % numClipVerticies];
		for (int j = 0; j < 3; j++) {
			vec4 edge[2];
			edge[0] = gl_in[j].gl_Position;
			edge[1] = gl_in[(j + 1) % 3].gl_Position;
			vec4 intersection;
			if (intersectAt(edge, side, intersection)) {
				float dist = distance(intersection.xy / intersection.w, side[0]);
				int index;
				for (index = numIntersects; index > 0; index--) {
					float dis = distance(intersects[index - 1].xy / intersects[index - 1].w, side[0]);
					if (dist > dis) {
						break;
					}
					intersects[index] = intersects[index - 1];
				}
				intersects[index] = intersection;
				numIntersects++;
			}
		}
		if (numIntersects > 0) {
			bool exiting = sourceContains(intersects[0].xy);
			clipVerticies[i].next1 = numIntersections;
			for (int j = 0; j < numIntersects; j++) {
				intersections[numIntersections] = Vertex(intersects[j], -1, -1, exiting);
				numIntersections++;
				exiting = !exiting;
			}
			intersections[numIntersections - 1].next1 = (i + 1) % numClipVerticies;
		}
	}
	for (int i = 0; i < 3; i++) {
		vec4 intersects[MAX_CLIP_VERTICIES];
		int numIntersects = 0;
		vec4 edge[2];
		edge[0] = gl_in[i].gl_Position;
		edge[1] = gl_in[(i + 1) % 3].gl_Position;
		for (int j = 0; j < numClipVerticies; j++) {
			vec2 side[2];
			side[0] = clipRegion[j];
			side[1] = clipRegion[(j + 1) % numClipVerticies];
			vec4 intersection;
			if (intersectAt(edge, side, intersection)) {
				float dist = distance(intersection.xy / intersection.w, edge[0].xy / edge[0].w);
				int index;
				for (index = numIntersects; index > 0; index--) {
					float dis = distance(intersects[index - 1].xy / intersects[index - 1].w, edge[0].xy / edge[0].w);
					if (dist > dis) {
						break;
					}
					intersects[index] = intersects[index - 1];
				}
				intersects[index] = intersection;
				numIntersects++;
			}
		}
		if (numIntersects > 0) {
			int index = findIntersect(intersections, intersects[0]);
			sourceVerticies[i].next2 = index;
			for (int j = 1; j < numIntersects; j++) {
				int next = findIntersect(intersections, intersects[j]);
				intersections[index].next2 = 3 + next;
				index = next;
			}
			intersections[index].next2 = (i + 1) % 3;
		}
	}
	if (numIntersections > 0) {
		for (int i = 0; i < numIntersections; i++) {
			if (intersections[i].exiting) {
				gl_Position = intersections[i].vertex;
				color = vec4(1.0, 0.0, 0.0f, 1.0f);
				//texCoord = vec2(textureMapper * gl_Position.xyz);
				EmitVertex();
				bool tracer = false;
				bool intersect = true;
				int current = intersections[i].next2 - 3;
				if (current < 0) {
					current += 3;
					intersect = false;
				}
				while (!(intersect && current == i)) {
					if (intersect) {
						gl_Position = intersections[current].vertex;
						color = vec4(0.0, 1.0, 0.0f, 1.0f);
						//texCoord = vec2(textureMapper * gl_Position.xyz);
						EmitVertex();
						intersections[current].exiting = false;
						tracer = !tracer;
					} else if (tracer) {
						gl_Position = clipVerticies[current].vertex;
						color = vec4(0.0, 0.0, 1.0f, 1.0f);
						//texCoord = vec2(textureMapper * gl_Position.xyz);
						EmitVertex();
					} else {
						gl_Position = sourceVerticies[current].vertex;
						color = vec4(1.0, 1.0, 0.0f, 1.0f);
						//texCoord = vec2(textureMapper * gl_Position.xyz);
						EmitVertex();
					}
					if (tracer) {
						if (intersect) {
							if (intersections[current].next1 == -1) {
								current = (current + 1) % numIntersections;
							} else {
								current = intersections[current].next1;
								intersect = false;
							}
						} else {
							if (clipVerticies[current].next1 == -1) {
								current = (current + 1) % numClipVerticies;
							} else {
								current = clipVerticies[current].next1;
								intersect = true;
							}
						}
					} else {
						if (intersect) {
							current = intersections[current].next2 - 3;
							if (current < 0) {
								current += 3;
								intersect = false;
							}
						} else {
							if (sourceVerticies[current].next2 == -1) {
								current = (current + 1) % 3;
							} else {
								current = sourceVerticies[current].next2;
								intersect = true;
							}
						}
					}
				}
				EndPrimitive();
			}
		}
	} else {
		bool allInside = true;
		for (int i = 0; i < 3; i++) {
			if (!clipContains(gl_in[i].gl_Position.xy / gl_in[i].gl_Position.w)) {
				allInside = false;
				break;
			}
		}
		if (allInside) {
			for (int i = 0; i < 3; i++) {
				gl_Position = gl_in[i].gl_Position;
				color = vec4(1.0, 0.0, 1.0, 1.0);
				//texCoord = vec2(textureMapper * gl_Position.xyz);
				EmitVertex();
			}
			EndPrimitive();
		} else {
			allInside = true;
			for (int i = 0; i < numClipVerticies; i++) {
				if (!sourceContains(clipRegion[i])) {
					allInside = false;
					break;
				}
			}
			if (allInside) {
				for (int i = 0; i < numClipVerticies; i++) {
					gl_Position = vec4(clipRegion[i], 0.0f, 1.0f);
					color = vec4(0.0, 1.0, 1.0, 1.0);
					//texCoord = vec2(textureMapper * gl_Position.xyz);
					EmitVertex();
				}
				EndPrimitive();
			}
		}
	}
}  