#include "Serializations.hpp"

#include "pch.h"

namespace nlohmann
{
    Scenes::Line adl_serializer<Scenes::Line>::from_json(const json& j)
    {
        if (j.at("event") == nullptr)
            return {j.at("text")};
        else
            return {j.at("text"), j.at("event"), j.at("arg")};
    }

    void adl_serializer<Scenes::Line>::to_json(json& j, Scenes::Line line)
    {
        j = json({"text", line.text()});
        if (line.eventName().has_value())
            j.push_back({"event", line.eventName().value()});
        else
            j.push_back({"event", nullptr});
        j.push_back({"arg", line.eventArg()});
    }
}