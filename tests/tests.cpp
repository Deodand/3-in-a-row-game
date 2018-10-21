#include <gtest/gtest.h>
#include <sum_function.h>

TEST(basic_check, test_eq) {
	EXPECT_EQ(1, 1);
}

TEST(basic_check, test_neq) {
	EXPECT_EQ(sum(2, 1), 1);
}


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}