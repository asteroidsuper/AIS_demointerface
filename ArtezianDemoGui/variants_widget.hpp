#pragma once

#include <QFrame>

namespace qclips
{
	class Env;
}

class VariantsWidget : public QFrame
{
	Q_OBJECT

public:
	VariantsWidget(const QString& title, shared_ptr<qclips::Env>& env, QWidget * parent = nullptr);
	~VariantsWidget();

	void addAssertVariant(const QString& text, const QString& assert);

	void assert();

private:
	class Impl;
	unique_ptr<Impl> _impl;
};
