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
};

TEST_F(LineTests, ReadNoEventLine)
{
    Line line{ testString };
    std::stringstream ss;
    line.readLine(ss);
    EXPECT_EQ(testString, ss.str());
    EXPECT_TRUE(log.empty()); // No event logged
}

TEST_F(LineTests, ReadEventLine)
{
    Line line{ testString, Event<std::string>([](const std::string& s) { return stoi(s); },
                                              "Test Event", log) };
    std::stringstream ss;
    line.readLine(ss, "10");
    EXPECT_EQ(testString, ss.str());

    const std::string eventString = line.event().value().eventString();
    EXPECT_EQ("Test Event,10", eventString);
    EXPECT_NE(std::vector<size_t>(), log.query(eventString)); // Event logged
}