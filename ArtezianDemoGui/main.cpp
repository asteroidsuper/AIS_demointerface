#include "main_window.h"

#include <QtWidgets/QApplication>

#include <QMessageBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	try
	{
		MainWindow w;
		w.show();
		return a.exec();
	}
	catch(const runtime_error& exp)
	{
		QMessageBox::critical(nullptr, u8"Ошибка при инициализации", exp.what());
	}
}
