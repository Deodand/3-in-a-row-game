#include <gtest/gtest.h>
#include <sum_function.h>
#include "window.h"


TEST(basic_check, test_eq) {
	EXPECT_EQ(1, 1);
}

TEST(basic_check, test_neq) {
	EXPECT_EQ(sum(1, 22), 23);
}

class WindowFixture : public ::testing::Test
{
protected:
	void SetUp()
	{
		win = new MainWindow;
	}
	void TearDown()
	{
		delete win;
	}
	MainWindow *win;
};

TEST_F(WindowFixture, test1)
{
	ASSERT_EQ(std::string(win->ClassName()), "Circle Window Class");
}

TEST_F(WindowFixture, test2)
{
	ASSERT_NE(std::string(win->ClassName()), "Circle Window Clas");
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
