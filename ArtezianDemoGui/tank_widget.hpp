#pragma once
#include <QSlider>

class TankWidget : public QSlider
{
	Q_OBJECT

public:
	TankWidget(QWidget * parent = nullptr);
	~TankWidget();

	virtual void paintEvent(QPaintEvent* event) override;

	virtual QSize minimumSizeHint() const override;

	virtual QSize sizeHint() const override;

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};
