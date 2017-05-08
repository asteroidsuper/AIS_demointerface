#pragma once
#include <QWidget>

class MainWidget : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(MainWidget)
public:
	MainWidget(QWidget * parent = nullptr);
	~MainWidget();

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};
