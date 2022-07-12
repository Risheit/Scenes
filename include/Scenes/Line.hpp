/**
 * @file Line.hpp
 * @brief Contains the Line class along with relevant types and functions.
 */

#pragma once
#include <string>
#include <optional>

#include "Event.hpp"

namespace Scenes
{
    using EventMap = std::unordered_map<std::string, Event<std::string> >;

    /**
     * @brief Contains a text string and an associated Event that is run when the string is read.
     */
    class Line
    {
    public:

        /**
         * @brief Initializes an instance of the Line class.
         *
         * @param text The text contained by this Line.
         * @param eventName The name of the event this Line can call.
         * @param eventArg The argument the event will taken. Should be "" if the event takes no arguments.
         */
        Line(
            std::string text,
            std::string eventName,
            std::string eventArg
        );

        /**
         * @brief Initializes an instance of the Line class without an Event.
         * @param text The text contained by this line.
         */
        explicit Line(
            std::string text
        );

        /**
         * @brief Prints this Line's text to a output stream and runs its Event if it exists in the given Event Map.
         * @param stream The output steam that is printed to.
         */
        void readLine(
            std::ostream& stream,
            EventMap& events
        );

        /**
         * @brief Gets this Line's possibly contained Event name.
         * @return An optional that may or may not contain the Event name of this line.
         */
        [[nodiscard]] const std::optional<std::string>& eventName() const noexcept;

        /**
         * @brief Gets this Line's possibly contained Event.
         *
         * References the given Event Map to find which Event this Line's contained Event name references.
         * If the Event Map doesn't contain the found event, returns an Optional Event with no event stored.
         *
         * @param events The Event Map to query an event for.
         * @return An optional that may or may not contain the Event of this line.
         */
        [[nodiscard]] std::optional<Event<std::string> > event(
          const EventMap& events
        ) const noexcept;

        /**
         * @brief Gets this Line's event argument.
         * @return A string containing the contained event's argument, and "" if that argument doesn't exist.
         */
        [[nodiscard]] const std::string& eventArg() const noexcept;

        /**
         * @brief Gets this Line's contained text.
         * @return A string of this Line's contained text.
         */
        [[nodiscard]] const std::string& text() const noexcept;

        bool operator==(const Line& rhs) const;

        bool operator!=(const Line& rhs) const;

    private:
        const std::string _text; //!< The text this line contains.

    private:
        std::optional<std::string> _eventName; //!< An optional that may or may not contain this Line's Event.
        std::string _eventArg; //!< The event argument _event takes. Is undefined if the Event doesn't exist.
    };
} // Scenes
