#include <gtest/gtest.h>
#include <stdexcept>
#include <utility>

#include "Scenes/Section.hpp"

using namespace Scenes;

class SectionTests : public testing::Test
{
protected:
    size_t linesRead{ 10 };
    Log sceneLog{ linesRead };
    EventLog eventLog{ linesRead };
    const std::deque<Line> lineQueue{
        { Line("Line 1"), Line("Line 2"), Line("Line 3"), Line("Line 4"), Line("Line 5") }
    };
    EventMap events{};

    SectionTests()
    {
        sceneLog.addLog("Example Scene");
        eventLog.addLog(createEventString("Example Event", 2));

        linesRead += 5;
        sceneLog.addLog("Example Scene");
        eventLog.addLog(createEventString("Example Event", 2));

        linesRead++;
        sceneLog.addLog("Example Scene");

        linesRead++;
        sceneLog.addLog("Example Scene 2");
        eventLog.addLog(createEventString("Example Event", 5));
    }

    Section createTestSection(Section::ConditionVector conditions)
    {
        return {
            std::queue<Line>(lineQueue),
            sceneLog,
            eventLog,
            std::move(conditions)
        };
    }

    std::deque<Line> sectionReadResult(Section section)
    {
        std::stringstream ss;
        std::deque<Line> actual{};

        while (section.isActive())
        {
            section.readLine(ss, events);
            actual.emplace_back(ss.str());
            ss.str("");
        }

        return actual;
    }
};

TEST_F(SectionTests, TestSectionWithNoConditions)
{
    Section section = createTestSection({});

    EXPECT_EQ(sectionReadResult(section), lineQueue);
}

TEST_F(SectionTests, TestSectionWithInvalidCondition)
{
    Section section = createTestSection(
        { Section::Condition("invalidCondition", { createEventString("Example Event", 2) }) }
    );

    EXPECT_THROW(sectionReadResult(section), std::out_of_range);
}

TEST_F(SectionTests, TestSectionWithEqualityCondition)
{
    Section success = createTestSection(
        { Section::Condition("expectEqual", { createEventString("Example Event", 2) }) }
    );

    EXPECT_EQ(sectionReadResult(success), lineQueue);

    Section failure = createTestSection(
        { Section::Condition("expectEqual", { createEventString("Example Event", 8) }) }
    );

    EXPECT_EQ(sectionReadResult(failure), std::deque<Line>());

    Section invalid_size{
        std::queue<Line>(lineQueue),
        sceneLog,
        eventLog,
        { Section::Condition("expectEqual",
                             { createEventString("Example Event", 2),
                               createEventString("Example Event", 2) }) }
    };

    EXPECT_THROW(sectionReadResult(invalid_size), std::invalid_argument);
}

TEST_F(SectionTests, TestSectionWithInequalityCondition)
{
    Section success = createTestSection(
        { Section::Condition("expectNotEqual", { createEventString("Example Event", 9) }) }
    );

    EXPECT_EQ(sectionReadResult(success), lineQueue);

    Section failure = createTestSection(
        { Section::Condition("expectNotEqual", { createEventString("Example Event", 2) }) }
    );

    EXPECT_EQ(sectionReadResult(failure), std::deque<Line>());

    Section invalid_size{
        std::queue<Line>(lineQueue),
        sceneLog,
        eventLog,
        { Section::Condition("expectNotEqual",
                             { createEventString("Example Event", 2),
                               createEventString("Example Event", 2) }) }
    };

    EXPECT_THROW(sectionReadResult(invalid_size), std::invalid_argument);
}

TEST_F(SectionTests, TestSectionWithLowerThanCondition)
{
    Section success = createTestSection(
        { Section::Condition("expectLower", { createEventString("Example Event", 5) }) }
    );

    EXPECT_EQ(sectionReadResult(success), lineQueue);

    Section failure = createTestSection(
        { Section::Condition("expectLower", { createEventString("Example Event", 2) }) }
    );

    EXPECT_EQ(sectionReadResult(failure), std::deque<Line>());

    Section invalid_size{
        std::queue<Line>(lineQueue),
        sceneLog,
        eventLog,
        { Section::Condition("expectLower",
                             { createEventString("Example Event", 5),
                               createEventString("Example Event", 5) }) }
    };

    EXPECT_THROW(sectionReadResult(invalid_size), std::invalid_argument);
}

TEST_F(SectionTests, TestSectionWithLowerThanOrEqualCondition)
{
    Section success = createTestSection(
        { Section::Condition("expectLowerOrEqual", { createEventString("Example Event", 2) }) }
    );

    EXPECT_EQ(sectionReadResult(success), lineQueue);

    Section failure = createTestSection(
        { Section::Condition("expectLowerOrEqual", { createEventString("Example Event", 1) }) }
    );

    EXPECT_EQ(sectionReadResult(failure), std::deque<Line>());

    Section invalid_size{
        std::queue<Line>(lineQueue),
        sceneLog,
        eventLog,
        { Section::Condition("expectLowerOrEqual",
                             { createEventString("Example Event", 2),
                               createEventString("Example Event", 2) }) }
    };

    EXPECT_THROW(sectionReadResult(invalid_size), std::invalid_argument);
}

TEST_F(SectionTests, TestSectionWithGreaterThanCondition)
{
    Section success = createTestSection(
        { Section::Condition("expectHigher", { createEventString("Example Event", 2) }) }
    );

    EXPECT_EQ(sectionReadResult(success), lineQueue);

    Section failure = createTestSection(
        { Section::Condition("expectHigher", { createEventString("Example Event", 5) }) }
    );

    EXPECT_EQ(sectionReadResult(failure), std::deque<Line>());

    Section invalid_size{
        std::queue<Line>(lineQueue),
        sceneLog,
        eventLog,
        { Section::Condition("expectHigher",
                             { createEventString("Example Event", 2),
                               createEventString("Example Event", 2) }) }
    };

    EXPECT_THROW(sectionReadResult(invalid_size), std::invalid_argument);
}

TEST_F(SectionTests, TestSectionWithGreaterThanOrEqualCondition)
{
    Section success = createTestSection(
        { Section::Condition("expectHigherOrEqual", { createEventString("Example Event", 5) }) }
    );

    EXPECT_EQ(sectionReadResult(success), lineQueue);

    Section failure = createTestSection(
        { Section::Condition("expectHigherOrEqual", { createEventString("Example Event", 6) }) }
    );

    EXPECT_EQ(sectionReadResult(failure), std::deque<Line>());

    Section invalid_size{
        std::queue<Line>(lineQueue),
        sceneLog,
        eventLog,
        { Section::Condition("expectHigherOrEqual",
                             { createEventString("Example Event", 2),
                               createEventString("Example Event", 2) }) }
    };

    EXPECT_THROW(sectionReadResult(invalid_size), std::invalid_argument);
}

TEST_F(SectionTests, TestSectionWithTriggeredSinceSceneCondition)
{
    Section success = createTestSection(
        { Section::Condition("triggeredSinceLatestSceneCall", { "Example Scene", createEventString("Example Event", 5) }) }
    );

    EXPECT_EQ(sectionReadResult(success), lineQueue);

    Section failure = createTestSection(
        { Section::Condition("triggeredSinceLatestSceneCall", { "Example Scene", createEventString("Example Event", 2) }) }
    );

    EXPECT_EQ(sectionReadResult(failure), std::deque<Line>());

    Section invalid_size{
        std::queue<Line>(lineQueue),
        sceneLog,
        eventLog,
        { Section::Condition("triggeredSinceLatestSceneCall", { createEventString("Example Event", 2) }) }
    };

    EXPECT_THROW(sectionReadResult(invalid_size), std::invalid_argument);
}

TEST_F(SectionTests, TestSectionWithNotTriggeredSinceSceneCondition)
{
    Section success = createTestSection(
        { Section::Condition("notTriggeredSinceLatestSceneCall", { "Example Scene 2", createEventString("Example Event", 2) }) }
    );

    EXPECT_EQ(sectionReadResult(success), lineQueue);

    Section failure = createTestSection(
        { Section::Condition("notTriggeredSinceLatestSceneCall", { "Example Scene 2", createEventString("Example Event", 5) }) }
    );

    EXPECT_EQ(sectionReadResult(failure), std::deque<Line>());

    Section invalid_size{
        std::queue<Line>(lineQueue),
        sceneLog,
        eventLog,
        { Section::Condition("notTriggeredSinceLatestSceneCall", { createEventString("Example Event", 2) }) }
    };

    EXPECT_THROW(sectionReadResult(invalid_size), std::invalid_argument);
}

TEST_F(SectionTests, TestSectionWithTriggeredBeforeSceneCondition)
{
    Section success = createTestSection(
        { Section::Condition("triggeredBeforeLatestSceneCall", { "Unreached Scene", createEventString("Example Event", 2) }) }
    );

    EXPECT_EQ(sectionReadResult(success), lineQueue);

    Section failure = createTestSection(
        { Section::Condition("triggeredBeforeLatestSceneCall", { "Example Scene", createEventString("Uncalled Event", 1) }) }
    );

    EXPECT_EQ(sectionReadResult(failure), std::deque<Line>());

    Section invalid_size{
        std::queue<Line>(lineQueue),
        sceneLog,
        eventLog,
        { Section::Condition("triggeredBeforeLatestSceneCall", { createEventString("Example Event", 2) }) }
    };

    EXPECT_THROW(sectionReadResult(invalid_size), std::invalid_argument);
}

TEST_F(SectionTests, TestSectionWithNotTriggeredBeforeSceneCondition)
{
    Section success = createTestSection(
        { Section::Condition("notTriggeredBeforeLatestSceneCall", { "Example Scene", createEventString("Uncalled Event", 1) }) }
    );

    EXPECT_EQ(sectionReadResult(success), lineQueue);

    Section failure = createTestSection(
        { Section::Condition("notTriggeredBeforeLatestSceneCall", { "Uncalled Scene", createEventString("Example Event", 2) }) }
    );

    EXPECT_EQ(sectionReadResult(failure), std::deque<Line>());

    Section invalid_size{
        std::queue<Line>(lineQueue),
        sceneLog,
        eventLog,
        { Section::Condition("notTriggeredBeforeLatestSceneCall", { createEventString("Example Event", 2) }) }
    };

    EXPECT_THROW(sectionReadResult(invalid_size), std::invalid_argument);
}

TEST_F(SectionTests, TestSectionWithMultipleConditions)
{
    Section section = createTestSection(
        {
            Section::Condition("notTriggeredBeforeLatestSceneCall", { "Example Scene", createEventString("Uncalled Event", 1) }),
            Section::Condition("expectHigherOrEqual", { createEventString("Example Event", 5) })
        }
    );

    EXPECT_EQ(sectionReadResult(section), lineQueue);
}

TEST_F(SectionTests, IsMoveConstructible)
{
    EXPECT_TRUE(std::is_move_constructible<Section>::value);
}