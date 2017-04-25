#pragma once

#include <QWidget>

class ClipsDemoWidget : public QWidget 
{
	Q_OBJECT

public:
	ClipsDemoWidget(QWidget * parent = nullptr);
	~ClipsDemoWidget();

private:
	class Impl;
	unique_ptr<Impl> _impl;
};
