// Generate board setups to be used for building a dataset. Continuously shuffle all pieces, takes a subset of them,
// fill the rest with free cells, shuffle again, and print as a matrix until each piece appears INDIVIDUAL_COUNT_GOAL
// of times at least. The success criteria is checked after each iteration, where one iteration consists of generating
// boards having [MINIMUM_AMOUNT, MAXIMUM_AMOUNT] elements (non-free pieces)


#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <fstream>


const bool LOG = true;							// if true, writes on console what is written in the file
const std::string PATH = "board.txt";			// output file path
std::ofstream outfile;							// output file
std::ostringstream os;							// for efficient "string concatenation"

const int INDIVIDUAL_COUNT_GOAL = 500;			// the least amount of counts of each piece. count = how many times a piece appears throughout all generated boards
const int MINIMUM_AMOUNT = 4;					// minimum number of pieces on a table
const int MAXIMUM_AMOUNT = 32;					// maximum amount of pieces on a table

std::vector<std::string> allPieces = {
	"BR", "BN", "BB", "BQ", "BK", "BB", "BN", "BR",
	"BP", "BP", "BP", "BP", "BP", "BP", "BP", "BP",
	"WP", "WP", "WP", "WP", "WP", "WP", "WP", "WP",
	"WR", "WN", "WB", "WQ", "WK", "WB", "WN", "WR"
};

std::map<std::string, int> encodingsAndCounts = {
	{ "WP", 0 },
	{ "WB", 0 },
	{ "WN", 0 },
	{ "WR", 0 },
	{ "WQ", 0 },
	{ "WK", 0 },
	{ "BP", 0 },
	{ "BB", 0 },
	{ "BN", 0 },
	{ "BR", 0 },
	{ "BQ", 0 },
	{ "BK", 0 }
};

int iterationNo = -1;	// iteration counter
int boardNo = 0;		// board counter

std::random_device randomDevice;
std::mt19937 generator(randomDevice());


// get a board with "elements" elements (pieces) on it
void getABoard(int elements)
{
	std::shuffle(allPieces.begin(), allPieces.end(), generator);

	auto first = allPieces.begin();
	auto last = allPieces.begin() + elements;
	std::vector<std::string> board(first, last);  // take the first elements pieces

	// log/save the resulting combinations
	os.str("");
	os << boardNo << "(" << iterationNo << "-" << board.size() << "): ";
	for (const std::string& elem : board)
	{
		os << elem << " ";
	}
	os << std::endl;
	std::string combinationString = os.str();
	if (LOG) { std::cout << combinationString; }
	outfile << combinationString;

	// update global counts and board number
	for (const std::string& elem : board)
	{
		encodingsAndCounts[elem]++;
	}
	boardNo++;

	// pad with empty cells and shuffle again
	board.resize(64, "**");
	std::shuffle(board.begin(), board.end(), generator);

	// log/save the resulting boards
	os.str("");
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			os << std::setfill(' ') << std::setw(3) << board[static_cast<std::size_t>(i) * 8 + j] << " ";
		}
		os << std::endl;
	}
	os << std::endl;
	std::string boardString = os.str();
	if (LOG) { std::cout << boardString << std::endl; }
	outfile << boardString << std::endl;
}


int main()
{
	// try opening the file
	outfile.open(PATH);
	if (!outfile.is_open())
	{
		std::cerr << "Could not open " << PATH << std::endl;
		return 1;
	}

	// generate until goal met
	bool done = false;
	while (!done)
	{
		iterationNo++;
		for (int i = MINIMUM_AMOUNT; i <= MAXIMUM_AMOUNT; i++)
		{
			getABoard(i);
		}
		done = true;  // presume success
		for (auto const& encodingAndCount : encodingsAndCounts)
		{
			if (encodingAndCount.second < INDIVIDUAL_COUNT_GOAL)
			{
				done = false;  // found unsatisfied count
				break;
			}
		}
	}

	// save/log conclusions
	int totalCount = 0;
	os.str("");
	os << "Reached goal in " << iterationNo << " iterations (" << boardNo << " boards)" << std::endl;
	os << "Final counts:" << std::endl;
	for (auto const& encodingAndCount : encodingsAndCounts)
	{
		os << encodingAndCount.first << ": " << encodingAndCount.second << std::endl;
		totalCount += encodingAndCount.second;
	}
	os << "Total count: " << totalCount << std::endl;
	std::string summaryString = os.str();
	if (LOG) { std::cout << summaryString; }
	outfile << summaryString;

	outfile.close();
}