#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>

int main()
{
	using namespace nana;

	//Define a form.
	form fm;

	//Popup msgbox when closing
	fm.events().unload(
			msgbox(fm, "Do you tanoshi?", msgbox::yes_no)<<"\\(^o^)/ Tanoshii~"
			);

	//Define a label and display a text.
	label lab{fm, "Welcome to the <bold size=14color=0x1fb575>J</><bold size=14color=0x93c317>A</><bold size=14color=0xf2970a>P</><bold size=14color=0x08a68f>A</><bold size=14color=0xd90b87>R</><bold size=14color=0x05a1de>I</> <bold size=14color=0x999b9a>P</><bold size=14color=0xf3950d>A</><bold size=14color=0xea0009>R</><bold size=14color=0xc7d50c>K</><bold size=14color=0x9e2c9d>!</>"};
	lab.format(true);

	//Define a button and answer the click event.
	button btn{fm, "Enter"};
	btn.events().click([&fm]{
			fm.close();
			});

	//Layout management
	fm.div("vert <><<><weight=80% text><>><><weight=24<><button><>><>");
	fm["text"]<<lab;
	fm["button"] << btn;
	fm.collocate();

	//Show the form
	fm.show();

	//Start to event loop process, it blocks until the form is closed.
	exec();
}
