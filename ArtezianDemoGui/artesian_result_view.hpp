#pragma once
#include <QDialog>

class ArtesianResultView : public QDialog 
{
	Q_OBJECT

public:
	ArtesianResultView(unsigned long long debit, QWidget * parent = nullptr);
	~ArtesianResultView();

private:
	class Impl;
	unique_ptr<Impl> _impl;
};
