/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#pragma once

#include "TableCore/TabType.h"



#define FIELD_UNPACK_NULL(_ValueType, _Data) _ValueType()

#define FIELD_UNPACK_INT(_ValueType, _Data) _ValueType(_Data.iValue)
#define FIELD_UNPACK_FLOAT(_ValueType, _Data) _ValueType(_Data.fValue)
#define FIELD_UNPACK_TEXT(_ValueType, _Data) _ValueType(_Data.pText)

#define FIELD_PROPERTY_DEF(_vtype, _vcode, _unpack)


namespace tab
{
	using FIELD_NUM = tabdef::int32;
	using FIELD_FLOAT = tabdef::float32;
	using FIELD_BOOL = tabdef::boolean;
	using FIELD_TEXT = StaticTextString;

	enum FieldType
	{
		EFT_Error = 0,
		EFT_Text,
		EFT_Num,	//number.
		EFT_Float,
		EFT_Bool,
		EFT_Count,
	};


	struct TABLE_API FieldTypeDef
	{
		FieldType type = EFT_Error;
		TextString name;
		inline FieldTypeDef()
		{

		}

		inline FieldTypeDef(FieldType t, TextString n)
			: type(t)
			, name(std::move(n))
		{

		}
	};

	struct TABLE_API FieldTypes
	{
		FieldTypes();
		TabArray<FieldTypeDef> FIELD_TYPE_DEFS;

		static const FieldTypeDef& GetTypeDef(FieldType ty);
		static const FieldTypeDef& GetTypeDef(const TextString& tyName);
		static const FieldTypeDef& GetTypeDef(const TextChar* tyName);

		static const FieldTypeDef& GetTypeDefOrDefault(const TextString& tyName, FieldType dftype);
		static const FieldTypeDef& GetTypeDefOrDefault(const TextChar* tyName, FieldType dftype);

		static const FieldTypes Dic;

	private:
		void SetTypeDef(FieldType ty, const TextChar* name);
	};


	using CTEXT_PTR = const TextChar*;

	union FieldCellData
	{
		FIELD_FLOAT	fValue;
		FIELD_NUM nValue;
		FIELD_BOOL bValue;
		CTEXT_PTR pText;
	};

}