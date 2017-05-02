#pragma once

struct ArtesianPair
{
	unsigned long long debit = 0;
	unsigned long long count = 0;

	bool operator < (const ArtesianPair& well) const;
	bool operator > (const ArtesianPair& well) const;
};

class ArtesianWellCounter
{
public:
	ArtesianWellCounter();
	~ArtesianWellCounter();

	QVector<ArtesianPair> calcToDebit(unsigned long long required, bool more = true) const;

	unsigned long long debitFor(const QVector<ArtesianPair>& wells) const;

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};