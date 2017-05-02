#pragma once
#include <QWidget>

class MainWidget : public QWidget {
	Q_OBJECT

public:
	MainWidget(QWidget * parent = nullptr);
	~MainWidget();

	NO_COPYABLE(MainWidget)

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};
