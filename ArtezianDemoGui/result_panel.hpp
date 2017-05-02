#pragma once
#include <QWidget>

namespace qclips
{
	class Env;
}

class ResultPanel : public QWidget {
	Q_OBJECT

public:
	ResultPanel(QWidget * parent = nullptr);
	~ResultPanel();

	void refresh(const QString& symbol);

PIMPL(ResultPanel)
};
