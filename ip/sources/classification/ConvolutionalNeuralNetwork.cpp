#include "../../headers/classification/ConvolutionalNeuralNetwork.h"
#include <thread>

ConvolutionalNeuralNetwork::ConvolutionalNeuralNetwork()
{
	// todo
}


int ConvolutionalNeuralNetwork::train()
{
	// example: conda run -n rolienv2 python train.py C:\open-cb\mem\img\trn
	std::string command = "conda run -n rolienv2 python " + std::string(TRAIN_SCRIPT_PATH) +
		std::string(" ") + std::string(TRAIN_FOLDER_PATH) +				// argv[1]
		std::string(" ") + std::string(VALIDATION_FOLDER_PATH);			// argv[2]
	SPDLOG_TRACE("Executing command {}", command);

	int ret = system(command.c_str());
	SPDLOG_TRACE("Command returned {}", ret);
	return ret;
}


int ConvolutionalNeuralNetwork::test()
{
	// todo
	return 404;
}


std::string ConvolutionalNeuralNetwork::classifyBoard()
{
	// todo
	return "";
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