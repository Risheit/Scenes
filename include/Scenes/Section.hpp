#pragma once

#include <queue>
#include <string>
#include <unordered_map>

#include "Event.hpp"
#include "Line.hpp"

namespace Scenes
{
    using EventMap = std::unordered_map<const std::string, Event<std::string> >;

    class Section
    {
    public:
        Section(
            std::queue<Line>  lines,
            const EventMap& eventMapRef
        );

        void readLine(
            std::ostream& stream
        );



    private:
        std::queue<Line> _lines;
        bool _isChecked;
        const EventMap& _eventMapRef;
    };
} // Scenes
