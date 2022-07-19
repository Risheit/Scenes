#include "Reader.hpp"

#include <chrono>
#include <exception>
#include <fstream>
#include <thread>
#include <utility>
#include <nlohmann/json.hpp>

#include "Serializations.hpp"
#include "pch.h"

namespace Scenes
{
#pragma region Add Events
    void Reader::addCustomEvent(const std::string& name, const std::function<int(std::string)>& func)
    {
        _events.emplace(name, Event(func, name, _eventLog));
    }

    void Reader::addEvent(const std::string& name, const std::function<int(std::string)>& func)
    {
        addCustomEvent(name, func);
    }

    void Reader::addEvent(const std::string& name, const std::function<void(std::string)>& func)
    {
        std::function<int(std::string)> converted = [func](std::string arg) -> int {
            func(std::move(arg));
            return 0;
        };

        addCustomEvent(name, converted);
    }

    void Reader::addEvent(const std::string& name, const std::function<int(void)>& func)
    {
        std::function<int(std::string)> converted = [func](const std::string&) -> int {
            return func();
        };

        addCustomEvent(name, converted);
    }

    void Reader::addEvent(const std::string& name, const std::function<void(void)>& func)
    {
        std::function<int(std::string)> converted = [func](const std::string&) -> int {
            func();
            return 0;
        };

        addCustomEvent(name, converted);
    }

    template<class Ret>
    void Reader::addEvent(const std::string& name, std::function<Ret(std::string)> func, std::function<int(Ret)> toInt)
    {
        std::function<int(std::string)> converted = [func, toInt](std::string arg) -> int {
            return toInt(func(std::move(arg)));
        };

        addCustomEvent(name, converted);
    }

    template<class Ret>
    void Reader::addEvent(const std::string& name, std::function<Ret(void)> func, std::function<int(Ret)> toInt)
    {
        std::function<int(std::string)> converted = [func, toInt](const std::string&) -> int {
            return toInt(func());
        };

        addCustomEvent(name, converted);
    }

    template<class Arg>
    void Reader::addEvent(const std::string& name, std::function<int(Arg)> func,
                          std::function<Arg(std::string)> fromString)
    {
        std::function<int(std::string)> converted = [func, fromString](std::string arg) -> int {
            return func(fromString(std::move(arg)));
        };

        addCustomEvent(name, converted);
    }

    template<class Arg>
    void Reader::addEvent(const std::string& name, std::function<void(Arg)> func,
                          std::function<Arg(std::string)> fromString)
    {
        std::function<int(std::string)> converted = [func, fromString](std::string arg) -> int {
            func(fromString(std::move(arg)));
            return 1;
        };

        addCustomEvent(name, converted);
    }

    template<class Ret, class Arg>
    void Reader::addEvent(
        const std::string& name, std::function<Ret(Arg)> func, std::function<int(Ret)> toInt,
        std::function<Arg(std::string)> fromString)
    {
        std::function<int(std::string)> converted = [func, toInt, fromString](std::string arg) -> int {
            return ToInt(func(fromString(std::move(arg))));
        };

        addCustomEvent(name, converted);
    }
#pragma endregion

#pragma region File Manipulation


    namespace
    {
        bool isDirectory(const std::filesystem::path& path, const std::string& name)
        {
            auto it = path.end();
            while (!path.empty() && ((it != path.begin() && *it == "") || it == path.end()))
                it--;

            return *it == name;
        }

        std::ifstream openSceneFile(const std::string& fileName, std::filesystem::path containedDirectory)
        {
            containedDirectory /= fileName;
            containedDirectory.replace_extension("json");

            return {containedDirectory};
        }

        void initializeSaveFile(std::filesystem::path& saveDirectory)
        {
            const static std::filesystem::path sceneSave{ "Globals" };
            if (!isDirectory(saveDirectory, "Scenes"))
                saveDirectory /= "Scenes/";

            // Optimize: Replace extension to json to prevent user errors.
            auto sceneFile = openSceneFile(sceneSave, saveDirectory);
        }

        std::string findStartScene(const std::string& saveName, std::filesystem::path saveDirectory)
        {
            auto saveFile = openSceneFile(saveName, std::move(saveDirectory));
            if (!saveFile)
                return "";

            return nlohmann::json::parse(saveFile).value("current scene", "");
        }
    }

    bool Reader::loadScene()
    {
        _sceneLog.addLog(_nextScene);

        auto sceneFile = openSceneFile(_nextScene, _saveLoc);
        if (!sceneFile)
            sceneFile = openSceneFile(_nextScene, _sceneLoc);
        if (!sceneFile)
            return false;

        auto sceneJson = nlohmann::json::parse(sceneFile);
        for (auto& val : sceneJson)
            _scene.emplace(val.value("lines", {}), _sceneLog, _eventLog, val.value("conditions", {}));

        return true;
    }

#pragma endregion

    void Reader::readSectionIfValid(std::ostream& stream)
    {
        using namespace std::chrono_literals;

        static const std::string pauseSignal = createEventString("pause", 1);
        static const std::string stopSignal = createEventString("stop", 1);

        static size_t lastPauseSignal = 0;
        static size_t lastStopSignal = 0;

        while (_scene.front().isActive())
        {
            if (_eventLog.query(pauseSignal).back() > lastPauseSignal)
            {
                lastPauseSignal = _eventLog.query(pauseSignal).back();
                break; // runPause()
            }
            if (_eventLog.query(stopSignal).back() > lastStopSignal)
            {
                lastStopSignal = _eventLog.query(stopSignal).back();
                break; // runStop(), saveScene()
            }

            // Optimize: Figure out how to implement a custom display method
            _scene.front().readLine(stream, _events);
            std::this_thread::sleep_for(0.5s);
        }
    }

    void Reader::read(std::ostream& stream)
    {
        _nextScene = findStartScene(_startScene, _saveLoc);
        if (_nextScene.empty())
            throw std::out_of_range("Entry Scene File " + _startScene + " doesn't exist.");

        bool loadedScene = loadScene();
        do
        {
            _nextScene = "";
            while (loadedScene && !_scene.empty())
            {
                readSectionIfValid(stream);
                _scene.pop();
            }
            loadedScene = loadScene();
        } while (loadedScene);
    }

    Reader::Reader(std::string sceneLoc, std::string saveLoc, std::string startSceneName)
        : _linesRead(0), _eventLog(_linesRead), _sceneLog(_linesRead),
          _sceneLoc(std::move(sceneLoc)), _saveLoc(std::move(saveLoc)), _startScene(std::move(startSceneName)),
          _nextScene(), _events({
              {"", Event<std::string>([](const std::string& s) -> int { return 0; }, "", _eventLog)}
          })
    {
        initializeSaveFile(_saveLoc);
    }

    Reader::Reader(std::string sceneLoc, std::string saveLoc)
        : Reader(std::move(sceneLoc), std::move(saveLoc), "Opening")
    {}
} // Scenes