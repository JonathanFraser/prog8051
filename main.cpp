#include <QApplication>

#include "prog8051.h"

int main(int argc, char* argv[])
{
	QApplication app(argc,argv);
	prog8051 win;
	win.show();
	
	return app.exec();
	
}
