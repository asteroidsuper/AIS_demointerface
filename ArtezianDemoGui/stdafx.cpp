#include <json/value.h>
#include <json/reader.h>

#include <QFile>

Json::Value helpers::parseFile(const QString& fileName)
{
	QFile file(fileName);

	if (file.open(QIODevice::ReadOnly) == false)
	{
		throw std::runtime_error("Json can not be opened");
	}

	auto data = file.readAll();

	file.close();

	Json::Value result;

	Json::Reader reader;

	const int BOMSize = 3;

	if (reader.parse(data.data() + BOMSize, data.data() + data.size(), result) == false)
	{
		throw std::runtime_error("parsing error has occurred");
	}

	return result;
}