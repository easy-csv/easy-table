/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#pragma once

#include "TableCore/TabType.h"
#include "TableCore/StringUtils.h"
#include "TableCore/SerializeUtils.h"

namespace tab
{
	class CsvTableParser;
	class TableDeserializeCache;
	class TABLE_API TableDeserializer
	{
	public:

		TableDeserializer(Table& t);
		virtual ~TableDeserializer();

		template<typename _Str>
		bool Deserialize(_Str&& text)
		{
			return DeserializeFromImpl(cstr::ToString<TextString>(text));
		}

		template<typename _Str>
		bool Deserialize(const _Str& text)
		{
			return DeserializeFromImpl(cstr::ToString<TextString>(text));
		}

		template<typename _Char>
		bool Deserialize(const _Char* text)
		{
			return DeserializeFromImpl(cstr::ToString<TextString>(text));
		}

		inline bool IsDeserialized() const
		{
			return bDeserialized;
		}

	protected:
		bool DeserializeFromImpl(TextString&& text);
		bool DeserializeImpl(const CsvTableParser& parser);

		void PrepareCache(Table* t);
		void DelCache();
	protected:

		TableDeserializeCache* mCache = nullptr;

		bool bHead = false;
		bool bDeserializing = false;
		bool bDeserialized = false;
	};
}

