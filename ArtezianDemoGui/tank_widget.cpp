#include "tank_widget.hpp"

#include "tank.hpp"

#include <QStyleOption>
#include <QPainter>

class TankWidget::Impl : public QObject
{
private:
	TankWidget* const _parent;

	int _segCount = 5;

	QColor _waterColor = "#42f4eb";
	QColor _segColor = "black";

public:
	Impl(TankWidget* parent) : _parent(parent)
	{
		parent->setOrientation(Qt::Vertical);
		grabTankValues();
		connectTank();
		connectParent();

		_parent->setPageStep(10000);
	}

	~Impl() { }

	void paintEvent()
	{
		QPainter painter(_parent);

		QStyleOptionSlider option;
		_parent->initStyleOption(&option);

		drawWater(painter, option);
		drawSegs(painter);
	}

private:
	void drawWater(QPainter& painter, const QStyleOptionSlider& option)
	{
		int maximum = option.maximum - option.minimum;
		int value = option.sliderValue - option.minimum;

		int h = _parent->height();

		int waterHeight = (value * _parent->height()) / maximum;

		painter.fillRect(0, _parent->height() - waterHeight, _parent->width(), waterHeight, _waterColor);
	}

	void drawSegs(QPainter& painter)
	{
		int step = _parent->height() / (_segCount + 2);

		painter.save();

		painter.setPen(QPen(_segColor, 4));

		int y;
		
		if (_segCount % 2 == 0)
		{
			y = step * 1.5;
		}
		else
		{
			y = _segCount % 2 ? _parent->height() / 2 - step * (_segCount / 2) : 0;
		}
		
		int x = _parent->width() / 2;

		for (int i = 0; i < _segCount; ++i)
		{
			painter.drawLine(0, y, x, y);
			
			y += step;
		}

		painter.restore();
	}

	void grabTankValues()
	{
		auto& tank = Tank::instance();

		auto v = tank.waterCount();

		_parent->setMaximum(tank.maxCapacity());
		_parent->setValue(tank.waterCount());
	}

	void connectParent()
	{
		connect(_parent, &TankWidget::valueChanged, this, &Impl::sliderValueChanged);
	}

	void connectTank()
	{
		auto& tank = Tank::instance();
		connect(&tank, &Tank::waterCountChanged, this, &Impl::tankChanged, Qt::QueuedConnection);
		connect(&tank, &Tank::maxCapacityChanged, this, &Impl::tankChanged, Qt::QueuedConnection);
	}

	Q_SLOT void sliderValueChanged()
	{
		auto& tank = Tank::instance();

		auto vl = _parent->value();

		tank.setWater(_parent->value());
	}

	Q_SLOT void tankChanged()
	{
		grabTankValues();
	}
};

TankWidget::TankWidget(QWidget * parent)
	: QSlider(parent),
	  _impl(new Impl(this))
{ }

TankWidget::~TankWidget() { }

void TankWidget::paintEvent(QPaintEvent * event)
{
	_impl->paintEvent();
	Q_UNUSED(event);
}

QSize TankWidget::minimumSizeHint() const
{
	return QSize(112, 180);
}

QSize TankWidget::sizeHint() const
{
	return minimumSizeHint();
}
