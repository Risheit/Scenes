#pragma once

namespace Scenes
{
	typedef std::string LogNameType;
	typedef std::vector<size_t> LogResultType;
	typedef std::map<LogNameType, LogResultType> LogType;

	class EventLog
	{
	public:
		EventLog(
			const size_t& linesRead
		);
		void addLog(
			LogNameType eventString
		);
		LogResultType queryLog(
			LogNameType eventString
		) const;

	public:
		// Map ordered with EventString -> [how many lines passed each call]
		LogType _log;
		const size_t& _linesRead;
		
	};
}