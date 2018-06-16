#include "scrollView.h"

ScrollView::ScrollView(float x, float y, float width, float height) : View(x, y, width, height), contentSize(width, height) {
	setIsScrollable(true);
	setClipSubviews(true);
}

void ScrollView::setContentSize(float width, float height) {
	Rectangle bounds = getBounds();
	if (width < bounds.getWidth()) {
		width = bounds.getWidth();
	}
	if (height < bounds.getHeight()) {
		height = bounds.getHeight();
	}
	contentSize.setWidth(width);
	contentSize.setHeight(height);
}

bool ScrollView::onScroll(double xOffset, double yOffset) {
	Position offsetPosition = getOffsetPosition();
	offsetPosition.move(xOffset, yOffset);
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
