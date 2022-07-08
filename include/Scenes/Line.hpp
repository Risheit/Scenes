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
    /**
     * @brief Contains a text string and an associated Event that is run when the string is read.
     */
    class Line
    {
    public:

        /**
         * @brief Initializes an instance of the Line class.
         *
         * @param text The text contained by this line.
         * @param event The event contained by this line.
         * @param eventArg The argument the event will taken. Should be "" if the event takes no arguments.
         */
        Line(
            std::string text,
            Event <std::string> event,
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
         * @brief Prints this Line's text to a output stream and runs its Event if it exists.
         * @param stream The output steam that is printed to.
         */
        void readLine(
            std::ostream& stream
        );

        /**
         * @brief Gets this Line's possibly contained Event.
         * @return An optional that may or may not contain an Event.
         */
        [[nodiscard]] const std::optional<Event<std::string> >& event() const noexcept;

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
        std::optional<Event<std::string> > _event; //!< An optional that may or may not contain this Line's Event.
        std::string _eventArg; //!< The event argument _event takes. Is undefined if the Event doesn't exist.
    };
} // Scenes
