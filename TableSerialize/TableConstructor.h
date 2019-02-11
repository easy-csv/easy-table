/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#pragma once

#include "TableCore/Table.h"

namespace tab
{
	class TableConstructorCache;

	/**
	* TableConstructor class
	* Constructor table data api.
	*/
	class TableConstructor
	{
	public:
		TableConstructor(Table& t);

		bool BeginHead();
		bool FinishHead();
		bool BeginData();
		bool FinishData();


		inline bool IsEditHead() const
		{
			return bEditHead;
		}

		inline bool IsHeadFinish() const
		{
			return bHeadFinish;
		}

		inline bool IsEditData() const
		{
			return bEditData;
		}

		inline bool IsFinishAll() const
		{
			return bFinishAll;
		}

		template<FieldType Code>
		inline bool addField(TextString&& name)
		{
			if (IsEditHead())
			{
				return emplaceBackField(name, Code);
			}
			return false;
		}
		template<FieldType Code>
		inline bool addField(const TextString& name)
		{
			if (IsEditHead())
			{
				return emplaceBackField(TextString(name), Code);
			}
			return false;
		}

		template<typename _Value>
		inline bool addFieldCell(_Value v)
		{
			using Property = FieldProperty<T>;

			if (IsEditData() && checkNextCellType<Property::Reflector::FieldTypeCode()>())
			{
				return emplaceBackFieldCell(v);
			}
			return false;
		}
		template<typename _Value>
		inline bool addFieldCell(const _Value& v)
		{
			using Property = FieldProperty<T>;

			if (IsEditData() && checkNextCellType<Property::Reflector::FieldTypeCode()>())
			{
				return emplaceBackFieldCell(v);
			}
			return false;
		}

	protected:

		template<FieldType Code>
		inline bool emplaceBackField(TextString&& name)
		{
			return emplaceBackField(std::move(name), Code);
		}
		bool emplaceBackField(TextString&& name, FieldType typeCode);

		template<FieldType Code>
		inline bool checkNextCellType() const
		{
			return getNextCellType() == Code;
		}

		FieldType getNextCellType() const;

		bool emplaceBackFieldCell(TextString val);
		bool emplaceBackFieldCell(FIELD_NUM val);
		bool emplaceBackFieldCell(FIELD_FLOAT val);
		bool emplaceBackFieldCell(FIELD_BOOL val);

		void prepareCache(Table* t);
		void delCache();

		bool bEditHead = false;
		bool bHeadFinish = false;
		bool bEditData = false;
		bool bFinishAll = false;

		TableConstructorCache* mCache = nullptr;
	};
}

