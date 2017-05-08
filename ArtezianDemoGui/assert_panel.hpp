#pragma once
#include <QScrollArea>

namespace qclips
{
	class Env;
}

class AssertPanel : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(AssertPanel)
public:
	AssertPanel(std::shared_ptr<qclips::Env>& env, QWidget * parent = nullptr);
	~AssertPanel();

	void assert();

	Q_SIGNAL void assertChanged();

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};
