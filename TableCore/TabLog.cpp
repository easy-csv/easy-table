/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#include "TableCore/TabLog.h"
#include "TableCore/StringUtils.h"
#include <iostream>


#if USE_STANDARD_LOG

#include <iostream>
namespace tab
{
	
#define  logio std::cout
	

	void LogStreamBase::_print_log(TabLogType t, const LogString& msg)
	{	
		switch (t)
		{
		case tab::LogType_Log:
		{
			logio << ("log: ");
			break;
		}
		case tab::LogType_Warn:
		{			
			logio << ("warn: ");
			break;
		}
		case tab::LogType_Error:
		{
			logio << ("error: ");
			break;
		}
		default:
		{
			break;
		}
		}
		logio << cstr::ToString<U8String>(msg) << std::endl;


	}

}


#endif