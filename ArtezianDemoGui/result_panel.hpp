#pragma once

#include <QWidget>

namespace qclips
{
	class Env;
}

class ResultPanel : public QWidget 
{
	Q_OBJECT
	Q_DISABLE_COPY(ResultPanel)
public:
	ResultPanel(QWidget * parent = nullptr);
	~ResultPanel();

	void refresh(const QString& symbol);

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};
