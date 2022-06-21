#include <string>
#include <gtest/gtest.h>

#include "Scenes/Event.hpp"

using namespace Scenes;

// Demonstrate some basic assertions.
class EventTests : public testing::Test
{
protected:
	size_t linesRead{ 0 };
	EventLog log{ EventLog(linesRead) };
	Event<int> eve{ [](const int a) -> int { return a; }, "test", log };
	Event<> noArgs{ []() -> int { return 0; }, "test", log };
};

TEST_F(EventTests, CallsCorrectly)
{
	EXPECT_EQ(4, eve(4));
	EXPECT_EQ(123499, eve(123499));
	EXPECT_EQ(0, noArgs());
}

TEST_F(EventTests, CallsLoggedCorrectly)
{
	LogResultType eveCallExpected;
	linesRead++;
	(void)eve(6);
	eveCallExpected.push_back(linesRead);
	linesRead++;
	linesRead++;
	(void)eve(6);
	eveCallExpected.push_back(linesRead);
	EXPECT_EQ(eveCallExpected, log.query(eve.eventString()));
	
	
	LogResultType noArgsCallExpected;
	EXPECT_EQ(noArgsCallExpected, log.query(noArgs.eventString()));

	linesRead += 5;
	noArgsCallExpected.push_back(linesRead);
	(void)noArgs();
	EXPECT_EQ(noArgsCallExpected, log.query(noArgs.eventString()));


	LogResultType noreturnCallExpected;
	noreturnCallExpected.push_back(linesRead);
	linesRead++;
	noreturnCallExpected.push_back(linesRead);
}