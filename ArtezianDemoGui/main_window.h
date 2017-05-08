#pragma once

#include <QtWidgets/QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT
	Q_DISABLE_COPY(MainWindow)
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};
