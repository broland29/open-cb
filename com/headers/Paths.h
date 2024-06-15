#pragma once

#include <string>


namespace Paths
{
	const std::string KNN_FOLDER = "C:\\open-cb\\mem\\knn";		// KNN saved and loaded here
	const std::string SVM_FOLDER = "C:\\open-cb\\mem\\svm";		// SVM saved and loaded here
	const std::string CNN_FOLDER = "C:\\open-cb\\mem\\cnn";		// CNN saved and loaded here

	const std::string TEMPORARY_FOLDER  = "C:\\open-cb\\mem\\img\\tmp";  // images labeled but yet put in trn/val/tst set
	const std::string TRAIN_FOLDER      = "C:\\open-cb\\mem\\img\\trn";  // images designated for training
	const std::string VALIDATION_FOLDER = "C:\\open-cb\\mem\\img\\val";  // images designated for validation
	const std::string TEST_FOLDER       = "C:\\open-cb\\mem\\img\\tst";  // images designated for testing
	const std::string BOARD_FOLDER      = "C:\\open-cb\\mem\\img\\brd";  // images for actual classification (cells from one image/board)
	const std::string GRAB_FOLDER       = "C:\\open-cb\\mem\\img\\grb";  // images grabbed by CameraReader, but not yet used

	const std::string CNN_TRAIN_SCRIPT = "C:\\open-cb\\ip\\sources\\classification\\cnn_train.py";
	const std::string CNN_TEST_SCRIPT = "C:\\open-cb\\ip\\sources\\classification\\cnn_test.py";
	const std::string CNN_CLASSIFY_BOARD_SCRIPT = "C:\\open-cb\\ip\\sources\\classification\\cnn_classify_board.py";

	const std::string JSON_PARAMETERS = "C:\\open-cb\\mem\\per\\persistence.json";
}