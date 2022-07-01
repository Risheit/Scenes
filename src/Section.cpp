#include "Section.hpp"

#include "pch.h"

#include "Event.hpp"
#include "Line.hpp"

namespace Scenes
{
    Section::Section(std::queue<Line>  lines, const EventMap& eventMapRef)
        : _lines(std::move(lines)), _isChecked(false), _eventMapRef(eventMapRef)
    {}

    void Section::readLine(std::ostream& stream)
    {
        _lines.front().readLine(stream);
        _lines.pop();
    }
} // Scenes