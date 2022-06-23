#include <algorithm>
#include <ranges>
#include <gtest/gtest.h>

#include "Scenes/EventLog.hpp"

using namespace Scenes;

class EventLogTests : public testing::Test
{
protected:
	size_t linesRead = 0;
	EventLog log{ linesRead };
};

TEST_F(EventLogTests, AddAndQuery)
{
	const LogNameType testStr1 = "Test Event 1";
	LogResultType expected;

	EXPECT_EQ(expected, log.query(testStr1));

	log.addLog(testStr1);
	expected.push_back(linesRead);
	linesRead++;
	log.addLog(testStr1);
	expected.push_back(linesRead);

	EXPECT_EQ(expected, log.query(testStr1));
}

TEST_F(EventLogTests, FindEventCorrectly)
{
	log.addLog("Event, 10");
	linesRead++;
	log.addLog("Event, 10");
	log.addLog("Event, 1");
	log.addLog("Event, 19");

	log.addLog("Event2, 10");
	linesRead++;
	log.addLog("Event2, 10");
	log.addLog("Event2, 1");
	log.addLog("Event2, 19");

	log.addLog("Event3, ");

	log.addLog(", 19");
	log.addLog("Event, 19");

	std::vector<LogNameType> expected{
		"Event, 10",
		"Event, 1",
		"Event, 19"
	};
	auto actual = log.findKeys("Event");

	// Order is irrelevant 
	std::ranges::sort(expected);
	std::ranges::sort(actual);

	EXPECT_EQ(expected, actual);
	EXPECT_EQ(std::vector<Scenes::LogNameType>{}, log.findKeys("NonExistent"));
}
