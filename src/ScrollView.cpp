#include "../include/ScrollView.h"

cgl::ScrollView::ScrollView(float x, float y, float width, float height) : View(x, y, width, height), contentSize(width, height) {
	setIsScrollable(true);
	setClipSubviews(true);
}

void cgl::ScrollView::setContentSize(float width, float height) {
	setContentSize(Size(width, height));
}

void cgl::ScrollView::setContentSize(const Size& s) {
	contentSize = s;
	fixContentSize();
}

void cgl::ScrollView::fixContentSize() {
	Rectangle bounds = getBounds();
	if (contentSize.getWidth() < bounds.getWidth()) {
		contentSize.setWidth(bounds.getWidth());
	}
	if (contentSize.getHeight() < bounds.getHeight()) {
		contentSize.setHeight(bounds.getHeight());
	}
}

bool cgl::ScrollView::onScroll(double xOffset, double yOffset) {
	Position offsetPosition = getOffsetPosition();
	offsetPosition.translate(xOffset, yOffset);
	Rectangle bounds = getBounds();
	if (offsetPosition.getX() > contentSize.getWidth() - bounds.getWidth()) {
		offsetPosition.setX(contentSize.getWidth() - bounds.getWidth());
	} else if (offsetPosition.getX() < 0) {
		offsetPosition.setX(0);
	}
	if (offsetPosition.getY() > contentSize.getHeight() - bounds.getHeight()) {
		offsetPosition.setY(contentSize.getHeight() - bounds.getHeight());
	} else if (offsetPosition.getY() < 0) {
		offsetPosition.setY(0);
	}
	setOffsetPosition(offsetPosition);
	return true;
}
