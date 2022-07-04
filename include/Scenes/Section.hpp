#pragma once

#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "EventLog.hpp"
#include "Line.hpp"

namespace Scenes
{
    class Section
    {
    public:
        struct Condition
        {
            std::string name;
            std::vector<std::string> arguments;

            Condition(
                std::string name,
                std::vector<std::string> arguments
            );
        };

        using ConditionVector = std::vector<Condition>;

    private:
        using UnaryPredicateMap = std::unordered_map<std::string, std::function<bool(const std::string&)> >;
        using BinaryPredicateMap = std::unordered_map<std::string,
                                                      std::function<bool(const std::string&, const std::string&)> >;

        enum class CheckResult
        {
            False,
            True,
            InvalidSize,
            InvalidCondition
        };

        [[nodiscard]] CheckResult checkUnaryCondition(
            const Condition& condition
        ) const noexcept;

        [[nodiscard]] CheckResult checkBinaryCondition(
            const Condition& condition
        ) const noexcept;

        // Conditions
        [[nodiscard]] bool expectEqual(
            const std::string& eventString
        );

        [[nodiscard]] bool expectLower(
            const std::string& eventString
        );

        [[nodiscard]] bool expectLowerOrEqual(
            const std::string& eventString
        );

        [[nodiscard]] bool expectHigher(
            const std::string& eventString
        );

        [[nodiscard]] bool expectHigherOrEqual(
            const std::string& eventString
        );

        [[nodiscard]] bool expectNotEqual(
            const std::string& eventString
        );

        [[nodiscard]] bool triggeredSinceScene(
            const std::string& sceneName,
            const std::string& eventString
        );

        [[nodiscard]] bool notTriggeredSinceScene(
            const std::string& sceneName,
            const std::string& eventString
        );

        [[nodiscard]] bool triggeredBeforeScene(
            const std::string& sceneName,
            const std::string& eventString
        );

        [[nodiscard]] bool notTriggeredBeforeScene(
            const std::string& sceneName,
            const std::string& eventString
        );


    public:
        Section(
            std::queue<Line> lines,
            const Log& sceneLogRef,
            const EventLog& eventLogRef,
            ConditionVector conditions
        );

        [[nodiscard]] bool isActive() const;

        void readLine(
            std::ostream& stream
        ) noexcept;

        [[nodiscard]] bool empty() const noexcept;

    private:
        std::queue<Line> _lines;
        mutable bool _isChecked;
        mutable bool _state;
        const Log& _sceneLogRef;
        const EventLog& _eventLogRef;

        const ConditionVector _conditions;
        const UnaryPredicateMap _unaryPredicateMap;
        const BinaryPredicateMap _binaryPredicateMap;
    };
} // Scenes
