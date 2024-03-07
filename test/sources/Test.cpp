#include "../headers/Test.h"


int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(TestSuiteName, TestName)
{
	std::cout << "Hello from Test!\n";
	EXPECT_TRUE(true);
}


