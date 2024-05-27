#include "../headers/Test.h"
#include "../log/headers/Logger.h"


int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	//Logger::initialize();  // uncomment if test fails and traces needed

	return RUN_ALL_TESTS();
}

TEST(SampleTestSuiteName, SampleTestName)
{
	std::cout << "Hello from SampleTestName!\n";
	EXPECT_TRUE(true);
}


