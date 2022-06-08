#include "pch.h"
#include "CppUnitTest.h"
#include "Scenes/EventLog.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Scenes;

namespace ScenesTests
{
	TEST_CLASS(EventLogTests)
	{
		size_t linesRead = 0;
		EventLog log = EventLog(linesRead);

	public:

		TEST_METHOD(AddAndQuerySize2Vector)
		{
			LogNameType testStr1 = "Test Event 1";
			LogResultType expected;

			log.addLog(testStr1);
			expected.push_back(linesRead);
			linesRead++;
			log.addLog(testStr1);
			expected.push_back(linesRead);


			Assert::IsTrue(expected == log.queryLog(testStr1), 
				L"First Query/Add fails. ");
		}

		TEST_METHOD(AddAndQuerySize1Vector)
		{
			LogNameType testStr2 = "Test Event 2";
			LogResultType expected;

			linesRead++;
			linesRead++;
			log.addLog(testStr2);
			expected.push_back(linesRead);
			Assert::IsTrue(expected == log.queryLog(testStr2),
				L"Second Query/Add fails.");
		}

	};
}
