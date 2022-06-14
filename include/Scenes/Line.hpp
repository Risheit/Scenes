#pragma once
#include <string>
#include "Event.hpp"

namespace Scenes
{
	template<class Ret, class ...Args>
	class Line
	{

	private:
		std::string	text;
		Event<Ret(Args...)> storedEvent;
	};
}