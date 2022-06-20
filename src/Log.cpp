#include "Log.h"

namespace Scenes
{
	Log::Log(const size_t& linesRead)
		: _log(LogType()), _linesRead(linesRead)
	{}

	void Log::addLog(LogNameType eventString)
	{
		_log[eventString].push_back(static_cast<unsigned int>(_linesRead));
	}

	/// <summary>
	///		Query the call times of a stored log name.
	/// </summary>
	/// <returns>A Vector of call times if query succeeds, empty vector if fails.</returns>
	LogResultType Log::query(LogNameType eventString) const
	{
		auto it = _log.find(eventString);

		if (it == _log.end())
			return std::vector<size_t>();

		return it->second;
	}


	/// <summary>
	///		Find all keys containing a given search term.
	/// </summary>
	/// <returns>A vector of keys if search succeeds, empty vector if none found.</returns>
	std::vector<LogNameType> Log::findKeys(LogNameType searchTerm) const
	{
		std::vector<LogNameType> results;

		for (const auto& it : _log)
			if (it.first.contains("searchTerm"))
				results.push_back(it.first);

		return results;
	}
}