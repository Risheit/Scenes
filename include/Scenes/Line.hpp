#pragma once
#include <string>
#include <optional>

#include "Event.hpp"

namespace Scenes
{
    class Line
    {
    public:
        Line(
            std::string text,
            Event <std::string> event,
            std::string eventArg
        );

        explicit Line(
            std::string text
        );

        void readLine(
            std::ostream& stream
        );

        [[nodiscard]] const std::optional<Event<std::string> >& event();
    public:
        const std::string text;

    private:
        std::optional<Event<std::string> > _event;
        std::string _eventArg;
    };
} // Scenes
