#pragma once
#include <string>
#include "Event.hpp"

namespace Scenes
{
	template<class Ret, class ...Args>
	struct Line
	{
		Line(
			std::string text, 
			Event<Ret(Args...)> event
		);

		const std::string text;
		const Event<Ret(Args...)> storedEvent;
	};

	template<class Ret, class ...Args>
	inline Line<Ret, Args...>::Line(std::string text, Event<Ret(Args...)> event)
		: text(text), storedEvent(event)
	{}
}