/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#pragma once

#include "TableCore/TabType.h"


namespace tab
{
	namespace cstr
	{

		void TABLE_API Convert(U16String& v, const Char8_t* str);
		void TABLE_API Convert(U8String& v, const Char16_t* str);

		inline void ConvertAscii(U16String& v, const U8String& str)
		{
			v.resize(str.length(), 0);

			for (int i = 0; i < str.length(); ++i)
			{
				v[i] = (Char16_t)(str[i]);
			}
		}

		// string to v
		template<typename T, typename _CharType> 
		inline void ToValue(T&,  const _CharType*)
		{
			static_assert(0);
		}

		template<typename T>
		inline void ToValue(T& v, const U8String& str)
		{
			using Str = StringClass<U8String>;

			ToValue(v, Str::GetBuffer(str));
		}

		template<typename T>
		inline void ToValue(T& v, const U16String& str)
		{
			using Str = StringClass<U16String>;

			ToValue(v, Str::GetBuffer(str));
		}

		//---------------------------------------------------------------------

		template<typename T, typename _CharType>
		inline T ToValue(const _CharType* str)
		{
			T v;
			ToValue(v, str);
			return std::move(v);
		}

		template<typename T, typename _String>
		inline T ToValue(const _String& str)
		{
			T v;
			ToValue(v, str);
			return std::move(v);
		}

		template<typename T, typename _String>
		inline T ToValue(_String&& str)
		{
			T v;
			ToValue(v, std::forward<_String>(str));
			return std::move(v);
		}

		//---------------------------------------------------------------------
		// to string.

		template<typename _String, typename T>
		void ToString(_String&, const T&)
		{
			static_assert(0);
		}

		template<typename _String, typename _Char>
		void ToString(_String&, const _Char*)
		{
			static_assert(0);
		}

		template<typename _String, typename T>
		inline _String ToString(const T& val)
		{
			_String str;
			ToString(str, val);
			return std::move(str);
		}

		template<typename _String, typename _Char>
		inline _String ToString(const _Char* val)
		{
			_String str;
			ToString(str, val);
			return std::move(str);
		}

		// u8
		template<>
		inline void ToString(U8String& str, const int32& v)
		{
			str = std::to_string(v);
		}
		template<>
		inline void ToString(U8String& str, const int64& v)
		{
			str = std::to_string(v);
		}
		template<>
		inline void ToString(U8String& str, const float32& v)
		{
			str = std::to_string(v);
		}
		template<>
		inline void ToString(U8String& str, const float64& v)
		{
			str = std::to_string(v);
		}

		template<>
		inline void ToString(U8String& str, const Char8_t& v)
		{
			str = v;
		}

		template<>
		inline void ToString(U8String& str, const Char16_t& v)
		{
			if (v <= 255)
			{
				str = (U8StringClass::CharType)v;
			}
			else
			{
				const Char16_t tmp[] = { v,0 };
				Convert(str, tmp);
			}

		}
		// u16

		template<>
		inline void ToString(U16String& str, const int32& v)
		{
			ConvertAscii(str, ToString<U8String>(v));
		}
		template<>
		inline void ToString(U16String& str, const int64& v)
		{
			ConvertAscii(str, ToString<U8String>(v));
		}
		template<>
		inline void ToString(U16String& str, const float32& v)
		{
			ConvertAscii(str, ToString<U8String>(v));
		}
		template<>
		inline void ToString(U16String& str, const float64& v)
		{
			ConvertAscii(str, ToString<U8String>(v));
		}

		template<>
		inline void ToString(U16String& str, const Char8_t& v)
		{
			str = (const U16StringClass::CharType) v;
		}
		template<>
		inline void ToString(U16String& str, const Char16_t& v)
		{
			str = v;
		}

		// string to string
		template<>
		inline void ToString(U8String& str, const U8String& v)
		{
			str = v;
		}

		inline void ToString(U8String& str, U8String&& v)
		{
			str = std::move(v);
		}
		template<>
		inline void ToString(U8String& str, const Char8_t* v)
		{
			str = v;
		}

		template<>
		inline void ToString(U8String& str, const Char16_t* v)
		{
			Convert(str, v);
		}

		template<>
		inline void ToString(U16String& str, const U16String& v)
		{
			str = v;
		}

		inline void ToString(U16String& str, U16String&& v)
		{
			str = std::move(v);
		}
		template<>
		inline void ToString(U16String& str, const Char16_t* v)
		{
			str = v;
		}

		template<>
		inline void ToString(U16String& str, const Char8_t* v)
		{
			Convert(str, v);
		}

		// convert
		template<>
		inline void ToString(U16String& v, const U8String& str)
		{
			Convert(v, U8StringClass::GetBuffer(str));
		}

		template<>
		inline void ToString(U8String& v, const U16String& str)
		{
			Convert(v, U16StringClass::GetBuffer(str));
		}

	};
}

