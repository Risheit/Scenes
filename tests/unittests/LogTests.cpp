#include <algorithm>
#include <ranges>
#include <gtest/gtest.h>

#include "Scenes/Log.h"

using namespace Scenes;

class LogTests: public testing::Test
{
protected:
	size_t linesRead = 0;
	Log log{ linesRead };
};

TEST_F(LogTests, AddAndQuery)
{
	const LogNameType testStr1 = "Test Log";
	LogResultType expected;

	EXPECT_EQ(expected, log.query(testStr1)); // Test empty query

	log.addLog(testStr1);
	expected.push_back(linesRead);
	linesRead++;
	log.addLog(testStr1);
	expected.push_back(linesRead);

	EXPECT_EQ(expected, log.query(testStr1)); // Test query

	const LogNameType testStr2 = "Another Log";
	expected.clear();
	linesRead += 10;
	log.addLog(testStr2);
	expected.push_back(linesRead);
	log.addLog(testStr2);
	expected.push_back(linesRead);

	EXPECT_EQ(expected, log.query(testStr2)); // Test query
}

TEST_F(LogTests, FindLogCorrectly)
{
	log.addLog("Test");
	log.addLog("Test Call");
	log.addLog("Call Test Call");
	log.addLog("Call Test2 Call");
	log.addLog("test");
	log.addLog("Other");

	std::vector<LogNameType> expected{
		"Test",
		"Test Call",
		"Call Test Call",
		"Call Test2 Call"
	};
	auto actual = log.findKeys("Test");

	// Order is irrelevant
	std::ranges::sort(expected);
	std::ranges::sort(actual);

	EXPECT_EQ(expected, actual);
	EXPECT_EQ(std::vector<Scenes::LogNameType>{}, log.findKeys("NonExistant"));
}
