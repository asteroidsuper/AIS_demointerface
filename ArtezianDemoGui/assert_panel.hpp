#pragma once
#include <QScrollArea>

namespace qclips
{
	class Env;
}

class AssertPanel : public QWidget
{
	Q_OBJECT

public:
	AssertPanel(std::shared_ptr<qclips::Env>& env, QWidget * parent = nullptr);
	~AssertPanel();

	void assert();

	Q_SIGNAL void assertChanged();

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};
