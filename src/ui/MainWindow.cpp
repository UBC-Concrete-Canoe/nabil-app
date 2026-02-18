#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
	setWindowTitle("Hull Designer");
	resize(800, 600);
}