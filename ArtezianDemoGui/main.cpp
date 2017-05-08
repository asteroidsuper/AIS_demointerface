#include "main_window.h"

#include <QtWidgets/QApplication>

#include <QMessageBox>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	try
	{
		MainWindow w;
		w.show();
		return application.exec();
	}
	catch(const std::runtime_error& exp)
	{
		QMessageBox::critical(nullptr, u8"Ошибка при инициализации: ", exp.what());

		return -1;
	}
}
