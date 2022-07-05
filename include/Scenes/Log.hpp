/**
 * @file Log.cpp
 * @brief Contains the Log class along with relevant types and functions.
 */

#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace Scenes
{
	using LogNameType = std::string; //!< The type of key that Log stores.
	using LogResultType = std::vector<size_t>; //!< The type of result Log stores.
	using LogType = std::unordered_map<LogNameType, LogResultType>; //!< The type of every record in a Log.

    /**
     * @brief Contains a record of key names and a vector of the times they were called.
     *
     * A Log class implements a hash table with string keys and values comprised of a vector of the line number at
     * which the record was logged. Log provides methods to add new records, query for a
     * specific record, and find records that contain a substring. As adding to a Log isn't meant to be done manually,
     * no tools are provided to delete added records.
     *
     * @warning Logs and log functions are case-sensitive.
     */
	class Log
	{
	public:
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;
		Log(Log&&) = default;
		Log& operator=(const Log&&) = delete;
		virtual ~Log() = default;

        /**
         * @brief Initializes a new instance of the Log class.
         *
         * @param linesRead A reference to the current line number in a game.
         */
		explicit Log(
			const size_t& linesRead
		);

        /**
         * @brief Adds a new record or updates the result of an existing record in this Log.
         *
         * When adding to a Log, if a name already exists, the current line number is added to the end of the name's
         * result vector. If the name doesn't exist in the log, a new name is created and the current line number is
         * added to a new result vector.
         *
         *
         * @param name The name of the record to add or update.
         */
		virtual void addLog(
			const LogNameType& name
		);

        /**
         * @brief Queries this Log for the result vector of a provided record name.
         *
         * @param name The name of the record to query for.
         * @return A vector containing line numbers of when this record was logged, or an empty vector if the query
         * fails.
         */
		[[nodiscard]] virtual LogResultType query(
			const LogNameType& name
		) const;

        /**
         * @brief Finds what record names in this Log contain a given search term.
         *
         * @param searchTerm The substring to search for.
         * @return A vector of all the names that contain searchTerm, or an empty vector if no names were found.
         */
		[[nodiscard]] virtual std::vector<LogNameType> findKeys(
			const LogNameType& searchTerm
		) const;

        /**
         * @brief Checks if this Log is empty (it contains no records).
         *
         * @return True if this Log is empty, false otherwise.
         */
        [[nodiscard]] bool empty() const noexcept;
	protected:
		LogType _log; //!< The hash table that stores all records in this Log.
		const size_t& _linesRead; //!< A reference to the current number of lines that have passed.
	};
} // Scenes
