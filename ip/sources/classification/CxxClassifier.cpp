#include "../../headers/classification/CxxClassifier.h"


CxxClassifier::CxxClassifier(bool uniteFrees)
{
	this->uniteFrees = uniteFrees;
	classCount = (uniteFrees) ? 13 : 14;
	trained = false;
}



int CxxClassifier::externalToInternal(std::string encoding)
{
	if (!uniteFrees)
	{
		if (encoding == "WF") { return 0; };
		if (encoding == "WP") { return 1; };
		if (encoding == "WB") { return 2; };
		if (encoding == "WN") { return 3; };
		if (encoding == "WR") { return 4; };
		if (encoding == "WQ") { return 5; };
		if (encoding == "WK") { return 6; };
		if (encoding == "BF") { return 7; };
		if (encoding == "BP") { return 8; };
		if (encoding == "BB") { return 9; };
		if (encoding == "BN") { return 10; };
		if (encoding == "BR") { return 11; };
		if (encoding == "BQ") { return 12; };
		if (encoding == "BK") { return 13; };
	}
	else  // WF and BF both encoded as 0 ("merged" labels)
	{
		if (encoding == "WF") { return 0; };
		if (encoding == "WP") { return 1; };
		if (encoding == "WB") { return 2; };
		if (encoding == "WN") { return 3; };
		if (encoding == "WR") { return 4; };
		if (encoding == "WQ") { return 5; };
		if (encoding == "WK") { return 6; };
		if (encoding == "BF") { return 0; };
		if (encoding == "BP") { return 7; };
		if (encoding == "BB") { return 8; };
		if (encoding == "BN") { return 9; };
		if (encoding == "BR") { return 10; };
		if (encoding == "BQ") { return 11; };
		if (encoding == "BK") { return 12; };
	}
	
	SPDLOG_ERROR("Could not convert {}, considering it 0", encoding);
	return 0;
}


QString CxxClassifier::internalToExternal(int encoding)
{
	if (!uniteFrees)
	{
		if (encoding == 0) { return "WF"; };
		if (encoding == 1) { return "WP"; };
		if (encoding == 2) { return "WB"; };
		if (encoding == 3) { return "WN"; };
		if (encoding == 4) { return "WR"; };
		if (encoding == 5) { return "WQ"; };
		if (encoding == 6) { return "WK"; };
		if (encoding == 7) { return "BF"; };
		if (encoding == 8) { return "BP"; };
		if (encoding == 9) { return "BB"; };
		if (encoding == 10) { return "BN"; };
		if (encoding == 11) { return "BR"; };
		if (encoding == 12) { return "BQ"; };
		if (encoding == 13) { return "BK"; };
	}
	else  // encoding 0 returns WF although it technically contains BF as well. does not matter since VAR does not distinguish
	{
		if (encoding == 0) { return "WF"; };
		if (encoding == 1) { return "WP"; };
		if (encoding == 2) { return "WB"; };
		if (encoding == 3) { return "WN"; };
		if (encoding == 4) { return "WR"; };
		if (encoding == 5) { return "WQ"; };
		if (encoding == 6) { return "WK"; };
		if (encoding == 7) { return "BP"; };
		if (encoding == 8) { return "BB"; };
		if (encoding == 9) { return "BN"; };
		if (encoding == 10) { return "BR"; };
		if (encoding == 11) { return "BQ"; };
		if (encoding == 12) { return "BK"; };
	}

	SPDLOG_ERROR("Could not convert {}", encoding);
	return "WF";
}


void CxxClassifier::logDistribution(std::map<std::string, int> labelsAndCounts, std::string setType)
{
	std::ostringstream os;
	os << "Samples by class in " << setType << ":" << std::endl;
	for (auto const& labelAndCount : labelsAndCounts)
	{
		os << "    " << labelAndCount.first << ":" << labelAndCount.second << std::endl;
	}
	SPDLOG_TRACE(os.str());
}
