#include "EventLog.hpp"

#include "pch.h"

namespace Scenes
{
	EventLog::EventLog(const size_t& linesRead) noexcept
		: Log(linesRead)
	{}

	/// <returns>
	///		A vector of eventStrings in the form "name, result" given the name.
	///		If none are found, returns an empty vector.
	/// </returns>
	std::vector<LogNameType> EventLog::findKeys(const LogNameType& eventName) const noexcept
	{
		std::vector<LogNameType> results;

		for (const auto& key : _log | std::views::keys)
			if (const auto cutoff = key.find(','); key.substr(0, cutoff) == eventName)
				results.push_back(key);

		return results;
	}
} // Scenes
