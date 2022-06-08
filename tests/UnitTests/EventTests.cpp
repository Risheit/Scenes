#include "pch.h"
#include "CppUnitTest.h"
#include "Scenes/Event.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Scenes;

namespace ScenesTests
{
	TEST_CLASS(EventTests)
	{
		size_t linesRead = 0;
		EventLog log = EventLog(linesRead);
		Event<std::string(int)> eve = Event<std::string(int)>(
			[](int a) -> std::string { return "a"; }, "test", log);
		Event<int(void)> noargs = Event<int(void)>([]() -> int { return 0; }, "test", log);

		LogResultType eveCallExpected;
		LogResultType noargsCallExpected;

	public:

		TEST_METHOD(Call)
		{
			Assert::AreEqual(std::string("a"), eve(4), L"Incorrect instantiation.");
			eveCallExpected.push_back(linesRead);
		}

		TEST_METHOD(CallFunctionNoArgs)
		{
			Assert::AreEqual(0, noargs(), L"Incorrect instantiation.");
			noargsCallExpected.push_back(linesRead);
		}
		TEST_METHOD(Logging)
		{
			linesRead++;
			eve(6);
			eveCallExpected.push_back(linesRead);

			Assert::IsTrue(log.queryLog(eve.eventString()) == eveCallExpected,
				L"Event calls not giving expected log results.");
		}

		TEST_METHOD(LoggingFunctionNoArgs)
		{
			linesRead++;
			noargs();
			noargsCallExpected.push_back(linesRead);

			Assert::IsTrue(log.queryLog(noargs.eventString()) == noargsCallExpected,
				L"Event with no arguments call not giving expected log results.");
		}

	};
}
