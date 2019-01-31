/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#pragma once

#include "TableCore/TabType.h"
#include "TableCore/TabData.h"
namespace tab
{
	namespace utils
	{
		inline void AddDefaultCellText(TabData& data)
		{
			data.Fields.push_back(FieldCell().SetTextPtr(&data.CacheZero));
		}

		inline void AddDefaultCellValue(TabData& data)
		{
			data.Fields.push_back(FieldCell());
		}

		template<typename T>
		inline void AddCellValue(TabData& data, const T& v)
		{
			data.Fields.emplace_back(FieldCell(v));
		}

		void AddDefaultCell(TabData& data, int32_t fieldIdx);
	}
}