#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <View.h>
#include <ImageView.h>
#include <ScrollView.h>
#include <Window.h>
#include <Font.h>
#include <TextView.h>

class TestWindow : public cgl::Window {
	private:
		cgl::ScrollView* scrollView;
		cgl::ImageView *subview;
		cgl::ScrollView* scrollSubview;
		cgl::View* subsubview;
		cgl::TextView* textView;
	public:
		TestWindow();
		virtual ~TestWindow();
		virtual void render();
};

#endif
