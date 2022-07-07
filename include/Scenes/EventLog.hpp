/**
 * @file EventLog.hpp
 * @brief Contains the EventLog class along with relevant types and functions.
 */

#pragma once

#include "Log.hpp"

namespace Scenes
{
    /**
     * @brief A Log class that stores records of Events that have been run.
     *     * Event records names are of the form "Event name,return value" and are called eventStrings.
     *
     * @copydetails Scenes::Log
     */
	class EventLog final : public Log
	{
	public:
        /**
         * @brief Initializes a new instance of the EventLog class.
         *
         * @copydetails Scenes::Log::Log(const size_t&)
         */
		explicit EventLog(
			const size_t& linesRead
		) noexcept;

        /**
         * @brief Finds what eventStrings in this log contain a given Event name.
         * @param eventName The name to search for.
         * @return A vector of all the eventStrings that contain eventName, or an empty vector if none were found.
         */
		[[nodiscard]] std::vector<LogNameType> findKeys(
			const LogNameType& eventName
		) const noexcept override;
	};
} // Scenes
