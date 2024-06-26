#pragma once

#include <QApplication>

#include "MainWindow.h"
#include "TestWindow.h"
#include "../com/headers/Parameters.h"


class UserApplication
{
public:
	QApplication* application;
	MainWindow* mainWindow;

private:

public:
	UserApplication(int argc, char* argv[]);
	
	int run();
	
	static int test(int argc, char* argv[]);
	
private:	
};