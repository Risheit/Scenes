#include <sstream>
#include <gtest/gtest.h>

#include "Scenes/Line.hpp"


using namespace Scenes;

class LineTests : public testing::Test
{
protected:
    size_t linesRead{ 1 };
    EventLog log{ linesRead };
    std::string testString = "Test Line";

    EventMap events{
        {"Test Event", Event<std::string>([](const std::string& s) -> int { return stoi(s); },
                                          "Test Event", log)}
    };
};

TEST_F(LineTests, ReadNoEventLine)
{
    Line line{ testString };
    std::stringstream ss;
    line.readLine(ss, events);
    EXPECT_EQ(testString, ss.str());
    EXPECT_TRUE(log.empty()); // No event logged
}

TEST_F(LineTests, ReadEventLine)
{
    Line line{ testString, "Test Event", "10"};
    std::stringstream ss;
    line.readLine(ss, events);
    EXPECT_EQ(testString, ss.str());

    const std::string eventString = line.event(events).value().eventString();
    EXPECT_EQ("Test Event,10", eventString);
    EXPECT_NE(std::vector<size_t>(), log.query(eventString)); // Event logged
}

TEST_F(LineTests, IsMoveConstructible)
{
    EXPECT_TRUE(std::is_move_constructible<Line>::value);
}