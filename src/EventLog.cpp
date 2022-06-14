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
	/// </summary>
	/// <returns>Vector of call times if query succeeds, empty vector if fails.</returns>
	LogResultType EventLog::query(LogNameType eventString) const
	{
		auto it = _log.find(eventString);

		if (it == _log.end())
			return std::vector<size_t>();

		return it->second;
	}

}