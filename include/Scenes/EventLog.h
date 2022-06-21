#pragma once

#include "Log.h"

namespace Scenes
{
	class EventLog final : public Log
	{
	public:
		explicit EventLog(
			const size_t& linesRead
		);

		[[nodiscard]] std::vector<LogNameType> findKeys(
			const LogNameType& eventName
		) const override;
	};
}
