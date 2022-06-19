#pragma once
#include <string>
#include <sstream>
#include <functional>
#include <typeinfo>
#include <stdexcept>
#include "EventLog.h"

namespace Scenes
{
	template<class Ret, class ...Args> class Event;

	template<class Ret, class ...Args>
	class Event<Ret(Args...)>
	{
	public:
		Event(
			std::function<Ret(Args...)> func,
			std::string name,
			EventLog& eventLogRef
			);

		virtual Ret operator()(
			Args... args
			);

		virtual std::string eventString() const;	

		const std::type_info& targetType() const;

		const std::string& name() const;

	protected:
		const std::function<Ret(Args...)> _function;
		const std::string _name;
		Ret _returnValue;
		EventLog& _eventLogRef;
	};

	/// <typeparam name="Ret">Return value.</typeparam>
	/// <typeparam name="...Args">Arguments.</typeparam>
	/// <param name="func">Contained functor.</param>
	/// <param name="name">The name of the event. Cannot contain ','.</param>
	/// <param name="eventLogRef">Refernce to log to store event calls in.</param>
	template<typename Ret, typename ...Args>
	inline Event<Ret(Args...)>::Event(std::function<Ret(Args...)> func, std::string name,
		EventLog& eventLogRef)
		: _function(func), _name(name), _returnValue(Ret()), _eventLogRef(eventLogRef)
	{
		
		if (name.find(",") != std::string::npos)
			throw std::invalid_argument{ "Event name cannot contain ','." };
	}

	template<class Ret, class ...Args>
	inline std::string Event<Ret(Args...)>::eventString() const
	{
		std::stringstream ss;
		ss << _name << "," << _returnValue;
		return ss.str();
	}

	template<class Ret, class ...Args>
	inline const std::type_info& Event<Ret(Args...)>::targetType() const
	{
		return _function.target_type();
	}

	template<class Ret, class ...Args>
	inline const std::string& Event<Ret(Args...)>::name() const
	{
		return _name;
	}

	template<class Ret, class ...Args>
	inline Ret Event<Ret(Args...)>::operator()(Args ...args)
	{
		_returnValue = _function(args...);
		_eventLogRef.addLog(eventString());
		return _returnValue;
	}




	// Specialize Event for void functions, as void _returnValue cannot exist
	template<class ...Args>
	class Event<void(Args...)>
	{
	public:
		Event(
			std::function<void(Args...)> func,
			std::string name,
			EventLog& eventLogRef
		);

		virtual void operator()(
			Args... args
			);

		virtual std::string eventString() const;

		const std::type_info& targetType() const;

		const std::string& name() const;

	protected:
		const std::function<void(Args...)> _function;
		const std::string _name;
		EventLog& _eventLogRef;
	};

	/// <typeparam name="...Args">Arguments.</typeparam>
	/// <param name="func">Contained functor.</param>
	/// <param name="name">The name of the event. Cannot contain ','.</param>
	/// <param name="eventLogRef">Refernce to log to store event calls in.</param>
	template<typename ...Args>
	inline Event<void(Args...)>::Event(std::function<void(Args...)> func, std::string name,
		EventLog& eventLogRef)
		: _function(func), _name(name), _eventLogRef(eventLogRef)
	{

		if (name.find(",") != std::string::npos)
			throw std::invalid_argument{ "Event name cannot contain ','." };
	}

	template<class ...Args>
	inline std::string Event<void(Args...)>::eventString() const
	{
		std::stringstream ss;
		ss << _name << ",";
		return ss.str();
	}

	template<class ...Args>
	inline const std::type_info& Event<void(Args...)>::targetType() const
	{
		return _function.target_type();
	}

	template<class ...Args>
	inline const std::string& Event<void(Args...)>::name() const
	{
		return _name;
	}

	template<class ...Args>
	inline void Event<void(Args...)>::operator()(Args ...args)
	{
		_function(args...);
		_eventLogRef.addLog(eventString());
	}
}
