#include "variants_widget.hpp"

#include <data_object.h>
#include <env.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QPainter>

namespace {
	struct AssertVariant
	{
		QRadioButton* button;
		QString assertString;
	};
}

class VariantsWidget::Impl : public QObject
{
private:
	VariantsWidget* _parent;
	std::shared_ptr<qclips::Env> _env;

	QVector<AssertVariant> _assertVariants;

	QVBoxLayout* _layout;

public:
	Impl(VariantsWidget* parent, std::shared_ptr<qclips::Env>& env, const QString& title) 
		: _parent(parent),
		  _env(env)
	{
		initLayout();
		initTitle(title);

		_parent->setFrameShape(QFrame::Box);
		_parent->setFrameShadow(QFrame::Sunken);
		_parent->setMidLineWidth(1);

		auto palette = _parent->palette();

		auto backgroundColor = palette.color(QPalette::Background);

		qreal hue, sat, lum;

		backgroundColor.getHslF(&hue, &sat, &lum);

		sat += 0.15;

		backgroundColor.setHslF(hue, sat, lum);

		palette.setColor(QPalette::Background, backgroundColor);

		_parent->setPalette(palette);

		_parent->setAutoFillBackground(true);
	}

	~Impl() { }

	void addAssertVariant(const QString& text, const QString& assert, const QString& toolTip)
	{
		AssertVariant var;

		var.assertString = assert;

		var.button = new QRadioButton(text);

		auto font = var.button->font();

		font.setPointSize(10);

		var.button->setFont(font);

		if (_assertVariants.isEmpty())
		{
			var.button->setChecked(true);
		}

		if (toolTip != nullptr)
		{
			var.button->setToolTip(toolTip);
		}

		_assertVariants << var;

		_layout->addWidget(var.button);

		connectRadio(var.button);
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

	void paintEvent()
	{
		QPainterPath path;
		{
			QRect frame = _parent->rect();

			frame.setTopLeft(QPoint());

			path.addRoundRect(frame, 15, 30);
		}
		
		QPainter painter(_parent);

		painter.fillPath(path, Qt::white);
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

	void connectRadio(QRadioButton* button)
	{
		connect(button, &QRadioButton::toggled, this, &Impl::assertChanged);
	}

	Q_SLOT void assertChanged()
	{
		emit _parent->assertChanged();
	}

};

VariantsWidget::VariantsWidget(const QString& title, std::shared_ptr<qclips::Env>& env, QWidget * parent)
	: QFrame(parent),
	  _impl(new Impl(this, env, title))
{ }

VariantsWidget::~VariantsWidget() { }

void VariantsWidget::addAssertVariant(const QString & text, const QString & assert, const QString& toolTip)
{
	_impl->addAssertVariant(text, assert, toolTip);
}

void VariantsWidget::assert()
{
	_impl->assert();
}

void VariantsWidget::paintEvent(QPaintEvent *event)
{
	QFrame::paintEvent(event);
}
