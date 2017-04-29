#include "clips_demo_widget.hpp"

#include "variants_widget.hpp"
#include "artesian_result_view.hpp"

#include <data_object.h>
#include <env.h>

#include <json/value.h>
#include <json/reader.h>

#include <QMap>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QByteArray>
#include <QFile>

#include <QMessageBox>

#include "tank_widget.hpp"

namespace {
	Json::Value parseFile(const QString& fileName)
	{
		QFile file(fileName);

		if (file.open(QIODevice::ReadOnly) == false)
		{
			throw std::runtime_error("Json can not be opened");
		}

		auto data = file.readAll();

		file.close();

		Json::Value result;

		Json::Reader reader;

		const int BOMSize = 3;

		if (reader.parse(data.data() + BOMSize, data.data() + data.size(), result) == false)
		{
			throw runtime_error("parsing error has occured");
		}

		return result;
	}

	VariantsWidget* createVariantWidget(const Json::Value& desc, shared_ptr<qclips::Env>& env)
	{
		auto titleStd = desc["title"].asString();

		auto title = QString::fromStdString(titleStd);

		auto result = new VariantsWidget(title, env);

		auto asserts = desc["asserts"];

		for (auto& assert : asserts)
		{
			auto text = QString::fromStdString(assert["text"].asString());
			auto assertText = QString::fromStdString(assert["assert"].asString());

			result->addAssertVariant(text, assertText);
		}

		return result;
	}
}

class ClipsDemoWidget::Impl
{
private:
	ClipsDemoWidget* _parent;

	shared_ptr<qclips::Env> _env;

	QVector<VariantsWidget*> _widgets;
	QVBoxLayout* _layout;

	QMap<QString, QString> _translations;
	QMap<QString, int> _debitTrans;
	QString _translationUnknowText;

public:
	Impl(ClipsDemoWidget* parent) 
		: _parent(parent),
		  _env(new qclips::Env)
	{
		initLayout();
		initTitle();
		loadRequired();
		loadExpertCode();
		loadDebitTrans();
		parseInitFile();
		initShowAnswerButton();
	}

	~Impl() { }

private:
	void initLayout()
	{
		_layout = new QVBoxLayout();

		_layout->setAlignment(Qt::AlignTop);

		_parent->setLayout(_layout);
	}

	void initTitle()
	{
		auto title = new QLabel(u8"Демо-версия интерфейса");

		auto font = title->font();

		font.setPointSize(16);

		title->setFont(font);

		_layout->addWidget(title, 0, Qt::AlignHCenter);
	}

	void loadRequired()
	{
		_env->loadFromString(u8"(defglobal ?*result* = nothing)"
			"(deffunction set_result(?arg) (bind ?*result* ?arg))");
	}

	void loadDebitTrans()
	{
		auto debitTrans = parseFile(u8"debit_trans.json");

		for (auto& d : debitTrans)
		{
			auto symbol = QString::fromStdString(d["symbol"].asString());

			_debitTrans[symbol] = d["debit"].asInt();
		}
	}

	void parseInitFile()
	{
		auto json = parseFile(u8"init_file.json");

		for (auto& var : json[u8"variants"])
		{
			auto widget = createVariantWidget(var, _env);

			_widgets << widget;

			_layout->addWidget(widget);
		}

		for (auto& var : json["translations"])
		{
			auto symbol = QString::fromStdString(var["symbol"].asString());
			auto description = QString::fromStdString(var["description"].asString());

			_translations[symbol] = description;
		}

		_translationUnknowText = QString::fromStdString(json["error_message"].asString());
	}

	void loadExpertCode()
	{
		if (_env->load(u8"expert_code.clp") == false)
		{
			throw std::runtime_error("Expert code has not loaded");
		}
	}

	void initShowAnswerButton()
	{
		auto pushButton = new QPushButton(u8"Показать решение системы");

		QObject::connect(pushButton, &QPushButton::clicked, [this]() 
		{
			showSystemAnswer();
		});

		_layout->addWidget(new TankWidget());

		_layout->addWidget(pushButton, 0, Qt::AlignHCenter);
	}

	QString translate(const QString& symbol) const
	{
		if (_translations.contains(symbol) == false)
		{
			return _translationUnknowText;
		}

		return _translations[symbol];
	}

	void showSystemAnswer()
	{
		_env->reset();

		_env->eval(u8"(set_result nothing)");

		for (auto widget : _widgets)
		{
			widget->assert();
		}

		_env->run(-1);

		auto answer = _env->eval(u8"?*result*").string();

		ArtesianResultView view(_debitTrans[answer]);

		view.setWindowTitle(translate(answer));

		view.exec();
	}
};

ClipsDemoWidget::ClipsDemoWidget(QWidget * parent)
	: QWidget(parent),
	  _impl(new Impl(this))
{ }

ClipsDemoWidget::~ClipsDemoWidget() { }
