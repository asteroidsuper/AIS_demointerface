#pragma once

#include <QObject>
#include <QString>
#include <QVector>

#include <memory>
#include <stdexcept>

#define NO_COPYABLE(arg) arg(arg&) = delete;\
arg& operator = (arg&) = delete;

#define PIMPL(arg) private:\
class Impl; \
std::unique_ptr<Impl> _impl;\
NO_COPYABLE(arg)

namespace Json
{
	class Value;
}

namespace helpers
{
	Json::Value parseFile(const QString& fileName);
}