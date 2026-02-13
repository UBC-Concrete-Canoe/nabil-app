#include <QApplication>
#include "app/Application.h"
#include "ui/MainWindow.h"

int
main(int argc, char* argv[])
{

	QApplication app(argc, argv);
	Application application;
	application.run();
	return app.exec();
}
