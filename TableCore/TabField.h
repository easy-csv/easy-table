/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#pragma once

#include "TableCore/TabFieldType.h"
#include "TableCore/TabFieldReflector.h"
namespace tab
{
	struct TABLE_API FieldDecl
	{
		FieldType TypeCode = EFT_Text;
		TextString FieldName;
		uint32_t HeadIndex = 0;

		template<typename Vtype>
		inline bool Reflectable() const
		{
			return PropertyReflectable<Vtype>(TypeCode);
		}
	};

	struct TABLE_API FieldHead
	{
		using FILED_DECLS = TabArray<FieldDecl>;

		FILED_DECLS Decls;

		inline void clear()
		{
			Decls.clear();
		}

		inline int32_t size() const
		{
			return (int32_t)Decls.size();
		}
		inline const FieldDecl& GetDecl(index_t idx) const
		{
			return Decls[idx];
		}

		inline FieldDecl& GetDecl(index_t idx)
		{
			return Decls[idx];
		}

		inline const FieldDecl& operator [] (index_t idx) const
		{
			return GetDecl(idx);
		}
		inline FieldDecl& operator [] (index_t idx)
		{
			return GetDecl(idx);
		}
	};

	struct TABLE_API FieldCell
	{
		inline FieldCell()
		{
			Val.nValue = 0;
		}

		inline FieldCell(FIELD_NUM value)
		{
			Val.nValue = value;
		}
		inline FieldCell(FIELD_FLOAT value)
		{
			Val.fValue = value;
		}

		static inline FieldCell MakeTextCell(CTEXT_PTR value)
		{
			return FieldCell().SetTextPtr(value);
		}

		inline FieldCell& SetTextPtr(CTEXT_PTR value)
		{
			Val.pText = value;
			return *this;
		}



		template<typename FieldType>
		inline FieldType GetValue() const
		{
			using Property = FieldProperty<FieldType>;
			return Property::Reflector::FromCell(Val);
		}

		FieldCellData Val;
		


	};
}