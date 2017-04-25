#include "main_window.h"

#include "clips_demo_widget.hpp"

class MainWindow::Impl
{
private:
	MainWindow* _parent;

public:
	Impl(MainWindow* parent) : _parent(parent)
	{
		_parent->setCentralWidget(new ClipsDemoWidget());
	}

	~Impl() { }
};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  _impl(new Impl(this))
{
}

MainWindow::~MainWindow() { }