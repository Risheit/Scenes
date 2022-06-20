#include "pch.h"
#include "EventLog.h"

namespace Scenes
{
	EventLog::EventLog(const size_t& linesRead)
		: Log(linesRead)
	{}

	/// <returns>
	///		A vector of eventStrings in the form "name, result" given the name.
	///		If none are found, returns an empty vector.
	/// </returns>
	std::vector<LogNameType> EventLog::findKeys(LogNameType eventName) const
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