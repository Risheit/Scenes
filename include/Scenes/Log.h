#pragma once

namespace Scenes
{
	using LogNameType = std::string;
	using LogResultType = std::vector<size_t>;
	using LogType = std::unordered_map<LogNameType, LogResultType>;

	class Log
	{
	public:
		Log(
			const size_t& linesRead
		);

		virtual void addLog(
			LogNameType eventString
		);

		virtual LogResultType query(
			LogNameType eventString
		) const;

		virtual std::vector<LogNameType> findKeys(
			LogNameType searchTerm
		) const;

	protected:
		// Map ordered with EventString -> [how many lines passed each call]
		LogType _log;
		const size_t& _linesRead;
	};
}