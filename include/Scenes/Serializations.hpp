#pragma once

#include <nlohmann/json.hpp>

#include "Line.hpp"
#include "Log.hpp"
#include "Section.hpp"

namespace nlohmann
{
    template<>
    struct adl_serializer<Scenes::Line>
    {
        static Scenes::Line from_json(const json& j);
        static void to_json(json& j, Scenes::Line line);
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Scenes::Section::Condition, name, arguments);
}