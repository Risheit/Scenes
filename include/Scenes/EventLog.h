#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "Log.h"

namespace Scenes
{
	class EventLog : public Log
	{
	public:
		EventLog(
			const size_t& linesRead
		);

		std::vector<LogNameType> findKeys(
			LogNameType searchTerm
		) const override;
	};
}
