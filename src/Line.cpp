#include "Line.hpp"

#include <string>
#include <utility>

#include "Event.hpp"

namespace Scenes
{

    Line::Line(std::string text, Event <std::string> event, std::string eventArg = "")
        : _text(std::move(text)), _event(std::make_optional<Event<std::string> >(std::move(event))),
          _eventArg(std::move(eventArg))
    {}

    Line::Line(std::string text)
        : _text(std::move(text))
    {}

    void Line::readLine(std::ostream& stream)
    {
        stream << _text;
        if (_event.has_value())
            (*_event)(std::move(_eventArg));
    }

    const std::optional<Event<std::string> >& Line::event()
    {
        return _event;
    }

    const std::string& Line::text() const noexcept
    {
        return _text;
    }

} // Scenes