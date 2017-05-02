#include "main_window.h"

#include "main_widget.hpp"

class MainWindow::Impl
{
private:
	MainWindow* _parent;

public:
	Impl(MainWindow* parent) : _parent(parent)
	{
		_parent->setCentralWidget(new MainWidget());
	}

	~Impl() { }
};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  _impl(new Impl(this))
{
}

MainWindow::~MainWindow() { }