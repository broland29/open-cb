#include "../headers/UserApplication.h"


UserApplication::UserApplication(int argc, char* argv[])
{
	application = new QApplication(argc, argv);

	mainWindow = new MainWindow();
}


int UserApplication::run()
{
	mainWindow->show();
	return application->exec();
}


int UserApplication::test(int argc, char* argv[])
{
	std::cout << "Hello from UserApplication!" << std::endl;

	QApplication app(argc, argv);

	TestWindow* window = new TestWindow();
	window->show();

	return app.exec();
}