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
            Event <std::string> event
        );

        explicit Line(
            std::string text
        );

        void readLine(
            std::ostream& stream,
            std::string eventArgs = ""
        );

    public:
        const std::string text;

    private:
        std::optional<Event<std::string> > _event;
    };
} // Scenes
