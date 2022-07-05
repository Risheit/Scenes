/**
 * @file Event.hpp
 * @brief Contains the Event class and functions based on it.
 */

#pragma once
#include <functional>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>

#include "EventLog.hpp"

namespace Scenes
{
    /**
     * @brief Creates an eventString for comparison or querying purposes.
     * @relates Event
     *
     * @param eventName The name of the Event.
     * @param returnValue the return value of the Event.
     * @return a created eventString.
     */
    [[nodiscard]] static std::string createEventString(
        const std::string& eventName,
        int returnValue
    );

    /**
     * @brief Runs a stored function and logs its results to a given eventLog.
     *
     * An Event is an object that acts as a wrapper for a functor taking in a variable number of arguments and
     * returning an integer. Calling an event through the () operator both calls the internal functor and logs the event
     * call to a provided eventLog.
     *
     * Events are logged via an eventString, which provides the log the event name and its return value in a
     * string format.
     *
     * @tparam Args The arguments taken in by the stored functor.
     */
	template<class ...Args>
	class Event
	{
	public:

        /**
         * @brief Initializes a new instance of the Event class.
         *
         * @param func The stored functor.
         * @param name The name of the event. Event names ARE case
         * @param eventLogRef The eventLog that this instance will log to.
         */
		Event(
			std::function<int(Args ...)> func,
			const std::string& name,
			EventLog& eventLogRef
		);

        /**
         * @brief Run the stored functor and logs its result to eventLogRef.
         * @param args The arguments _function.
         * @return The return value of _function.
         */
		int operator()(
			Args ... args
		);

        /**
         * @brief Determine the eventString of this Event.
         * @return A string taking the form "eventName,returnValue".
         */
		[[nodiscard]] std::string eventString() const noexcept;

        /**
          * @brief Determine the type of the target of this Event.
          * @return The type identifier of the target function object, or typeid(void) if !(bool)*this.
         */
		[[nodiscard]] const std::type_info& targetType() const noexcept;

        /**
         * @brief Get the name of this Event.
         * @return A string containing this Event's name.
         */
		[[nodiscard]] const std::string& name() const noexcept;

        bool operator==(const Event& rhs) const;

        bool operator!=(const Event& rhs) const;

    protected:
		const std::function<int(Args ...)> _function; //!< The stored functor.
		const std::string _name; //!< This Event's name.
		int _returnValue; //!< The return value of _function. Populated only after _function is called.
		EventLog& _eventLogRef; //!< A reference to the eventLog this Event will log to.
	};


	template<class ...Args>
	Event<Args...>::Event(std::function<int(Args ...)> func,
                          const std::string& name,EventLog& eventLogRef)
		: _function(std::move(func)), _name(name), _returnValue(int()),
          _eventLogRef(eventLogRef)
	{
		if (name.find(',') != std::string::npos)
			throw std::invalid_argument{ "Event name cannot contain ','." };
	}

    std::string createEventString(const std::string& eventName, int returnValue)
    {
        return eventName + "," + std::to_string(returnValue);
    }

	template<class ...Args>
	std::string Event<Args...>::eventString() const noexcept
	{
		return name() + "," + std::to_string(_returnValue);
	}

	template<class ...Args>
	const std::type_info& Event<Args...>::targetType() const noexcept
	{
		return _function.target_type();
	}

	template<class ...Args>
	const std::string& Event<Args...>::name() const noexcept
	{
		return _name;
	}

	template<class ...Args>
	int Event<Args...>::operator()(Args ...args)
	{
		_returnValue = _function(args...);
		_eventLogRef.addLog(eventString());
		return _returnValue;
	}

    template<class... Args>
    bool Event<Args...>::operator==(const Event& rhs) const
    {
        return _name == rhs._name &&
               _returnValue == rhs._returnValue;
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
    template<class... Args>
    bool Event<Args...>::operator!=(const Event& rhs) const
    {
        return !(rhs == *this);
    }
#pragma clang diagnostic pop

} // Scenes
