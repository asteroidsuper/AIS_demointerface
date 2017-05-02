#pragma once
#include <QObject>

class WaterTank : public QObject
{
	Q_OBJECT

public:
	~WaterTank();

	static WaterTank& instance();

	void setMaxCapacity(unsigned long long capacity);
	unsigned long long maxCapacity() const;

	void setWater(unsigned long long count);
	unsigned long long waterCount() const;

	Q_SIGNAL void waterCountChanged(unsigned long long count);
	Q_SIGNAL void maxCapacityChanged(unsigned long long capacity);

private:
	class Impl;
	std::unique_ptr<Impl> _impl;

	WaterTank(QObject * parent = nullptr);
};
