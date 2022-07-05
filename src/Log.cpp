#include "Log.hpp"

#include "pch.h"


namespace Scenes
{
	Log::Log(const size_t& linesRead)
		: _log(LogType()), _linesRead(linesRead)
	{}

	void Log::addLog(const LogNameType& name)
	{
		_log[name].push_back(_linesRead);
	}

	LogResultType Log::query(const LogNameType& name) const
	{
		const auto it = _log.find(name);

		if (it == _log.end())
			return {};
		return it->second;
	}

	std::vector<LogNameType> Log::findKeys(const LogNameType& searchTerm) const
	{
		std::vector<LogNameType> results;

		for (const auto& key : _log | std::views::keys)
			if (key.find(searchTerm) != std::string::npos)
				results.push_back(key);

		return results;
	}

    bool Log::empty() const noexcept
    {
        return _log.empty();
    }
} // Scenes
