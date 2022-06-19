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
	/// <returns>A Vector of call times if query succeeds, empty vector if fails.</returns>
	LogResultType EventLog::query(LogNameType eventString) const
	{
		auto it = _log.find(eventString);

		if (it == _log.end())
			return std::vector<size_t>();

		return it->second;
	}

	/// <returns>
	///		A vector of eventStrings in the form "name, result" given the name.
	///		If none are found, returns an empty vector.
	/// </returns>
	std::vector<LogNameType> EventLog::findEventCalls(LogNameType eventName) const
	{
		std::vector<LogNameType> results;

		for (const auto& it : _log)
		{
			auto cutoff = it.first.find(',');
			if (it.first.substr(0, cutoff) == eventName)
				results.push_back(it.first);
		}

		return results;
	}
}