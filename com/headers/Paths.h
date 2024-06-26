#pragma once

#include <string>


namespace Paths
{
	const std::string OPENCB_ROOT = "C:\\open-cb";

	const std::string KNN_FOLDER = OPENCB_ROOT + "\\mem\\knn";		// KNN saved and loaded here
	const std::string SVM_FOLDER = OPENCB_ROOT + "\\mem\\svm";		// SVM saved and loaded here
	const std::string CNN_FOLDER = OPENCB_ROOT + "\\mem\\cnn";		// CNN saved and loaded here

	const std::string TEMPORARY_FOLDER  = OPENCB_ROOT + "\\mem\\img\\tmp";  // images labeled but yet put in trn/val/tst set
	const std::string TRAIN_FOLDER      = OPENCB_ROOT + "\\mem\\img\\trn";  // images designated for training
	const std::string VALIDATION_FOLDER = OPENCB_ROOT + "\\mem\\img\\val";  // images designated for validation
	const std::string TEST_FOLDER       = OPENCB_ROOT + "\\mem\\img\\tst";  // images designated for testing
	const std::string BOARD_FOLDER      = OPENCB_ROOT + "\\mem\\img\\brd";  // images for actual classification (cells from one image/board)
	const std::string GRAB_FOLDER       = OPENCB_ROOT + "\\mem\\img\\grb";  // images grabbed by CameraReader, but not yet used

	const std::string CNN_TRAIN_SCRIPT			= OPENCB_ROOT + "\\ip\\sources\\classification\\cnn_train.py";
	const std::string CNN_TEST_SCRIPT			= OPENCB_ROOT + "\\ip\\sources\\classification\\cnn_test.py";
	const std::string CNN_CLASSIFY_BOARD_SCRIPT = OPENCB_ROOT + "\\ip\\sources\\classification\\cnn_classify_board.py";
	const std::string CNN_PATH_CONF_MATRIX		= OPENCB_ROOT + "\\mem\\cnn\\conf_matrix.txt";

	const std::string JSON_PARAMETERS = OPENCB_ROOT + "\\mem\\per\\persistence.json";
}