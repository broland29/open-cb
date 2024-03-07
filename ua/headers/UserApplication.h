#pragma once

#include "MainWindow.h"
#include "TestWindow.h"
#include <QApplication>
#include <iostream>


class UserApplication
{
public:
	UserApplication(int argc, char* argv[]);
	int run();
	static int test(int argc, char* argv[]);
	
	MainWindow* mainWindow;
private:
	QApplication* application;
};