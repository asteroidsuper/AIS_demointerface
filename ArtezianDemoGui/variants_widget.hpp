#pragma once

#include <QFrame>

namespace qclips
{
	class Env;
}

class VariantsWidget : public QFrame
{
	Q_OBJECT
	Q_DISABLE_COPY(VariantsWidget)
public:
	VariantsWidget(const QString& title, std::shared_ptr<qclips::Env>& env, QWidget * parent = nullptr);
	~VariantsWidget();

	void addAssertVariant(const QString& text, const QString& assert, const QString& toolTip);

	void assert();

	Q_SIGNAL void assertChanged();

protected:
	virtual void paintEvent(QPaintEvent *event) override;

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};
