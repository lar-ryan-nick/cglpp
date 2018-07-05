#include "testWindow.h"

TestWindow::TestWindow() {
	setBackgroundColor(cgl::Color(0.5f, 0.5f, 0.5f, 1.0f));
	getView().setBackgroundColor(cgl::Color(1.0f, 0.0f, 0.0f, 1.0f));
	cgl::ScrollView* scrollView = new cgl::ScrollView(100.0f, 100.0f, 650.0f, 450.0f);
	scrollView->setBackgroundColor(cgl::Color(0.0f, 0.0f, 1.0f, 1.0f));
	scrollView->setContentSize(1000.0f, 1000.0f);
	scrollView->rotate(1.0f);
	cgl::ImageView *subview = new cgl::ImageView("res/img/container.jpg", 250.0f, 250.0f, 500.0f, 500.0f);
	scrollView->addSubview(subview);
	cgl::ScrollView* scrollSubview = new cgl::ScrollView(100.0f, 100.0f, 200.0f, 200.0f);
	scrollSubview->setBackgroundColor(cgl::Color(0.0f, 1.0f, 0.0f, 1.0f));
	scrollSubview->setContentSize(500.0f, 500.0f);
	cgl::View* subsubview = new cgl::View(100.0f, 100.0f, 300.0f, 300.0f);
	subsubview->setBackgroundColor(cgl::Color(1.0f, 1.0f, 1.0f, 1.0f));
	scrollSubview->addSubview(subsubview);
	scrollView->addSubview(scrollSubview);
	getView().addSubview(scrollView);
}
