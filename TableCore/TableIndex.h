/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#pragma once

#include "TableCore/TableIndexBase.h"
#include "TableCore/TabData.h"
#include "TableCore/TabLog.h"
#include "TableCore/HashTextString.h"
#include <unordered_map>

namespace tab
{
	template<typename _KeyType>
	class TableIndex : public TableIndexBase
	{
	public:
		using KeyType = _KeyType;

		using SelfType = TableIndex<KeyType>;

		using RecordIndex = index_t;

		using FieldIndex = index_t;

		template<typename K>
		using HashMapping = std::unordered_map<K, RecordIndex>;

		using HashMap = HashMapping<KeyType>;

		virtual void Clear() override
		{
			mMap.clear();
		}

		virtual index_t GetFieldIndex() const override
		{
			return mFieldIndex;
		}

		virtual index_t SearchRecord(const FIELD_NUM&) const override
		{
			return TAB_INDEX_NONE;
		}
		virtual index_t SearchRecord(const TextString&) const override
		{
			return TAB_INDEX_NONE;
		}

		template<typename CellType>
		inline void BuildIndexBy(const TabData& data, index_t index)
		{
			mFieldIndex = index;

			for (index_t record = 0; record < data.RecordCount; ++record)
			{
				const FieldCell& cell = data.GetCell(record, mFieldIndex);

				KeyType key = KeyType(cell.GetValue<typename CellType>());
				mMap.size();
				auto result = mMap.insert_or_assign(key, record);
				if (!result.second)
				{
					LOG_START(Warn) << LOG_TEXT("key '") << key << LOG_TEXT("' is redifined.") << LOG_END;
				}
			}
		}
	protected:

		inline index_t SearchRecordImpl(const _KeyType& key) const
		{
			auto itr = mMap.find(key);
			if (itr != mMap.end())
			{
				return itr->second;
			}

			return TAB_INDEX_NONE;
		}

		FieldIndex mFieldIndex = 0;
		HashMap mMap;
	};


	class TableIndexByInt : public TableIndex<FIELD_NUM>
	{
	public:
		using Super = TableIndex<FIELD_NUM>;

		virtual index_t SearchRecord(const FIELD_NUM& key) const override
		{
			return SearchRecordImpl(key);
		}
		virtual index_t SearchRecord(const TextString& key) const override
		{
			return SearchRecordImpl(cstr::ToValue<Super::KeyType, TextString>(key));
		}
	};

	class TableIndexByText : public TableIndex<HashTextString>
	{
	public:
		using Super = TableIndex<HashTextString>;

		virtual index_t SearchRecord(const TextString& key) const override
		{
			return SearchRecordImpl(HashTextString(key));
		}
	};


	template<typename _TableIndexType, typename _DataType>
	struct TableIndexMapping
	{
		using TableIndexType = _TableIndexType;
		using DataType = _DataType;
		using KeyType = typename TableIndexType::KeyType;
	};
}