#pragma once

#include <QObject>
#include <QString>
#include <QVector>

#include <memory>
#include <stdexcept>

namespace Json
{
	class Value;
}

namespace helpers
{
	Json::Value parseFile(const QString& fileName);
}