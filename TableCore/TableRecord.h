/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */

#pragma once
#include "TableCore/TabField.h"

namespace tab
{
	class TableRecord
	{
	public:

		const FieldCell& GetFieldCell(index_t idx) const
		{
			assert(0 <= idx && idx < mLength);
			return mBeginCell[idx];
		}

		int32_t ValueCount() const
		{
			return mLength;
		}

		template<typename FieldType>
		inline FieldType GetValueFast(index_t idx) const
		{
			return mBeginCell[idx].GetValue<FieldType>();
		}
		template<typename ValueType>
		inline ValueType GetValue(index_t idx) const
		{
			if (HasData() 
				&& 0 <= idx 
				&& idx < mLength 
				&& Reflectable<ValueType>(idx))
			{
				return mBeginCell[idx].GetValue<ValueType>();
			}
			return ValueType();
		}

		inline bool IsInvalid() const
		{
			return (mHead == nullptr);
		}

		inline bool IsValid() const
		{
			return (mHead != nullptr);
		}

		inline bool HasData() const
		{
			return (mBeginCell != nullptr);
		}

		template<typename VType>
		inline bool Reflectable(index_t idx) const
		{
			return mHead->GetDecl(idx).Reflectable<VType>();
		}

		const FieldHead* GetHeadPtr() const
		{
			return mHead;
		}

		const FieldCell* GetBeginCellPtr() const
		{
			return mBeginCell;
		}
		friend class TableRecordFactory;
	protected:
		TableRecord(const FieldCell* cell, const FieldHead* head, int32_t len)
			: mBeginCell(cell)
			, mHead(head)
			, mLength(len)
		{
		}
		TableRecord(){}
		const FieldCell* mBeginCell = nullptr;
		const FieldHead* mHead = nullptr;
		int32_t mLength = 0;
	};

}