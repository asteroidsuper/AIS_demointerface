#include "result_panel.hpp"

#include "artesian_well_counter.h"
#include "external_files.h"

#include <json/value.h>

#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMap>

namespace {
	QHBoxLayout* hbox(QWidget* widget)
	{
		auto res = new QHBoxLayout();

		res->setAlignment(Qt::AlignLeft);

		res->addWidget(widget);

		return res;
	}

	void setFontPointSize(QLabel* label, int size)
	{
		auto font = label->font();

		font.setPointSize(size);

		label->setFont(font);
	}
}

class ResultPanel::Impl
{
private:
	ResultPanel* _parent;

	QMap<QString, QString> _translations;
	QMap<QString, int> _debitTrans;
	QString _translationUnknowText;

	QLabel* _decisionTitle;
	QLabel* _debitTitle;

	QLabel* _wellsLabel;

public:
	Impl(ResultPanel* parent) 
		: _parent(parent)
	{
		loadTranslation();
		loadDebitsTrans();
		createDecisionTitle();
		createDebitTitle();
		createWellsLabel();

		initLayout();
	}

	~Impl() { }

	void refresh(const QString& symbol)
	{
		_decisionTitle->setText(translate(symbol));
		_debitTitle->setText(u8"Система потребовала: " + QString::number(_debitTrans[symbol]));

		ArtesianWellCounter counter;

		auto wells = counter.calcToDebit(_debitTrans[symbol]);

		if (wells.isEmpty())
		{
			_wellsLabel->setText(u8"Имеющийся в наличии скважины не способны удовлетворить потребление");
			return;
		}

		QString debits = u8"";

		for (auto& well : wells)
		{
			if (well.count == 0)
			{
				continue;
			}

			debits += u8"С дебитом: " + QString::number(well.debit) + u8", требуется: " + QString::number(well.count);
			debits += u8"\n\n";
		}

		_wellsLabel->setText(debits);
	}

private:
	void createDecisionTitle()
	{
		_decisionTitle = new QLabel(u8"Тестовая строка");
		setFontPointSize(_decisionTitle, 12);
	}

	void createDebitTitle()
	{
		_debitTitle = new QLabel(u8"Тестовая строка");
		setFontPointSize(_debitTitle, 12);
	}

	void createWellsLabel()
	{
		_wellsLabel = new QLabel(u8"Для отображения результата");
		setFontPointSize(_wellsLabel, 12);

		_wellsLabel->setWordWrap(true);
	}

	void initLayout()
	{
		auto mainLayout = new QVBoxLayout();

		mainLayout->setSpacing(0);
		
		auto topLayout = new QVBoxLayout();
		topLayout->setSpacing(0);
		topLayout->setAlignment(Qt::AlignTop);

		auto decisionFrame = createFrame(hbox(_decisionTitle));

		topLayout->addWidget(decisionFrame, 0, Qt::AlignTop);

		auto debitFrame = createFrame(hbox(_debitTitle));

		topLayout->addWidget(debitFrame, 0, Qt::AlignTop);

		mainLayout->addLayout(topLayout);

		auto wellsLayout = new QHBoxLayout();

		wellsLayout->addWidget(_wellsLabel);

		mainLayout->addWidget(createFrame(wellsLayout),1);

		_parent->setLayout(mainLayout);
	}

	static QFrame* createFrame(QLayout* layout)
	{
		QFrame* res = new QFrame();

		res->setFrameShape(QFrame::Box);
		res->setFrameShadow(QFrame::Sunken);
		res->setMidLineWidth(1);

		res->setLayout(layout);

		return res;
	}

	void loadDebitsTrans()
	{
		auto json = helpers::parseFile(external_files::debitTranslations());

		for (auto& d : json)
		{
			auto symbol = QString::fromStdString(d["symbol"].asString());

			_debitTrans[symbol] = d["debit"].asInt();
		}
	}

	void loadTranslation()
	{
		auto json = helpers::parseFile(external_files::guiInitFilePath());

		for (auto& var : json["translations"])
		{
			auto symbol = QString::fromStdString(var["symbol"].asString());
			auto description = QString::fromStdString(var["description"].asString());

			_translations[symbol] = description;
		}

		_translationUnknowText = QString::fromStdString(json["error_message"].asString());
	}

	QString translate(const QString& symbol) const
	{
		if (_translations.contains(symbol) == false)
		{
			return _translationUnknowText;
		}

		return _translations[symbol];
	}
};

ResultPanel::ResultPanel(QWidget * parent)
	: QWidget(parent),
	  _impl(new Impl(this))
{ }

ResultPanel::~ResultPanel() { }

void ResultPanel::refresh(const QString& symbol)
{
	_impl->refresh(symbol);
}
