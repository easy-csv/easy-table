/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */

#pragma once

#include <vector>
#include <string>
#include <cassert>



#ifdef __STDC_VERSION__
#	if (__STDC_VERSION__ < 201710L)
#		define _constexpr
#	else 
#		define _constexpr constexpr
#	endif
#	else
#		define _constexpr
#endif

#if defined(_MSC_VER)
#	define VS_PLATFORM 1
#endif

#if defined(_MSC_VER) && _MSC_VER > 1919 
#	undef  _constexpr 
#	define _constexpr constexpr
#endif

#define TEXT_TYPE_CHAR8 1
#define TEXT_TYPE_CHAR16 2
#define TEXT_TYPE_CHAR32 3

#ifndef TABLE_TEXT_TYPE 
#	define TABLE_TEXT_TYPE TEXT_TYPE_CHAR16
#endif

#ifndef USE_STANDARD_LOG
#	define USE_STANDARD_LOG 0
#endif

#ifndef TABLE_API
#	define TABLE_API //	empty
#endif


#ifndef USE_UE4_CONVERT
#	define USE_UE4_CONVERT 0
#endif 


#if USE_STANDARD_LOG
#	define HIDE_TABLE_LOG 0
#endif

#ifndef HIDE_TABLE_LOG
#	define HIDE_TABLE_LOG 1
#endif




namespace tab
{
	template <typename T>
	struct StringClass
	{
		using StringType = typename T;
		using CharType = typename StringType::value_type;

		static const CharType* GetBuffer(const T& str)
		{
			return str.c_str();
		}

	};

	namespace tabdef
	{
		using int32 = int32_t;
		using int64 = int64_t;
		using uint32 = uint32_t;
		using uint64 = uint64_t;
		using float32 = float;
		using float64 = double;

		using u8str = std::string;
		using char8 = StringClass<u8str>::CharType;


#if VS_PLATFORM
		using u16str = std::wstring;
#else
		using u16str = std::u16string;

#endif

		using char16 = StringClass<u16str>::CharType;
	}

}

