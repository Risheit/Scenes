/**
 * @file Section.hpp
 * @brief Contains the Section class along with relevant types and functions.
 */

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
    /**
     * @brief Contains a queue of lines that are read depending on activity conditions.
     *
     * A Section class is used by Scenes to allow for dynamic script reading and control flow. A Section provides three
     * functions to correctly read its contents. The first two, @c empty() and @c isActive() exist to verify that a
     * Section is ready for reading, while @c readLine() prints the output of the first single line before removing it
     * from the Section queue.
     *
     * The Section class does not define any control flow behaviours itself to allow the control flow of Section reading
     * to be defined entirely by the user. A simple Section reading may look like:
     * @code
     * while (section.isActive())
     *     section.readLine(std::cout);
     * @endcode
     * @remark @c isActive() calls @c empty() when determining a Section's state, so calling @c empty() alongside
     * @c isActive() is unnecessary.
     */
    class Section
    {
    public:
        /**
         * @brief Holds a Section condition's name and the arguments it will take.
         */
        struct Condition
        {
            std::string name; //!< This Condition's name
            std::vector<std::string> arguments; //!< The arguments taken by this Condition.

            /**
             * @brief Initializes a new instance of the Condition struct.
             * @param name This Condition's name.
             * @param arguments The arguments taken by this Condition.
             */
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

        /**
         * @internal Values used to check Condition results.
         */
        enum class CheckResult
        {
            False,
            True,
            InvalidSize, //!< @internal The Condition has too many arguments for its predicate type.
            InvalidCondition //!< @internal Condition isn't found in a Condition map.
        };

        [[nodiscard]] CheckResult checkUnaryCondition(
            const Condition& condition
        ) const noexcept;

        [[nodiscard]] CheckResult checkBinaryCondition(
            const Condition& condition
        ) const noexcept;

        /**
         * @brief Check to see if a given eventString has already been logged.
         * 
         * @param eventString The eventString to check.
         * @return True if one or more logs are found, false otherwise.
         */
        [[nodiscard]] bool expectEqual(
            const std::string& eventString
        );

        /**
         * @brief Check to see if this Event has already been logged with a lower result.
         * 
         * @param eventString The eventString to check.
         * @return True if one or more logs are found, false otherwise.
         */
        [[nodiscard]] bool expectLower(
            const std::string& eventString
        );

        /**
         * @brief Check to see if this Event has already been logged with a lower or equal result.
         * 
         * @param eventString The eventString to check.
         * @return True if one or more logs are found, false otherwise.
         */
        [[nodiscard]] bool expectLowerOrEqual(
            const std::string& eventString
        );

        /**
         * @brief Check to see if this Event has already been logged with a higher result.
         * 
         * @param eventString The eventString to check.
         * @return True if one or more logs are found, false otherwise.
         */
        [[nodiscard]] bool expectHigher(
            const std::string& eventString
        );

        /**
         * @brief Check to see if this Event has already been logged with a higher or equal result.
         * 
         * @param eventString The eventString to check.
         * @return True if one or more logs are found, false otherwise.
         */
        [[nodiscard]] bool expectHigherOrEqual(
            const std::string& eventString
        );

        /**
         * @brief Check to see if this Event hasn't been logged with an equal result.
         *
         * @param eventString The eventString to check.
         * @return True if no logs are found, false otherwise.
         */
        [[nodiscard]] bool expectNotEqual(
            const std::string& eventString
        );

        /**
         * @brief Check to see if this Event has been recorded since the latest record of a given Scene.
         * 
         * @param sceneName The name of the Scene to check.
         * @param eventString The eventString to check.
         * @return True if the latest Event record >= the latest Scene record, false otherwise.
         * Returns false if the Event or Scene has yet to be recorded, no matter what.
         */
        [[nodiscard]] bool triggeredSinceLatestSceneCall(
            const std::string& sceneName,
            const std::string& eventString
        );

        /**
         * @brief Check to see if this Event hasn't been recorded since the latest record of a given Scene.
         * 
         * @param sceneName The name of the Scene to check.
         * @param eventString The eventString to check.
         * @return True if the latest Event record < the latest Scene record, false otherwise.
         * Returns true if this Event or Scene has yet to be recorded, no matter what.
         */
        [[nodiscard]] bool notTriggeredSinceLatestSceneCall(
            const std::string& sceneName,
            const std::string& eventString
        );

        /**
         * @brief Check to see if this Event has been recorded before the latest record of a given Scene.
         * 
         * @param sceneName The name of the Scene to check.
         * @param eventString The eventString to check.
         * @return True if the latest Event record < the latest Scene record, false otherwise.
         * Returns true if the given Scene hasn't been recorded, unless this Event hasn't been recorded either, in
         * which case it returns false.
         */
        [[nodiscard]] bool triggeredBeforeLatestSceneCall(
            const std::string& sceneName,
            const std::string& eventString
        );

        /**
         * @brief Check to see if this Event hasn't been recorded before the latest record of a given Scene.
         *
         * @param sceneName The name of the Scene to check.
         * @param eventString The eventString to check.
         * @return True if the latest Event record >= the latest Scene record, false otherwise.
         * Returns false if the given Scene hasn't been recorded, unless this Event hasn't been recorded either, in
         * which case it returns true.
         */
        [[nodiscard]] bool notTriggeredBeforeLatestSceneCall(
            const std::string& sceneName,
            const std::string& eventString
        );


    public:
        /**
         * @brief Initializes a new instance of the Section class.
         *
         * @param lines The queue of Lines this Section can read through.
         * @param sceneLogRef A reference to a Log holding currently recorded Scenes.
         * @param eventLogRef A reference to an eventLog.
         * @param conditions The Conditions that need to be true for this Section to be active.
         */
        Section(
            std::queue<Line> lines,
            const Log& sceneLogRef,
            const EventLog& eventLogRef,
            ConditionVector conditions
        );

        /**
         * @brief Checks if this Section is active or not.
         *
         * To determine the state of this Section, this function checks whether or not this Section's Line
         * queue is empty, and then checks the conditions of this Section. If the queue is empty and all the conditions
         * are true, then the Section is considered active.
         *
         * @remark As the Conditions tied to this Section are static and decided during this Section's construction,
         * after the first call to this function, the function will no longer check this Section's conditions, instead
         * echoing the results of the initial checks. This prevents excess work during repeated calls to this function.
         *
         * @return True if this Section is Active, false otherwise.
         */
        [[nodiscard]] bool isActive() const;

        /**
         * @brief Prints the first Line contained in this Section's Line queue to an output stream, then pops it.

         * @copydetails Scenes::Line::readLine(std::ostream&)
         *
         * @warning Reading from an empty queue causes undefined behaviour, so always check @c empty() or @c isActive()
         * before calling @c readLine().
         */
        void readLine(
            std::ostream& stream
        ) noexcept;

        /**
         * @brief Checks if this Section's Line queue is empty.
         *
         * @return True if the queue is empty, false if it isn't.
         */
        [[nodiscard]] bool empty() const noexcept;

    private:
        std::queue<Line> _lines; //!< The Lines to be read.
        const Log& _sceneLogRef; //!< A reference to a log of Scenes.
        const EventLog& _eventLogRef; //!< A reference to a log of Events.
        const ConditionVector _conditions; //!< The conditions required to activate this Section.

        mutable bool _isChecked; //!< @internal Whether this Section's conditions have already been checked.
        mutable bool _state; //!< @internal The current activity state of this Section.
        const UnaryPredicateMap _unaryPredicateMap; //!< @internal Maps between unary condition names to their
                                                    //!< corresponding predicates.
        const BinaryPredicateMap _binaryPredicateMap; //!< @internal Maps between binary condition names to their
                                                      //!< corresponding predicates.
    };
} // Scenes
