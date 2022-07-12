#include "Section.hpp"

#include <algorithm>
#include <stdexcept>

#include "pch.h"

#include "Event.hpp"
#include "Line.hpp"

namespace Scenes
{
    Section::Condition::Condition(std::string name, std::vector<std::string> arguments)
        : name(std::move(name)), arguments(std::move(arguments)) {}

    Section::Section(
        std::queue<Line> lines, const Log& sceneLogRef, const EventLog& eventLogRef,
        ConditionVector conditions
    )
        : _lines(std::move(lines)), _isChecked(false), _sceneLogRef(sceneLogRef), _eventLogRef(eventLogRef),
          _conditions(std::move(conditions)), _state(false),
          _unaryPredicateMap({
                                 { "expectEqual",
                                   [this](const std::string& eventString) -> bool { return expectEqual(eventString); }},
                                 { "expectLower",
                                   [this](const std::string& eventString) -> bool { return expectLower(eventString); }},
                                 { "expectLowerOrEqual", [this](const std::string& eventString) -> bool
                                 {
                                     return expectLowerOrEqual(eventString);
                                 }},
                                 { "expectHigher", [this](const std::string& eventString) -> bool
                                 {
                                     return expectHigher(eventString);
                                 }},
                                 { "expectHigherOrEqual", [this](const std::string& eventString) -> bool
                                 {
                                     return expectHigherOrEqual(eventString);
                                 }},
                                 { "expectNotEqual", [this](const std::string& eventString) -> bool
                                 {
                                     return expectNotEqual(eventString);
                                 }}
                             }),
          _binaryPredicateMap({
                                  { "triggeredSinceLatestSceneCall",
                                    [this](const std::string& sceneName, const std::string& eventString) -> bool
                                    {
                                        return triggeredSinceLatestSceneCall(sceneName, eventString);
                                    }},
                                  { "notTriggeredSinceLatestSceneCall",
                                    [this](const std::string& sceneName, const std::string& eventString) -> bool
                                    {
                                        return notTriggeredSinceLatestSceneCall(sceneName, eventString);
                                    }},
                                  { "triggeredBeforeLatestSceneCall",
                                    [this](const std::string& sceneName, const std::string& eventString) -> bool
                                    {
                                        return triggeredBeforeLatestSceneCall(sceneName, eventString);
                                    }},
                                  { "notTriggeredBeforeLatestSceneCall",
                                    [this](const std::string& sceneName, const std::string& eventString) -> bool
                                    {
                                        return notTriggeredBeforeLatestSceneCall(sceneName, eventString);
                                    }},

                              }) {}

    void Section::readLine(std::ostream& stream, EventMap& events) noexcept
    {
        _lines.front().readLine(stream, events);
        _lines.pop();
    }

    bool Section::empty() const noexcept
    {
        return _lines.empty();
    }

    Section::CheckResult Section::checkUnaryCondition(const Condition& condition) const noexcept
    {

        auto refUnaryPredicate = _unaryPredicateMap.find(condition.name);
        if (refUnaryPredicate == _unaryPredicateMap.end())
            return CheckResult::InvalidCondition;

        if (condition.arguments.size() != 1)
            return CheckResult::InvalidSize;

        if (!refUnaryPredicate->second(condition.arguments[0]))
            return CheckResult::False;
        return CheckResult::True;
    }

    Section::CheckResult Section::checkBinaryCondition(const Condition& condition) const noexcept
    {

        auto refBinaryPredicate = _binaryPredicateMap.find(condition.name);
        if (refBinaryPredicate == _binaryPredicateMap.end())
            return CheckResult::InvalidCondition;

        if (condition.arguments.size() != 2)
            return CheckResult::InvalidSize;

        if (!refBinaryPredicate->second(condition.arguments[0], condition.arguments[1]))
            return CheckResult::False;

        return CheckResult::True;
    }

    bool Section::isActive() const
    {
        if (this->empty())
            _state = false;

        if (_isChecked)
            return _state;

        _isChecked = true;

        _state = std::ranges::all_of(_conditions, [this](const auto& condition) -> bool
        {
            auto unaryCheck = checkUnaryCondition(condition);
            auto binaryCheck = checkBinaryCondition(condition);
            if (unaryCheck == CheckResult::True || binaryCheck == CheckResult::True)
                return true;
            if (unaryCheck == CheckResult::False || binaryCheck == CheckResult::False)
                return false;
            if (unaryCheck == CheckResult::InvalidSize || binaryCheck == CheckResult::InvalidSize)
                throw std::invalid_argument{ condition.name + " contains an invalid number of arguments." };

            throw std::out_of_range{ condition.name + " is not a valid condition." };
        });

        return _state;
    }

#pragma region Condition functions

    namespace
    {
        [[nodiscard]] std::string getEventName(const std::string& eventString)
        {
            return eventString.substr(0, eventString.find(','));
        }

        [[nodiscard]] int getEventReturn(const std::string& eventString)
        {
            return std::stoi(eventString.substr(eventString.find(',') + 1));
        }
    }
    bool Section::expectEqual(const std::string& eventString)
    {
        return !expectNotEqual(eventString);
    }

    bool Section::expectLower(const std::string& eventString)
    {
        auto name = getEventName(eventString);
        auto expectedReturn = getEventReturn(eventString);

        auto results = _eventLogRef.findKeys(name);
        return std::ranges::any_of(results, [expectedReturn](auto result) -> bool {
           return getEventReturn(result) < expectedReturn;
        });
    }

    bool Section::expectLowerOrEqual(const std::string& eventString)
    {
        auto name = getEventName(eventString);
        auto expectedReturn = getEventReturn(eventString);

        auto results = _eventLogRef.findKeys(name);
        return std::ranges::any_of(results, [expectedReturn](auto result) -> bool {
           return getEventReturn(result) <= expectedReturn;
        });
    }

    bool Section::expectHigher(const std::string& eventString)
    {
        auto name = getEventName(eventString);
        auto expectedReturn = getEventReturn(eventString);

        auto results = _eventLogRef.findKeys(name);
        return std::ranges::any_of(results, [expectedReturn](auto result) -> bool {
            return getEventReturn(result) > expectedReturn;
        });
    }

    bool Section::expectHigherOrEqual(const std::string& eventString)
    {
        auto name = getEventName(eventString);
        auto expectedReturn = getEventReturn(eventString);

        auto results = _eventLogRef.findKeys(name);
        return std::ranges::any_of(results, [expectedReturn](auto result) -> bool {
            return getEventReturn(result) >= expectedReturn;
        });
    }

    bool Section::expectNotEqual(const std::string& eventString)
    {
        return _eventLogRef.query(eventString).empty();
    }

    bool Section::triggeredSinceLatestSceneCall(const std::string& sceneName, const std::string& eventString)
    {
        auto sceneCalls = _sceneLogRef.query(sceneName);
        auto eventCalls =  _eventLogRef.query(eventString);
        if (eventCalls.empty() || sceneCalls.empty())
            return false;
        return eventCalls.back() >= sceneCalls.back();
    }

    bool Section::notTriggeredSinceLatestSceneCall(const std::string& sceneName, const std::string& eventString)
    {
        auto sceneCalls = _sceneLogRef.query(sceneName);
        auto eventCalls =  _eventLogRef.query(eventString);
        if (eventCalls.empty() || sceneCalls.empty())
            return true;
        return eventCalls.back() < sceneCalls.back();
    }

    bool Section::triggeredBeforeLatestSceneCall(const std::string& sceneName, const std::string& eventString)
    {
        auto sceneCalls = _sceneLogRef.query(sceneName);
        auto eventCalls =  _eventLogRef.query(eventString);
        if (eventCalls.empty())
            return false;
        if (sceneCalls.empty())
            return true;

        return eventCalls.back() < sceneCalls.back();
    }

    bool Section::notTriggeredBeforeLatestSceneCall(const std::string& sceneName, const std::string& eventString)
    {
        auto sceneCalls = _sceneLogRef.query(sceneName);
        auto eventCalls =  _eventLogRef.query(eventString);
        if (eventCalls.empty())
            return true;
        if (sceneCalls.empty())
            return false;

        return eventCalls.back() >= sceneCalls.back();
    }

#pragma endregion
} // Scenes