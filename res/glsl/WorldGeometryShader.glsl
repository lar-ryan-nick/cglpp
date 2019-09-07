#version 330 core
layout (triangles) in;
// max_verticies = (MAX_GL_IN - 1) * MAX_CLIP_VERTICIES
layout (triangle_strip, max_vertices = 100) out;

const int MAX_GL_IN = 3;
// must be the same as MAX_VERTICIES in cgl::View
const int MAX_CLIP_VERTICIES = 50;

struct Vertex {
	vec4 vertex;
	int next1;
	int next2;
	bool exiting;
};

in vec2 TexCoord[];

out vec2 texCoord;
out vec4 color;

uniform vec2 clipRegion[MAX_CLIP_VERTICIES];
uniform int numClipVerticies;
uniform mat4 vp;
uniform mat4 ivp;

// max number of intersections between a triangle and an N-sided shape is 2N
Vertex intersections[2 * MAX_CLIP_VERTICIES];
int numIntersections = 0;

// assume clipRegion is clockwise
bool sourceClockwise() {
	float tArea = 0.0f;
	for (int i = 0; i < gl_in.length(); i++) {
		vec2 current = gl_in[i].gl_Position.xy / gl_in[i].gl_Position.w;
		vec2 next = gl_in[(i + 1) % gl_in.length()].gl_Position.xy / gl_in[(i + 1) % gl_in.length()].gl_Position.w;
		tArea += (next.x - current.x) * (next.y + current.y);
	}
	return tArea > 0;
}

bool clipContains(vec2 position) {
	vec2 insidePoint = vec2(0.0f, 0.0f);
	for (int i = 0; i < numClipVerticies; i++) {
		insidePoint += clipRegion[i];
	}
	insidePoint /= numClipVerticies;
	for (int i = 0; i < numClipVerticies; i++) {
		vec3 diff = vec3(clipRegion[(i + 1) % numClipVerticies] - clipRegion[i], 0.0f);
		vec3 insideDistance = cross(vec3(insidePoint - clipRegion[i], 0.0f), diff);
		vec3 queryDistance = cross(vec3(position - clipRegion[i], 0.0f), diff);
		if (insideDistance.z * queryDistance.z < 0) {
			return false;
		}
	}
	return true;
}

bool sourceContains(vec2 position) {
	vec2 insidePoint = vec2(0.0f, 0.0f);
	for (int i = 0; i < gl_in.length(); i++) {
		insidePoint += gl_in[i].gl_Position.xy / gl_in[i].gl_Position.w;
	}
	insidePoint /= gl_in.length();
	for (int i = 0; i < gl_in.length(); i++) {
		vec2 normalized = gl_in[i].gl_Position.xy / gl_in[i].gl_Position.w;
		vec2 normalized2 = gl_in[(i + 1) % gl_in.length()].gl_Position.xy / gl_in[(i + 1) % gl_in.length()].gl_Position.w;
		vec3 diff = vec3(normalized2 - normalized, 0.0f);
		vec3 insideDistance = cross(vec3(insidePoint - normalized, 0.0f), diff);
		vec3 queryDistance = cross(vec3(position - normalized, 0.0f), diff);
		if (insideDistance.z * queryDistance.z < 0) {
			return false;
		}
	}
	return true;
}

bool intersectAt(vec4 line1[2], vec2 line2[2], out vec4 intersect) {
	vec3 normLine[2];
	normLine[0] = line1[0].xyz / line1[0].w;
	normLine[1] = line1[1].xyz / line1[1].w;
	if (normLine[1].x == normLine[0].x) {
		if (line2[1].x != line2[0].x) {
			if (min(line2[0].x, line2[1].x) <= normLine[0].x && normLine[0].x <= max(line2[0].x, line2[1].x)) {
				float change = (normLine[0].x - line2[0].x) / (line2[1].x - line2[0].x);
				float y = (line2[1].y - line2[0].y) * change + line2[0].y;
				if (min(normLine[0].y, normLine[1].y) <= y && y <= max(normLine[0].y, normLine[1].y)) {
					change = (y - normLine[0].y) / (normLine[1].y - normLine[0].y);
					float z = (normLine[1].z - normLine[0].z) * change + normLine[0].z;
					intersect = vec4(normLine[0].x, y, z, 1.0f);
					return true;
				}
			}
			return false;
		} else {
			// both lines could be vertical and overlap here may want to consider having a line segment to return
			return false;
		}
	}
	if (line2[1].x == line2[0].x) {
		if (min(normLine[0].x, normLine[1].x) <= line2[0].x && line2[0].x <= max(normLine[0].x, normLine[1].x)) {
			float change = (line2[0].x - normLine[0].x) / (normLine[1].x - normLine[0].x);
			float y = (normLine[1].y - normLine[0].y) * change + normLine[0].y;
			if (min(line2[0].y, line2[1].y) <= y && y <= max(line2[0].y, line2[1].y)) {
				float z = (normLine[1].z - normLine[0].z) * change + normLine[0].z;
				intersect = vec4(line2[0].x, y, z, 1.0f);
				return true;
			}
		}
		return false;
	}
	float denominator = line2[1].y - line2[0].y - (line2[1].x - line2[0].x) * (normLine[1].y - normLine[0].y) / (normLine[1].x - normLine[0].x);
	// Line segments have the same slope
	if (denominator == 0) {
		// both lines could overlap here may want to consider having a line segment to return
		return false;
	}
	float j = (normLine[0].y - line2[0].y + (normLine[1].y - normLine[0].y) * (line2[0].x - normLine[0].x) / (normLine[1].x - normLine[0].x)) / denominator;
	float i = (line2[0].x + (line2[1].x - line2[0].x) * j - normLine[0].x) / (normLine[1].x - normLine[0].x);
	if (i > 0 && i < 1 && j > 0 && j < 1) {
		intersect = vec4(normLine[0].x + (normLine[1].x - normLine[0].x) * i, normLine[0].y + (normLine[1].y - normLine[0].y) * i, normLine[0].z + (normLine[1].z - normLine[0].z) * i, 1.0f);
		return true;
	}
	return false;
}

int findIntersect(vec4 intersect) {
	for (int i = 0; i < numIntersections; i++) {
		if (intersect == intersections[i].vertex) {
			return i;
		}
	}
	// should never get to this line
	return -1;
}

void main() {
	color = vec4(0.0, 1.0, 0.0, 1.0);
	/* pass-through code
	for (int i = 0; i < gl_in.length(); i++) {
		gl_Position = gl_in[i].gl_Position;
		color = vec4(0.0, 0.0, 1.0, 1.0);
		texCoord = TexCoord[i];
		EmitVertex();
	}
	EndPrimitive();
	return;
	*/
	// check for preconditions (to prevent division by 0)
	for (int i = 0; i < gl_in.length(); i++) {
		if (gl_in[i].gl_Position.w == 0) {
			return;
		}
		if (abs(gl_in[i].gl_Position.z) > abs(gl_in[i].gl_Position.w)) {
			// TODO: change this point to its intersection with the z clipping plane
			return;
		}
	}
	vec3 points[3];
	for (int i = 0; i < gl_in.length(); i++) {
		points[i] = gl_in[i].gl_Position.xyz / gl_in[i].gl_Position.w;
	}
	if (points[0].z == 0) {
		return;
	}
	float denom = points[1].y - points[1].z / points[0].z * points[0].y;
	if (denom == 0) {
		return;
	}
	float mapperDenom = (points[2].x + points[2].y / denom * (-points[1].x + points[1].z / points[0].z * points[0].x) + points[2].z / points[0].z * (-points[0].y / denom * (-points[1].x + points[1].z / points[0].z * points[0].x)) - points[0].x);
	if (mapperDenom == 0) {
		return;
	}

	float a = (TexCoord[2].x - points[2].y / denom * (TexCoord[1].x - points[1].z / points[0].z * TexCoord[0].x) - points[2].z / points[0].z * (TexCoord[0].x - points[0].y / denom * (TexCoord[1].x - points[1].z / points[0].z * TexCoord[0].x))) / mapperDenom;
	float b = (TexCoord[1].x - a * points[1].x - points[1].z / points[0].z * (TexCoord[0].x - a * points[0].x)) / denom;
	float c = (TexCoord[0].x - b * points[0].y - a * points[0].x) / points[0].z;
	float d = (TexCoord[2].y - points[2].y / denom * (TexCoord[1].y - points[1].z / points[0].z * TexCoord[0].y) - points[2].z / points[0].z * (TexCoord[0].y - points[0].y / denom * (TexCoord[1].y - points[1].z / points[0].z * TexCoord[0].y))) / mapperDenom;
	float e = (TexCoord[1].y - d * points[1].x - points[1].z / points[0].z * (TexCoord[0].y - d * points[0].x)) / denom;
	float f = (TexCoord[0].y - e * points[0].y - d * points[0].x) / points[0].z;
	mat3 textureMapper = mat3(a, d, 0.0f, b, e, 0.0f, c, f, 0.0f);
	/* visual test
	for (int i = 0; i < gl_in.length(); i++) {
		vec4 temp = ivp * vec4(points[i], 1.0);
		if (abs((vp * (temp / temp.w)).z - gl_in[i].gl_Position.z) > 0.0001) {
			color = vec4(1.0, 0.0, 0.0, 1.0);
		} else {
			color = vec4(0.0, 1.0, 0.0, 1.0);
		}
		gl_Position = gl_in[i].gl_Position;
		texCoord = vec2(textureMapper * gl_Position.xyz / gl_Position.w);
		EmitVertex();
	}
	EndPrimitive();
	return;
	*/
	Vertex clipVerticies[MAX_CLIP_VERTICIES];
	bool clockwise = sourceClockwise();
	if (clockwise) {
		for (int i = 0; i < numClipVerticies; i++) {
			clipVerticies[i] = Vertex(vec4(clipRegion[i], 0.0f, 1.0f), -1, -1, false);
		}
	} else {
		for (int i = 0; i < numClipVerticies; i++) {
			clipVerticies[i] = Vertex(vec4(clipRegion[numClipVerticies - i - 1], 0.0f, 1.0f), -1, -1, false);
		}
	}
	Vertex sourceVerticies[MAX_GL_IN];
	for (int i = 0; i < MAX_GL_IN; i++) {
		sourceVerticies[i] = Vertex(gl_in[i].gl_Position, -1, -1, false);
	}
	for (int i = 0; i < numClipVerticies; i++) {
		vec4 intersects[MAX_GL_IN];
		int numIntersects = 0;
		vec2 side[2];
		side[0] = clipVerticies[i].vertex.xy;
		side[1] = clipVerticies[(i + 1) % numClipVerticies].vertex.xy;
		for (int j = 0; j < gl_in.length(); j++) {
			vec4 edge[2];
			edge[0] = gl_in[j].gl_Position;
			edge[1] = gl_in[(j + 1) % gl_in.length()].gl_Position;
			vec4 intersect;
			if (intersectAt(edge, side, intersect)) {
				float dist = distance(intersect.xy / intersect.w, side[0]);
				int index;
				for (index = numIntersects; index > 0; index--) {
					float dis = distance(intersects[index - 1].xy / intersects[index - 1].w, side[0]);
					if (dist > dis) {
						break;
					}
					intersects[index] = intersects[index - 1];
				}
				intersects[index] = intersect;
				numIntersects++;
			}
		}
		if (numIntersects > 0) {
			bool exiting = sourceContains(side[0]);
			clipVerticies[i].next1 = numIntersections;
			for (int j = 0; j < numIntersects; j++) {
				intersections[numIntersections] = Vertex(intersects[j], -1, -1, exiting);
				numIntersections++;
				exiting = !exiting;
			}
			intersections[numIntersections - 1].next1 = (i + 1) % numClipVerticies;
		}
	}
	for (int i = 0; i < gl_in.length(); i++) {
		vec4 intersects[MAX_CLIP_VERTICIES];
		int numIntersects = 0;
		vec4 edge[2];
		edge[0] = gl_in[i].gl_Position;
		edge[1] = gl_in[(i + 1) % gl_in.length()].gl_Position;
		for (int j = 0; j < numClipVerticies; j++) {
			vec2 side[2];
			side[0] = clipVerticies[j].vertex.xy;
			side[1] = clipVerticies[(j + 1) % numClipVerticies].vertex.xy;
			vec4 intersect;
			if (intersectAt(edge, side, intersect)) {
				float dist = distance(intersect.xy / intersect.w, points[i].xy);
				int index;
				for (index = numIntersects; index > 0; index--) {
					float dis = distance(intersects[index - 1].xy / intersects[index - 1].w, points[i].xy);
					if (dist > dis) {
						break;
					}
					intersects[index] = intersects[index - 1];
				}
				intersects[index] = intersect;
				numIntersects++;
			}
		}
		if (numIntersects > 0) {
			int index = findIntersect(intersects[0]);
			sourceVerticies[i].next2 = index;
			for (int j = 1; j < numIntersects; j++) {
				int next = findIntersect(intersects[j]);
				intersections[index].next2 = gl_in.length() + next;
				index = next;
			}
			intersections[index].next2 = (i + 1) % gl_in.length();
		}
	}
	if (numIntersections > 0) {
		for (int i = 0; i < numIntersections; i++) {
			if (intersections[i].exiting) {
				vec4 clipped[2 * MAX_CLIP_VERTICIES];
				int clippedLength = 0;
				clipped[clippedLength] = intersections[i].vertex;
				clippedLength++;
				bool traceClip = false;
				bool intersect = true;
				int current = intersections[i].next2 - gl_in.length();
				if (current < 0) {
					current += gl_in.length();
					intersect = false;
				}
				while (!(intersect && current == i)) {
					if (intersect) {
						clipped[clippedLength] = intersections[current].vertex;
						intersections[current].exiting = false;
						traceClip = !traceClip;
					} else if (traceClip) {
						clipped[clippedLength] = clipVerticies[current].vertex;
					} else {
						clipped[clippedLength] = sourceVerticies[current].vertex;
					}
					clippedLength++;
					if (traceClip) {
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
							current = intersections[current].next2 - gl_in.length();
							if (current < 0) {
								current += gl_in.length();
								intersect = false;
							}
						} else {
							if (sourceVerticies[current].next2 == -1) {
								current = (current + 1) % gl_in.length();
							} else {
								current = sourceVerticies[current].next2;
								intersect = true;
							}
						}
					}
				}
				vec3 normal = cross(points[2] - points[0], points[1] - points[0]);
				normal = normalize(normal);
				if (normal.y == 0) {
					// prevent division by zero
					return;
				}
				for (int j = 1; j < clippedLength - 1; j++) {
					vec4 verticies[3];
					if (clipped[0].w == 1.0f) {
						color = vec4(0.0, 0.0, 1.0, 1.0);
						clipped[0] = ivp * clipped[0];
						if (clipped[0].w == 0.0f) {
							color = vec4(1.0, 0.0, 0.0, 1.0);
							break;
						}
						clipped[0] = vp * (clipped[0] / clipped[0].w);
					}
					verticies[0] = clipped[0];
					if (clipped[j].w == 1.0f) {
						clipped[j] = ivp * clipped[j];
						if (clipped[j].w == 0.0f) {
							color = vec4(1.0, 0.0, 0.0, 1.0);
							break;
						}
						clipped[j] = vp * (clipped[j] / clipped[j].w);
					}
					verticies[1] = clipped[j];
					if (clipped[j + 1].w == 1.0f) {
						if (clipped[j + 1].z == 0.0f) {
							// set proper z val to clipRegion Vertex by making normal vector match gl_in
							vec3 clippedPoints[3];
							clippedPoints[0] = clipped[0].xyz / clipped[0].w;
							clippedPoints[1] = clipped[j].xyz / clipped[j].w;
							clippedPoints[2] = clipped[j + 1].xyz / clipped[j + 1].w;
							float normDenom = (-clippedPoints[1].y + clippedPoints[0].y - normal.x / normal.y * (clippedPoints[1].x - clippedPoints[0].x));
							if (normDenom == 0.0f) {
								// prevent division by zero
								return;
							}
							clipped[j + 1].z = (-clippedPoints[2].y * clippedPoints[1].z + clippedPoints[2].y * clippedPoints[0].z + clippedPoints[0].y * clippedPoints[1].z - clippedPoints[1].y * clippedPoints[0].z + normal.x / normal.y * (-clippedPoints[1].x * clippedPoints[0].z - clippedPoints[2].x * clippedPoints[1].z + clippedPoints[0].x * clippedPoints[1].z + clippedPoints[2].x * clippedPoints[0].z)) / normDenom;
							clippedPoints[2].z = clipped[j + 1].z;
							vec3 normTest = cross(clippedPoints[2] - clippedPoints[0], clippedPoints[1] - clippedPoints[0]);
							normTest = normalize(normTest);
							float xDiff = abs(normTest.x - normal.x);
							float yDiff = abs(normTest.y - normal.y);
							float zDiff = abs(normTest.z - normal.z);
							if (xDiff > .001f || yDiff > .001f || zDiff > .001f) {
								if (findIntersect(clipped[j] / clipped[j].w) == -1) {
									color = vec4(1.0, 1.0, 0.0, 1.0);
								} else {
									color = vec4(1.0, 0.0, 0.0, 1.0);
								}
								//break;
							}
						}
						clipped[j + 1] = ivp * clipped[j + 1];
						if (clipped[j + 1].w == 0) {
							color = vec4(1.0, 0.0, 0.0, 1.0);
							break;
						}
						clipped[j + 1] = vp * (clipped[j + 1] / clipped[j + 1].w);
					}
					verticies[2] = clipped[j + 1];
					for (int k = 0; k < verticies.length(); k++) {
						gl_Position = verticies[k];
						texCoord = vec2(textureMapper * gl_Position.xyz / gl_Position.w);
						EmitVertex();
					}
					EndPrimitive();
				}
			}
		}
	} else {
		bool allInside = true;
		for (int i = 0; i < gl_in.length(); i++) {
			if (!clipContains(points[i].xy)) {
				allInside = false;
				break;
			}
		}
		if (allInside) {
			for (int i = 0; i < gl_in.length(); i++) {
				gl_Position = gl_in[i].gl_Position;
				color = vec4(0.0, 0.0, 1.0, 1.0);
				texCoord = TexCoord[i];
				EmitVertex();
			}
			EndPrimitive();
		} else {
			/*
			allInside = true;
			for (int i = 0; i < numClipVerticies; i++) {
				if (!sourceContains(clipVerticies[i].vertex.xy)) {
					allInside = false;
					break;
				}
			}
			if (allInside) {
				for (int i = 1; i < numClipVerticies - 1; i++) {
					gl_Position = clipVerticies[0].vertex;
					texCoord = vec2(textureMapper * gl_Position.xyz);
					color = vec4(0.0, 1.0, 1.0, 1.0);
					EmitVertex();
					gl_Position = clipVerticies[i].vertex;
					texCoord = vec2(textureMapper * gl_Position.xyz);
					color = vec4(0.0, 1.0, 1.0, 1.0);
					EmitVertex();
					gl_Position = clipVerticies[i + 1].vertex;
					texCoord = vec2(textureMapper * gl_Position.xyz);
					color = vec4(0.0, 1.0, 1.0, 1.0);
					EmitVertex();
					EndPrimitive();
				}
			}
			*/
		}
	}
}  
