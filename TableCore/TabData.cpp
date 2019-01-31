/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#include "TableCore/TabData.h"
#include "TableCore/StringUtils.h"
namespace tab
{
	TabData::TabData()
		: CacheZero((TextChar)cstr::CHAR_NUL)
	{

	}

	TabData::~TabData()
	{
		DelData();
	}
	void TabData::Clear()
	{
		TableHead.clear();
		Fields.clear();
		DelData();
	}

	void TabData::NewCache(int32_t size)
	{
		DelData();

		TextCache = new TEXT_CHAR[size];
		TextCacheSize = size;
	}



	void TabData::DelData()
	{
		if (TextCache)
		{
			delete[] TextCache;
			TextCache = nullptr;
			TextCacheSize = 0;
		}
	}

}