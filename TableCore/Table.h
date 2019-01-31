/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#pragma once

#include "TableCore/TabData.h"
#include "TableCore/TableRecord.h"
#include "TableCore/TableIndexBase.h"
#include <memory>

namespace tab
{

	class IFieldIndex;

	class TableRecordFactory
	{
	public:
		static TableRecord MakeRecord(const FieldCell* cell, const FieldHead* head, int32_t len)
		{
			return TableRecord(cell, head, len);
		}

		static TableRecord EmptyRecord()
		{
			return TableRecord();
		}
	};


	class TABLE_API Table
	{
	public:

		inline index_t GetRecordCount() const
		{
			return mData.RecordCount;
		}

		inline index_t GetFieldCount() const
		{
			return mData.FieldCount;
		}

		inline int32_t GetCellCount() const
		{
			return (int32_t)mData.Fields.size();
		}

		inline const FieldDecl& GetFieldDecl(int index = 0) const
		{
			return mData.TableHead[index];
		}

		inline const FieldCell& GetCell(index_t idx) const
		{
			return mData.GetCell(idx);
		}
		inline const FieldCell& GetCell(index_t recordIndex, index_t fieldIndex) const
		{
			return mData.GetCell(recordIndex, fieldIndex);
		}

		inline const FieldCell& GetRecordBeginCell(index_t recordIndex) const
		{
			return mData.GetCell(recordIndex, 0);
		}
		template<typename _KeyType>
		inline TableRecord SearchRecordTmpl(const _KeyType& key)
		{
			index_t idx = SearchRecordIndex(mSearchIndices.get(), key);
			if (idx != TAB_INDEX_NONE)
			{

				return TableRecordFactory::MakeRecord(&GetRecordBeginCell(idx), &mData.TableHead, GetFieldCount());
			}
			return TableRecordFactory::EmptyRecord();
		}

		inline TableRecord SearchRecord(const FIELD_NUM& key)
		{
			return SearchRecordTmpl(key);
		}

		inline TableRecord SearchRecord(const TextString& key)
		{
			return SearchRecordTmpl(key);
		}
		virtual ~Table();
		friend class TableSerializeCache;
		friend class TableDeserializeCache;
		friend class TableConstructorCache;
	protected:
		template<typename _KeyType>
		static inline index_t SearchRecordIndex(TableIndexBase* search, const _KeyType& key)
		{
			if (search)
			{
				return search->SearchRecord(key);
			}
			return TAB_INDEX_NONE;
		}

		virtual void OnCache();

		TabData& getData()
		{
			return mData;
		}
		std::unique_ptr<TableIndexBase> mSearchIndices;
		TabData mData;
	};
}

