#include "artesian_result_view.hpp"

#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>

#include "artesian_well_counter.h"

namespace{
	QLabel* setFont_(QLabel* label, int fontSize)
	{
		auto font = label->font();

		font.setPointSize(fontSize);

		label->setFont(font);

		return label;
	}
}

class ArtesianResultView::Impl
{
private:
	ArtesianResultView* _parent;

	QVBoxLayout* _layout;

public:
	Impl(ArtesianResultView* parent, unsigned long long debit) : _parent(parent)
	{
		initLayout(debit);
	}

private:
	void initLayout(unsigned long long debit)
	{
		auto title = new QLabel(u8"Требуемое количество воды: " + QString::number(debit));

		_layout = new QVBoxLayout();

		_layout->setAlignment(Qt::AlignTop);

		_layout->addWidget(setFont_(title, 16), 0, Qt::AlignHCenter);

		ArtesianWellCounter counter;

		auto wells = counter.calcToDebit(debit);

		for (auto& well : wells)
		{
			_layout->addWidget(createFor(well.count, well.debit));
		}

		if (wells.isEmpty())
		{
			_layout->addWidget(setFont_(new QLabel(
				u8"Скважины в наличии не могут покрыть требуемое потребление!"), 18));
		}
		else
		{
			_layout->addWidget(setFont_(new QLabel(
				u8"Полученный дебит: " + QString::number(counter.debitFor(wells)))
				, 14));
		}

		_parent->setLayout(_layout);
	}

	QFrame* createFor(int count, int debit) const
	{
		auto result = new QFrame();

		auto layout = new QVBoxLayout();

		layout->setAlignment(Qt::AlignTop);

		layout->addWidget(setFont_(new QLabel(u8"Дебит скважины: " + QString::number(debit)), 14));

		layout->addWidget(setFont_(new QLabel(u8"Зайдействованное количество: " + QString::number(count)), 14));

		result->setLayout(layout);

		return result;
	}
};

ArtesianResultView::ArtesianResultView(unsigned long long debit, QWidget * parent) 
	: QDialog(parent),
	  _impl(new Impl(this, debit))
{ }

ArtesianResultView::~ArtesianResultView() { }
