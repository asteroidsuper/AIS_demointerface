#include "tank.hpp"

class Tank::Impl
{
	Tank* const _parent;

	unsigned long long _waterUnitsCount = 100000;
	unsigned long long _maxCapacity = 200000;

public:
	Impl(Tank* parent) : _parent(parent)
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
		count = min(count, _maxCapacity);

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

Tank& Tank::instance()
{
	static Tank tank;

	return tank;
}

void Tank::setMaxCapacity(unsigned long long capacity)
{
	_impl->setMaxCapacity(capacity);
}

unsigned long long Tank::maxCapacity() const
{
	return _impl->maxCapacity();
}

void Tank::setWater(unsigned long long count)
{
	_impl->setWaterCount(count);
}

unsigned long long Tank::waterCount() const
{
	return _impl->waterCount();
}

Tank::Tank(QObject * parent)
	: QObject(parent),
	  _impl(new Impl(this))
{ }

Tank::~Tank() { }