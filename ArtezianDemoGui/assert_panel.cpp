#include "assert_panel.hpp"

#include "variants_widget.hpp"

#include <env.h>

#include <json/value.h>
#include <json/reader.h>

#include <QMap>
#include <QLabel>
#include <QVBoxLayout>
#include <QByteArray>

namespace {
	VariantsWidget* createVariantWidget(const Json::Value& desc, std::shared_ptr<qclips::Env>& env)
	{
		auto titleStd = desc["title"].asString();

		auto title = QString::fromStdString(titleStd);

		auto result = new VariantsWidget(title, env);

		auto asserts = desc["asserts"];

		for (auto& assert : asserts)
		{
			auto text = QString::fromStdString(assert["text"].asString());
			auto assertText = QString::fromStdString(assert["assert"].asString());
			auto toolTip = QString::fromStdString(assert["tool_tip"].asString());

			result->addAssertVariant(text, assertText, toolTip);
		}

		auto toolTip = QString::fromStdString(desc["tool_tip"].asString());

		result->setToolTip(toolTip);

		return result;
	}
}

class AssertPanel::Impl
{
private:
	AssertPanel* _parent;

	std::shared_ptr<qclips::Env> _env;

	QVector<VariantsWidget*> _widgets;
	QVBoxLayout* _layout;

public:
	Impl(std::shared_ptr<qclips::Env>& env, AssertPanel* parent) 
		: _parent(parent),
		  _env(env)
	{
		initContent();
	}

	~Impl() { }

	void assert()
	{
		for (auto widget : _widgets)
		{
			widget->assert();
		}
	}

private:
	void initContent()
	{
		_layout = new QVBoxLayout();

		_layout->setAlignment(Qt::AlignTop);
		
		auto margins = _layout->contentsMargins();
		margins.setRight(0);
		_layout->setContentsMargins(margins);

		initAsserts();

		_parent->setLayout(_layout);
	}

	void initAsserts()
	{
		auto json = helpers::parseFile(u8"init_file.json");

		for (auto& var : json[u8"variants"])
		{
			auto widget = createVariantWidget(var, _env);

			_widgets << widget;

			_layout->addWidget(widget);

			connectVariant(widget);
		}
	}

	void connectVariant(VariantsWidget* variant)
	{
		QObject::connect(variant, &VariantsWidget::assertChanged, _parent, &AssertPanel::assertChanged);
	}

	void loadRequired()
	{
		_env->loadFromString(u8"(defglobal ?*result* = nothing)"
			"(deffunction set_result(?arg) (bind ?*result* ?arg))");
	}
};

AssertPanel::AssertPanel(std::shared_ptr<qclips::Env>& env, QWidget * parent)
	: QWidget(parent),
	  _impl(new Impl(env, this))
{ }

AssertPanel::~AssertPanel() { }

void AssertPanel::assert()
{
	_impl->assert();
}
