#include "../headers/Test.h"


int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(SampleTestSuiteName, SampleTestName)
{
	std::cout << "Hello from SampleTestName!\n";
	EXPECT_TRUE(true);
}


