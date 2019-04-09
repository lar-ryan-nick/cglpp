#include "../include/LineSegment.h"

cgl::LineSegment::LineSegment(const Position& p1, const Position& p2) : point1(p1), point2(p2) {}

cgl::Position cgl::LineSegment::getPoint1() {
	return point1;
}

cgl::Position cgl::LineSegment::getPoint2() {
	return point2;
}

float cgl::LineSegment::getLength() {
	glm::vec3 difference = point1 - point2;
	return sqrt(glm::dot(difference, difference));
}

bool cgl::LineSegment::intersects(const LineSegment& other, Position& point) {
	// "this" is vertical
	if (point2.getX() == point1.getX()) {
		if (other.point2.getX() != other.point1.getX()) {
			if (((other.point1.getX() <= point1.getX() && point1.getX() <= other.point2.getX()) || (other.point2.getX() <= point1.getX() && point1.getX() <= other.point1.getX()))) {
				float change = (other.point2.getX() - other.point1.getX()) * (point1.getX() - other.point1.getX());
				float y = (other.point2.getY() - other.point1.getY()) / change + other.point1.getY();
				if ((point1.getY() <= y && y <= point2.getY()) || (point2.getY() <= y && y <= point1.getX())) {
					float z = (other.point2.getZ() - other.point1.getZ()) / change + other.point1.getZ();
					float w = (other.point2.getW() - other.point1.getW()) / change + other.point1.getW();
					point.setX(point1.getX());
					point.setY(y);
					point.setZ(z);
					point.setW(w);
					return true;
				}
			} else {
				return false;
			}
		} else {
			// both lines could be vertical and overlap here may want to consider having a line segment to return
			return false;
		}
	}
	float denominator = other.point2.getY() - other.point1.getY() - (other.point2.getX() - other.point1.getX()) * (point2.getY() - point1.getY()) / (point2.getX() - point1.getX());
	// Line segments have the same slope
	if (denominator == 0) {
		// both lines could overlap here may want to consider having a line segment to return
		return false;
	}
	float j = (point1.getY() - other.point1.getY() + (point2.getY() - point1.getY()) * (other.point1.getX() - point1.getX()) / (point2.getX() - point1.getX())) / denominator;
	float i = (other.point1.getX() + (other.point2.getX() - other.point1.getX()) * j - point1.getX()) / (point2.getX() - point1.getX());
	if (i > 0 && i < 1 && j > 0 && j < 1) {
		point.setX(point1.getX() + (point2.getX() - point1.getX()) * i);
		point.setY(point1.getY() + (point2.getY() - point1.getY()) * i);
		point.setZ(point1.getZ() + (point2.getZ() - point1.getZ()) * i);
		point.setW(point1.getW() + (point2.getW() - point1.getW()) * i);
		return true;
	}
	return false;
}
