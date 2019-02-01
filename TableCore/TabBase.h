/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#pragma once

#include "TableCore/TabDef.h"

namespace tab
{
	using tabdef::int32;
	using tabdef::int64;
	using tabdef::uint32;
	using tabdef::uint64;
	using tabdef::float32;
	using tabdef::float64;

	using index_t = int32_t;

	using hash_t = uint64;

	using U16String = tabdef::u16str;
	using U8String = tabdef::u8str;

	using Char8_t = tabdef::char8;
	using Char16_t = tabdef::char16;

	using TabString = U8String;

#if TABLE_TEXT_TYPE == TEXT_TYPE_CHAR8
	using TextString = U8String;
#endif

#if TABLE_TEXT_TYPE == TEXT_TYPE_CHAR16
	using TextString = U16String;
#endif

	using TextChar = StringClass<TextString>::CharType;

	using TabChar = StringClass<TabString>::CharType;

	using U8StringClass = StringClass<U8String>;
	using U16StringClass = StringClass<U16String>;

	template<typename Val>
	using TabArray = std::vector<Val>;

	constexpr index_t TAB_INDEX_NONE = -1;

	enum TabLogType
	{
		LogType_Log = 0,
		LogType_Warn = 1,
		LogType_Error = 2,
	};

	enum LogEndFlag
	{
		LogEnd,
	};
}

#if TABLE_TEXT_TYPE == TEXT_TYPE_CHAR16
#	define _TEXT_PASTE_(x) (const tab::TextChar*)(u ## x)
#endif

#if TABLE_TEXT_TYPE == TEXT_TYPE_CHAR8
#	define _TEXT_PASTE_(x) L ## x
#endif

#define TABLE_TEXT(x) _TEXT_PASTE_(x)