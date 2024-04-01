#pragma once

#include "AbstractClassifier.h"

class SupportVectorMachine : public AbstractClassifier
{
public:
	SupportVectorMachine();

	virtual int train() override;

	virtual int test() override;

	virtual std::string classifyBoard() override;

	virtual int save(std::string folderPath) override;

	virtual int load(std::string folderPath) override;
};