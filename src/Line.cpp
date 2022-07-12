#include "Line.hpp"

#include "pch.h"

#include "Event.hpp"

namespace Scenes
{

    Line::Line(std::string text, std::string eventName, std::string eventArg = "")
        : _text(std::move(text)), _eventName(std::make_optional<std::string>(std::move(eventName))),
          _eventArg(std::move(eventArg))
    {}

    Line::Line(std::string text)
        : _text(std::move(text))
    {}

    void Line::readLine(std::ostream& stream, EventMap& events)
    {
        stream << _text;
        if (_eventName.has_value())
            try
            {
                (events.at(_eventName.value()))(_eventArg);
            } catch (std::out_of_range&) {}
    }

    const std::optional<std::string>& Line::eventName() const noexcept
    {
        return _eventName;
    }



    const std::string& Line::text() const noexcept
    {
        return _text;
    }

    std::optional<Event<std::string>> Line::event(const EventMap& events) const noexcept
    {
        if (!_eventName)
            return {};

        auto event =  events.find(_eventName.value());
        return event == events.end() ? std::optional<Event<std::string> >() : std::make_optional(event->second);
    }

    bool Line::operator==(const Line& rhs) const
    {
        return _text == rhs._text &&
               _eventName == rhs._eventName &&
               _eventArg == rhs._eventArg;
    }

    bool Line::operator!=(const Line& rhs) const
    {
        return !(rhs == *this);
    }

    const std::string& Line::eventArg() const noexcept
    {
        return _eventArg;
    }
} // Scenes