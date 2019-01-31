/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#pragma once

#include "TableCore/TabFieldType.h"

namespace tab
{
	struct BadFieldReflector
	{	
		template<typename _ValueType>
		inline static _ValueType ReadFromCell(const FieldCellData& data)
		{
			static_assert(0, "unknown unpack");
			return _ValueType();
		}
	};

	struct NumFieldReflector
	{
		template<typename _ValueType>
		inline static _ValueType ReadFromCell(const FieldCellData& data)
		{
			return _ValueType(data.nValue);
		}
	};

	struct FloatFieldReflector
	{
		template<typename _ValueType>
		inline static _ValueType ReadFromCell(const FieldCellData& data)
		{
			return _ValueType(data.fValue);
		}
	};
	struct TextFieldReflector
	{
		template<typename _ValueType>
		inline static _ValueType ReadFromCell(const FieldCellData& data)
		{
			return _ValueType(data.pText);
		}
	};

	template<typename _ValueType, FieldType _TypeCode, typename _Read>
	struct FieldPropertyReflector
	{
		using ValueType = _ValueType;
		using ReadFunc = _Read;
		
		inline static FieldType FieldTypeCode()
		{
			return _TypeCode;
		}

		inline static bool Reflectable(FieldType ty)
		{
			return FieldTypeCode() == ty;
		}
		inline static ValueType FromCell(const FieldCellData& cell)
		{
			return ReadFunc::ReadFromCell<ValueType>(cell);
		}
	};

	template<typename _ValueType>
	using UnknownFieldReflector =  FieldPropertyReflector<_ValueType, EFT_Error, BadFieldReflector>;

	template<typename _ValueType>
	struct FieldProperty
	{
		using Reflector = UnknownFieldReflector<_ValueType>;
	};

	template<>
	struct FieldProperty<FIELD_NUM>
	{
		using Reflector = FieldPropertyReflector<FIELD_NUM, EFT_Num, NumFieldReflector>;
	};
	template<>
	struct FieldProperty<FIELD_FLOAT>
	{
		using Reflector = FieldPropertyReflector<FIELD_FLOAT, EFT_Float, FloatFieldReflector>;
	};

	template<>
	struct FieldProperty<FIELD_TEXT>
	{
		using Reflector = FieldPropertyReflector<FIELD_TEXT, EFT_Text, TextFieldReflector>;
	};

	template<>
	struct FieldProperty<TextString>
	{
		using Reflector = FieldPropertyReflector<TextString, EFT_Text, TextFieldReflector>;
	};


	template<typename T>
	inline bool PropertyReflectable(FieldType ty)
	{
		return FieldProperty<T>::Reflector::Reflectable(ty);
	}
}