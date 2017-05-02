#pragma once

#include <QtWidgets/QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};
