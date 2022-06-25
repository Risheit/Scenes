#include "Line.hpp"

#include <string>
#include <utility>

#include "Event.hpp"

namespace Scenes
{

    Line::Line(std::string text, Event<std::string> event)
        : text(std::move(text)),
          _event(std::make_optional<Event<std::string> >(std::move(event)))
    {}

    Line::Line(std::string text)
        : text(std::move(text))
    {}

    void Line::readLine(std::ostream& stream, std::string eventArg)
    {
        stream << text;
        if (_event.has_value())
            (*_event)(std::move(eventArg));
    }

    const std::optional<Event<std::string> >& Line::event()
    {
        return _event;
    }
} // Scenes