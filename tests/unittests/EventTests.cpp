#include "Scenes/Event.hpp"
#include <gtest/gtest.h>
#include <string>

using namespace Scenes;

// Demonstrate some basic assertions.
class EventTests : public ::testing::Test
{
protected:
	size_t linesRead{ 0 };
	EventLog log{ EventLog(linesRead) };
	Event<std::string(int)> eve{ Event<std::string(int)>(
		[](int a) -> std::string { return std::to_string(a); }, "test", log
		) };
	Event<int(void)> noargs{ Event<int(void)>(
		[]() -> int { return 0; }, "test", log
		) };
};

TEST_F(EventTests, CallsCorrectly)
{
	EXPECT_EQ(std::string("4"), eve(4));
	EXPECT_EQ(std::string("123499"), eve(123499));
	EXPECT_EQ(0, noargs());
}

TEST_F(EventTests, CallsLoggedCorrectly)
{
	LogResultType eveCallExpected = {};
	LogResultType noargsCallExpected = {};

	linesRead++;
	(void)eve(6);
	eveCallExpected.push_back(linesRead);
	linesRead++;
	linesRead++;
	(void)eve(6);
	eveCallExpected.push_back(linesRead);

	EXPECT_EQ(eveCallExpected, log.query(eve.eventString()));
	EXPECT_EQ(noargsCallExpected, log.query(noargs.eventString()));

	linesRead += 5;
	noargsCallExpected.push_back(linesRead);
	(void)noargs();

	EXPECT_EQ(noargsCallExpected, log.query(noargs.eventString()));
}
