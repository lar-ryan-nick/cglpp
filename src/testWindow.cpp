#include "testWindow.h"

TestWindow::TestWindow() {
	setBackgroundColor(Color(0.5f, 0.5f, 0.5f, 1.0f));
	getView().setBackgroundColor(Color(1.0f, 0.0f, 0.0f, 1.0f));
	ScrollView* scrollView = new ScrollView(100, 100, 650, 450);
	scrollView->setBackgroundColor(Color(0.0f, 0.0f, 1.0f, 1.0f));
	scrollView->setContentSize(1000, 1000);
	scrollView->rotate(1);
	ImageView *subview = new ImageView("res/img/container.jpg", 250, 250, 500, 500);
	//subview->setBackgroundColor(Color(0.0f, 1.0f, 0.0f, 1.0f));
	scrollView->addSubview(subview);
	ScrollView* scrollSubview = new ScrollView(100, 100, 200, 200);
	scrollSubview->setBackgroundColor(Color(0.0f, 1.0f, 0.0f, 1.0f));
	scrollSubview->setContentSize(500, 500);
	View* subsubview = new View(100, 100, 300, 300);
	subsubview->setBackgroundColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	scrollSubview->addSubview(subsubview);
	scrollView->addSubview(scrollSubview);
	getView().addSubview(scrollView);
}
