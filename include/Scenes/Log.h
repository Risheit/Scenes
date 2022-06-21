#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace Scenes
{
	using LogNameType = std::string;
	using LogResultType = std::vector<size_t>;
	using LogType = std::unordered_map<LogNameType, LogResultType>;

	class Log
	{
	public:

		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;
		Log(Log&&) = default;
		Log& operator=(const Log&&) = delete;
		virtual ~Log() = default;

		explicit Log(
			const size_t& linesRead
		);

		virtual void addLog(
			const LogNameType& name
		);

		[[nodiscard]] virtual LogResultType query(
			const LogNameType& name
		) const;

		[[nodiscard]] virtual std::vector<LogNameType> findKeys(
			const LogNameType& searchTerm
		) const;

	protected:
		// Map ordered with EventString -> [how many lines passed each call]
		LogType _log;
		const size_t& _linesRead;
	};
}