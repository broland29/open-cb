#include "../../headers/classification/ConvolutionalNeuralNetwork.h"

ConvolutionalNeuralNetwork::ConvolutionalNeuralNetwork()
{
	// todo
}



int ConvolutionalNeuralNetwork::train()
{
	// --no-capture-output for avoiding stdout buffering - https://github.com/conda/conda/issues/9412#issuecomment-719759077
	// example: ...
	std::string command = "conda run --no-capture-output -n rolienv2 python " + std::string(TRAIN_SCRIPT_PATH) +
		std::string(" ") + std::string(TRAIN_FOLDER_PATH) +			// argv[1]
		std::string(" ") + std::string(VALIDATION_FOLDER_PATH) +	// argv[2]
		std::string(" ") + std::string(CNN_FOLDER_PATH);			// argv[3]
	SPDLOG_TRACE("Executing command {}", command);

	int ret = system(command.c_str());
	SPDLOG_TRACE("Command returned {}", ret);
	return ret;
}


int ConvolutionalNeuralNetwork::test()
{
	// example: 
	std::string command = "conda run --no-capture-output -n rolienv2 python " + std::string(TEST_SCRIPT_PATH) +
		std::string(" ") + std::string(TEST_FOLDER_PATH) +			// argv[1]
		std::string(" ") + std::string(CNN_FOLDER_PATH);			// argv[2]
	SPDLOG_TRACE("Executing command {}", command);

	int ret = system(command.c_str());
	SPDLOG_TRACE("Command returned {}", ret);
	return ret;
}


int ConvolutionalNeuralNetwork::classifyBoard(QVector<QString>& encodings)
{
	encodings.resize(64, "?");
	// todo
	return 404;
}


int ConvolutionalNeuralNetwork::save(std::string folderPath)
{
	// todo
	// CNNs already saved after training phase - maybe renaming and some move operations are enough
	return 404;
}


int ConvolutionalNeuralNetwork::load(std::string folderPath)
{
	// todo
	// CNNs already load when testing/ classifying board - maybe enough to change a field to current CNN
	return 404;
}