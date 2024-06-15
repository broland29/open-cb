#pragma once

#include "MainWindow.h"
#include "TestWindow.h"
#include "../com/headers/Parameters.h"
#include <QApplication>
#include <iostream>


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