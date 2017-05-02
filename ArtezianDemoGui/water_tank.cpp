#include "water_tank.hpp"

class WaterTank::Impl
{
	WaterTank* const _parent;

	unsigned long long _waterUnitsCount = 100000;
	unsigned long long _maxCapacity = 200000;

public:
	Impl(WaterTank* parent) : _parent(parent)
	{}

	void setMaxCapacity(unsigned long long capacity)
	{
		if (capacity == _maxCapacity)
		{
			return;
		}

		_maxCapacity = capacity;

		emit _parent->maxCapacityChanged(capacity);
		setWaterCount(_waterUnitsCount);
	}

	unsigned long long maxCapacity() const
	{
		return _maxCapacity;
	}

	void setWaterCount(unsigned long long count)
	{
		count = std::min(count, _maxCapacity);

		if (count != _waterUnitsCount)
		{
			_waterUnitsCount = count;
			emit _parent->waterCountChanged(count);
		}
	}

	unsigned long long waterCount() const
	{
		return _waterUnitsCount;
	}

	~Impl() { }

private:

};

WaterTank& WaterTank::instance()
{
	static WaterTank tank;

	return tank;
}

void WaterTank::setMaxCapacity(unsigned long long capacity)
{
	_impl->setMaxCapacity(capacity);
}

unsigned long long WaterTank::maxCapacity() const
{
	return _impl->maxCapacity();
}

void WaterTank::setWater(unsigned long long count)
{
	_impl->setWaterCount(count);
}

unsigned long long WaterTank::waterCount() const
{
	return _impl->waterCount();
}

WaterTank::WaterTank(QObject * parent)
	: QObject(parent),
	  _impl(new Impl(this))
{ }

WaterTank::~WaterTank() { }