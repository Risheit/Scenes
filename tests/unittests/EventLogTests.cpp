#include "Scenes/EventLog.h"
#include <algorithm>
#include <gtest/gtest.h>

using namespace Scenes;

class EventLogTests : public ::testing::Test
{
protected:
	size_t linesRead = 0;
	EventLog log = EventLog(linesRead);
};

TEST_F(EventLogTests, AddAndQuery)
{
	LogNameType testStr1 = "Test Event 1";
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

	// Vector is ordered in 
	std::vector<Scenes::LogNameType> expected{
		"Event, 10",
		"Event, 1",
		"Event, 19"
	};
	auto actual = log.findEventCalls("Event");
	
	// Order is irrelavent 
	std::sort(expected.begin(), expected.end());
	std::sort(actual.begin(), actual.end());

	EXPECT_EQ(expected, actual);
	EXPECT_EQ(std::vector<Scenes::LogNameType>(), log.findEventCalls("NonExistant"));
}
