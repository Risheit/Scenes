#include "pch.h"
#include "EventLog.h"
#include <map>

namespace Scenes
{
	EventLog::EventLog(const size_t& linesRead)
		: _log(LogType()), _linesRead(linesRead)
	{}

	void EventLog::addLog(LogNameType eventString)
	{
		_log[eventString].push_back(static_cast<unsigned int>(_linesRead));
	}

	/// <summary>
	///		Query the call times of a stored log name.
	/// </summary>
	/// <returns>Vector of call times if query succeeds, empty vector if fails.</returns>
	LogResultType EventLog::query(LogNameType eventString) const
	{
		auto it = _log.find(eventString);

		if (it == _log.end())
			return std::vector<size_t>();

		return it->second;
	}

	/// <summary>
	///		Find log names where a given search term occurs somewhere within the name.
	/// </summary>
	/// <returns>Vector of contained log name containing search term.</returns>
	std::vector<LogNameType> EventLog::findEventCalls(LogNameType searchTerm) const
	{
		std::vector<LogNameType> results;

		for (const auto &it : _log)
			if (it.first.contains(searchTerm))
				results.push_back(it.first);

		return results;
	}
}