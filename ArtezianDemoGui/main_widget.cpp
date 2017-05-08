#include "main_widget.hpp"

#include "assert_panel.hpp"
#include "result_panel.hpp"

#include "external_files.h"

#include <env.h>
#include <data_object.h>

#include <QHBoxLayout>
#include <QVBoxLayout>

class MainWidget::Impl : public QObject
{
private:
	MainWidget * _parent;

	AssertPanel* _assertPanel;
	ResultPanel* _resultPanel;

	std::shared_ptr<qclips::Env> _env;

public :
	Impl(MainWidget* parent)
		: _parent(parent),
		  _env(new qclips::Env())
	{
		_assertPanel = new AssertPanel(_env);
		_resultPanel = new ResultPanel();
		createLayout();

		_env->loadFromString(u8"(defglobal ?*result* = nothing)"
			"(deffunction set_result(?arg) (bind ?*result* ?arg))");

		_env->load(external_files::clipsCodeFilePath());

		connect(_assertPanel, &AssertPanel::assertChanged, this, &Impl::refresh);

		refresh();
	}

	~Impl() { }

private:
	void createLayout()
	{
		auto mainLayout = new QHBoxLayout();

		mainLayout->setSpacing(0);

		mainLayout->addWidget(_assertPanel, 0, Qt::AlignLeft);
		mainLayout->addWidget(_resultPanel, 1);

		_parent->setLayout(mainLayout);
	}

	Q_SLOT void refresh()
	{
		_env->reset();

		_env->eval(u8"(set_result nothing)");

		_assertPanel->assert();

		_env->run(-1);

		auto res = _env->eval(u8"?*result*");

		_resultPanel->refresh(res.string());
	}
};

MainWidget::MainWidget(QWidget * parent)
	: QWidget(parent),
	 _impl(new Impl(this))
{ }

MainWidget::~MainWidget() { }
