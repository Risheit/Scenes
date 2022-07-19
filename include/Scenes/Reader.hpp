#pragma once

#include <filesystem>
#include <functional>
#include <ostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "Event.hpp"
#include "EventLog.hpp"
#include "Log.hpp"
#include "Section.hpp"

namespace Scenes
{
    class Reader
    {
#pragma region Add Events
    private:
        void addCustomEvent(
            const std::string& name,
            const std::function<int(std::string)>& func
        );

    public:

        void addEvent(
            const std::string& name,
            const std::function<int(std::string)>& func
        );

        void addEvent(
            const std::string& name,
            const std::function<void(std::string)>& func
        );

        void addEvent(
            const std::string& name,
            const std::function<int(void)>& func
        );

        void addEvent(
            const std::string& name,
            const std::function<void(void)>& func
        );

        template<class Ret>
        void addEvent(
            const std::string& name,
            std::function<Ret(std::string)> func,
            std::function<int(Ret)> toInt
        );

        template<class Ret>
        void addEvent(
            const std::string& name,
            std::function<Ret(void)> func,
            std::function<int(Ret)> toInt
        );

        template<class Arg>
        void addEvent(
            const std::string& name,
            std::function<int(Arg)> func,
            std::function<Arg(std::string)> fromString
        );

        template<class Arg>
        void addEvent(
            const std::string& name,
            std::function<void(Arg)> func,
            std::function<Arg(std::string)> fromString
        );

        template<class Ret, class Arg>
        void addEvent(
            const std::string& name,
            std::function<Ret(Arg)> func,
            std::function<int(Ret)> toInt,
            std::function<Arg(std::string)> fromString
        );
#pragma endregion

    private:
        bool loadScene();
        bool saveScene();
        void readSectionIfValid(
            std::ostream& stream
        );

    public:
        void read(
            std::ostream& stream
        );

        Reader(
            std::string sceneLoc,
            std::string saveLoc,
            std::string startSceneName
        );

        Reader(
            std::string sceneLoc,
            std::string saveLoc
        );

        // TODO: Save Reader to saveLoc.
        //  Save section parts individually
        //  Save position in which Scene File
        //  Save Event/Scene Logs

    private:
        size_t _linesRead; //!< The lines read so far into the game.
        EventLog _eventLog; //!< A log of recorded events.
        Log _sceneLog; //!< A log of recorded Scenes.
        std::queue<Section> _scene; //!< Queue of sections that Reader reads through.

        const std::filesystem::path _sceneLoc; //!< The path to the directory of the default Scene files to query.
        std::filesystem::path _saveLoc; //!< The path that Reader can save to.
        const std::string _startScene; //!< The name of the scene that Reader will open first.
        std::string _nextScene; //!< The name of the next Scene file to read.

        EventMap _events; //!< Map between user-defined Event names to the Events themselves.

    };
} // Scenes