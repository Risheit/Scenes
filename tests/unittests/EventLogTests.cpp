#include "Scenes/EventLog.h"
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
