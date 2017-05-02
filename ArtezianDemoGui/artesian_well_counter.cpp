#include "artesian_well_counter.h"

#include <QByteArray>
#include <QFile>

#include <json\reader.h>
#include <json\value.h>

namespace {
	void takeNext(QVector<ArtesianPair>& wells, QVector<ArtesianPair>& dst)
	{
		for (int i = 0; i < wells.count(); ++i)
		{
			auto& well = wells[i];
			if (well.count == 0)
			{
				continue;
			}

			well.count--;
			dst[i].count++;

			return;
		}
	}
}

class ArtesianWellCounter::Impl
{
private:
	QVector<ArtesianPair> _wells;

public:
	Impl()
	{
		loadFromJson(u8"wells.json");
	}

	~Impl()	{ }

	QVector<ArtesianPair> calcForDebit(unsigned long long debit, bool more) const
	{
		if(debit > maxDebit())
		{
			return { };
		}

		auto wells = _wells;

		std::sort(wells.begin(), wells.end());

		QVector<ArtesianPair> result;

		for (auto& well : wells)
		{
			ArtesianPair res;
			res.count = 0;
			res.debit = well.debit;

			result << res;
		}

		while (debitFor(result) < debit)
		{
			takeNext(wells, result);
		}

		return result;
	}

	unsigned long long maxDebit() const
	{
		return debitFor(_wells);
	}

	unsigned long long debitFor(const QVector<ArtesianPair>& wells) const 
	{
		unsigned long long result = 0;

		for (auto& well : wells)
		{
			result += well.count * well.debit;
		}

		return result;
	}

private:
	void loadFromJson(const QString& path)
	{
		Json::Value root;
		{
			QFile file(path);

			file.open(QIODevice::ReadOnly);

			auto docData = file.readAll();

			const int BOMSize = 3;

			if (Json::Reader().parse(docData.data() + BOMSize, docData.data() + docData.size(), root) == false)
			{
				throw std::runtime_error("parse error (artesian wells)");
			}
		}

		for (auto& well : root)
		{
			ArtesianPair newItem;

			newItem.debit = well["debit"].asUInt64();

			newItem.count = well["count"].asUInt64();

			_wells << newItem;
		}
	}

};

ArtesianWellCounter::ArtesianWellCounter() : _impl(new Impl)
{ }

ArtesianWellCounter::~ArtesianWellCounter()
{
}

QVector<ArtesianPair> ArtesianWellCounter::calcToDebit(unsigned long long required, bool more) const
{
	return _impl->calcForDebit(required, more);
}

unsigned long long ArtesianWellCounter::debitFor(const QVector<ArtesianPair>& wells) const
{
	return _impl->debitFor(wells);
}

bool ArtesianPair::operator<(const ArtesianPair & well) const
{
	return debit < well.debit;
}

bool ArtesianPair::operator>(const ArtesianPair & well) const
{
	return debit > well.debit;
}
