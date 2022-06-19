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
	Event<std::string(int)> eve{ [](int a) -> std::string { return std::to_string(a); }, "test", log };
	Event<int(void)> noargs{ []() -> int { return 0; }, "test", log };
	Event<void(int)> noreturn{ [](int a) { std::cout << a; }, "test", log };
};

TEST_F(EventTests, CallsCorrectly)
{
	EXPECT_EQ(std::string("4"), eve(4));
	EXPECT_EQ(std::string("123499"), eve(123499));
	EXPECT_EQ(0, noargs());
	noreturn(4);
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
	
	
	LogResultType noargsCallExpected;
	EXPECT_EQ(noargsCallExpected, log.query(noargs.eventString()));

	linesRead += 5;
	noargsCallExpected.push_back(linesRead);
	(void)noargs();
	EXPECT_EQ(noargsCallExpected, log.query(noargs.eventString()));


	LogResultType noreturnCallExpected;
	noreturnCallExpected.push_back(linesRead);
	noreturn(5);
	linesRead++;
	noreturnCallExpected.push_back(linesRead);
	noreturn(8);
	EXPECT_EQ(noreturnCallExpected, log.query(noreturn.eventString()));
}