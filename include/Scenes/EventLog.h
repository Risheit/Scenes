#pragma once
#include <map>
#include <vector>
#include <string>

namespace Scenes
{
	using LogNameType = std::string;
	using LogResultType = std::vector<size_t>;
	using LogType = std::map<LogNameType, LogResultType>;

	class EventLog
	{
	public:
		EventLog(
			const size_t& linesRead
		);

		void addLog(
			LogNameType eventString
		);

		LogResultType query(
			LogNameType eventString
		) const;

		std::vector<LogNameType> findEventCalls(
			LogNameType searchTerm
		) const;

	public:
		// Map ordered with EventString -> [how many lines passed each call]
		LogType _log;
		const size_t& _linesRead;
		
	};
}
