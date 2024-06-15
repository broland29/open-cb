#include "../../headers/classification/ConvolutionalNeuralNetwork.h"

ConvolutionalNeuralNetwork::ConvolutionalNeuralNetwork(ConvolutionalNeuralNetworkParameters convolutionalNeuralNetworkParameters)
{
	epochs = convolutionalNeuralNetworkParameters.epochs;
	applyAugmentation = convolutionalNeuralNetworkParameters.applyAugmentation;
}



int ConvolutionalNeuralNetwork::train()
{
	// --no-capture-output for avoiding stdout buffering - https://github.com/conda/conda/issues/9412#issuecomment-719759077
	// example: ...
	std::string command = "conda run --no-capture-output -n rolienv2 python " + std::string(TRAIN_SCRIPT_PATH) +
		std::string(" ") + std::string(TRAIN_FOLDER_PATH) +			// argv[1]
		std::string(" ") + std::string(VALIDATION_FOLDER_PATH) +	// argv[2]
		std::string(" ") + std::string(CNN_FOLDER_PATH) + 			// argv[3]
		std::string(" ") + std::to_string(epochs) +					// argv[4]
		std::string(" ") + (applyAugmentation ? "true" : "false");	// argv[5]
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
	std::string command = "conda run --no-capture-output -n rolienv2 python " + std::string(CLASSIFY_BOARD_SCRIPT_PATH) +
		std::string(" ") + std::string(BOARD_FOLDER_PATH) +			// argv[1]
		std::string(" ") + std::string(CNN_FOLDER_PATH) +			// argv[2]
		std::string(" ") + std::string(CNN_FOLDER_PATH);			// argv[3]
	SPDLOG_TRACE("Executing command {}", command);
	
	int ret = system(command.c_str());
	SPDLOG_TRACE("Command returned {}", ret);
	
	// in case of failure from the script side, end
	if (ret != 0)
	{
		return ret;
	}

	std::string path = CNN_FOLDER_PATH + std::string("\\pred.txt");
	std::ifstream infile(path);
	if (!infile.is_open())
	{
		SPDLOG_ERROR("Could not open predictions file {}", path);
		return 1;
	}

	encodings.resize(64);
	int buffer;
	for (int i = 0; i < 64; i++)
	{
		infile >> buffer;
		encodings[i] = internalToExternal(buffer);
	}
	
	return ret;
}


int ConvolutionalNeuralNetwork::save()
{
	SPDLOG_INFO("CNN is implicitly saved after training");
	return 0;
}


int ConvolutionalNeuralNetwork::load()
{
	SPDLOG_INFO("CNN is implicitly loaded before testing/ classifying board");
	return 0;
}


QString ConvolutionalNeuralNetwork::internalToExternal(int encoding)
{
	// print(dataset_trn.class_names) in cnn_train.py results ['BB', 'BF', 'BK', 'BN', 'BP', 'BQ', 'BR', 'WB', 'WF', 'WK', 'WN', 'WP', 'WQ', 'WR']
	if (encoding == 0) { return "BB"; };
	if (encoding == 1) { return "BF"; };
	if (encoding == 2) { return "BK"; };
	if (encoding == 3) { return "BN"; };
	if (encoding == 4) { return "BP"; };
	if (encoding == 5) { return "BQ"; };
	if (encoding == 6) { return "BR"; };
	if (encoding == 7) { return "WB"; };
	if (encoding == 8) { return "WF"; };
	if (encoding == 9) { return "WK"; };
	if (encoding == 10) { return "WN"; };
	if (encoding == 11) { return "WP"; };
	if (encoding == 12) { return "WQ"; };
	if (encoding == 13) { return "WR"; };

	SPDLOG_ERROR("Could not convert {}", encoding);
	return "WF";
}