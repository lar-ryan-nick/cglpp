#include "TestWindow.h"

TestWindow::TestWindow() {
	setBackgroundColor(cgl::Color(0.5f, 0.5f, 0.5f, 1.0f));
	getView().setBackgroundColor(cgl::Color(1.0f, 0.0f, 0.0f, 1.0f));
	scrollView = new cgl::ScrollView(100.0f, 100.0f, 650.0f, 450.0f);
	scrollView->setBackgroundColor(cgl::Color(0.0f, 0.0f, 1.0f, 1.0f));
	scrollView->setContentSize(1000.0f, 1000.0f);
	scrollView->rotate(1.0f);
	subview = new cgl::ImageView("res/img/container.jpg", 250.0f, 250.0f, 500.0f, 500.0f);
	scrollView->addSubview(subview);
	scrollSubview = new cgl::ScrollView(100.0f, 100.0f, 200.0f, 200.0f);
	scrollSubview->setBackgroundColor(cgl::Color(0.0f, 1.0f, 0.0f, 1.0f));
	scrollSubview->setContentSize(500.0f, 500.0f);
	subsubview = new cgl::View(100.0f, 100.0f, 300.0f, 300.0f);
	subsubview->setBackgroundColor(cgl::Color(1.0f, 1.0f, 1.0f, 1.0f));
	scrollSubview->addSubview(subsubview);
	scrollView->addSubview(scrollSubview);
	getView().addSubview(scrollView);
	textView = new cgl::TextView("But I am a dumb god", 400.0f, 250.0f, 450.0f, 50.0f);
	textView->setBackgroundColor(cgl::Color(1.0f, 1.0f, 1.0f, 0.5f));
	getView().addSubview(textView);
}

TestWindow::~TestWindow() {
	delete scrollView;
	delete subview;
	delete scrollSubview;
	delete subsubview;
	delete textView;
}

void TestWindow::render() {
	subview->setRotation(static_cast<float>(glfwGetTime()));
	textView->setRotation(static_cast<float>(glfwGetTime()));
	Window::render();
}
