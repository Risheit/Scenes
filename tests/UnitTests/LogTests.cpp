#include <algorithm>
#include <gtest/gtest.h>

#include "Scenes/Log.h"

using namespace Scenes;

class LogTests : public ::testing::Test
{
protected:
	size_t linesRead = 0;
	Log log{ linesRead };
};

TEST_F(LogTests, AddAndQuery)
{
	LogNameType testStr1 = "Test Log";
	LogResultType expected;

	EXPECT_EQ(expected, log.query(testStr1));

	log.addLog(testStr1);
	expected.push_back(linesRead);
	linesRead++;
	log.addLog(testStr1);
	expected.push_back(linesRead);

	EXPECT_EQ(expected, log.query(testStr1));
}

TEST_F(LogTests, FindLogCorrectly)
{
	
}