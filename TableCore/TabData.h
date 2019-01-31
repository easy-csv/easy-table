/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#pragma once

#include "TableCore/TabField.h"

namespace tab
{

	class TABLE_API TabData
	{
	public:

		using FIELDS = TabArray<FieldCell>;

		using TEXT_CHAR = TextChar;
		using TEXT_CACHE = TEXT_CHAR*;

		friend class TableDeserializer;
		friend class Table;

		TabData();
		~TabData();

		FieldHead TableHead;
		FIELDS Fields;

		index_t RecordCount = 0;

		index_t FieldCount = 0;

		const TextChar CacheZero;
		TEXT_CACHE TextCache = nullptr;
		int32_t TextCacheSize = 0;

		TabData& Self()
		{
			return *this;
		}

		inline int32_t CellCount() const
		{
			return (int32_t)Fields.size();
		}

		inline const FieldCell& GetCell(index_t index) const
		{
			return Fields[index];
		}
		inline const FieldCell& GetCell(index_t recordIndex, index_t fieldIndex) const
		{
			return Fields[recordIndex * FieldCount + fieldIndex];
		}


		void NewCache(int32_t size);

		void Clear();

		//static const FieldChar EmptyC;
		//static const FIELD_VAL DefaultTextValue;

	private:
		TabData(const TabData& d);
		void operator = (const TabData& d);

		void DelData();
	};

	template<typename T>
	void SafeDeleteArray(T* arr)
	{
		if (arr)
		{
			delete[](arr);
		}
	}
}

