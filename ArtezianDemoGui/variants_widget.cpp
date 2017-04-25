#include "variants_widget.hpp"

#include <data_object.h>
#include <env.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QRadioButton>

namespace {
	struct AssertVariant
	{
		QRadioButton* button;
		QString assertString;
	};
}

class VariantsWidget::Impl
{
private:
	VariantsWidget* _parent;
	shared_ptr<qclips::Env> _env;

	QVector<AssertVariant> _assertVariants;

	QVBoxLayout* _layout;
public:
	Impl(VariantsWidget* parent, shared_ptr<qclips::Env>& env, const QString& title) 
		: _parent(parent),
		  _env(env)
	{
		initLayout();
		initTitle(title);
	}

	~Impl() { }

	void addAssertVariant(const QString& text, const QString& assert)
	{
		AssertVariant var;

		var.assertString = assert;

		var.button = new QRadioButton(text);

		if (_assertVariants.isEmpty())
		{
			var.button->setChecked(true);
		}

		_assertVariants << var;

		_layout->addWidget(var.button);
	}

	void assert()
	{
		for (auto& var : _assertVariants)
		{
			if (var.button->isChecked())
			{
				_env->eval("(assert " + var.assertString + ")");

				return;
			}
		}
	}

private:
	void initLayout()
	{
		_layout = new QVBoxLayout();

		_layout->setAlignment(Qt::AlignTop);

		_parent->setLayout(_layout);
	}

	void initTitle(const QString& title)
	{
		auto titleLabel = new QLabel(title);

		auto font = titleLabel->font();

		font.setPointSize(12);

		titleLabel->setFont(font);

		_layout->addWidget(titleLabel, 0, Qt::AlignHCenter);
	}

};

VariantsWidget::VariantsWidget(const QString& title, shared_ptr<qclips::Env>& env, QWidget * parent)
	: QFrame(parent),
	  _impl(new Impl(this, env, title))
{ }

VariantsWidget::~VariantsWidget() { }

void VariantsWidget::addAssertVariant(const QString & text, const QString & assert)
{
	_impl->addAssertVariant(text, assert);
}

void VariantsWidget::assert()
{
	_impl->assert();
}
