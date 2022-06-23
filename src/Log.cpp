#include "Log.hpp"

#include "pch.h"


namespace Scenes
{
	Log::Log(const size_t& linesRead)
		: _log(LogType()), _linesRead(linesRead)
	{}

	void Log::addLog(const LogNameType& name)
	{
		_log[name].push_back(static_cast<unsigned int>(_linesRead));
	}

	/// <summary>
	///		Query the call times of a stored log name.
	/// </summary>
	/// <returns>A Vector of call times if query succeeds, empty vector if fails.</returns>
	LogResultType Log::query(const LogNameType& name) const
	{
		const auto it = _log.find(name);

		if (it == _log.end())
			return {};
		return it->second;
	}


	/// <summary>
	///		Find all keys containing a given search term.
	/// </summary>
	/// <returns>A vector of keys if search succeeds, empty vector if none found.</returns>
	std::vector<LogNameType> Log::findKeys(const LogNameType& searchTerm) const
	{
		std::vector<LogNameType> results;

		for (const auto& key : _log | std::views::keys)
			if (key.find(searchTerm) != std::string::npos)
				results.push_back(key);

		return results;
	}
} // Scenes
