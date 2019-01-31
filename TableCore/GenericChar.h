/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#pragma once

#include "TableCore/TabBase.h"


namespace tab
{
	namespace cstr
	{
		constexpr TabChar CHAR_CR = '\r';
		constexpr TabChar CHAR_LF = '\n';
		constexpr TabChar CHAR_HT = '\t';
		constexpr TabChar CHAR_NUL = '\0';

		constexpr TabChar CHAR_EQ = '=';
		constexpr TabChar CHAR_AND = '&';
		constexpr TabChar CHAR_QUOT = '"';
		constexpr TabChar CHAR_COMMA = ',';
		constexpr TabChar CHAR_LPAREN = '(';
		constexpr TabChar CHAR_RPAREN = ')';
	}


	namespace cstr
	{
		template<typename Char, typename HashType>
		inline HashType BKDRHashTmpl(const Char* str)
		{
			register HashType hash = 0;
			while (HashType ch = (HashType)*str++)
			{
				hash = hash * 131 + ch;   // 31、131、1313、13131、131313..      
			}
			return hash;
		}

		inline hash_t TextHash(const TextChar* str)
		{
			return BKDRHashTmpl<TextChar, hash_t>(str);
		}

		inline size_t StrLen(const Char8_t* str)
		{
			return std::strlen(str);
		}

		inline size_t StrLen(const Char16_t* str)
		{
			//static_assert(sizeof(Char16_t) == sizeof(wchar_t));
			return std::wcslen(str);//(const wchar_t*)
		}


		inline bool StrCmp(const Char8_t* str1, const Char8_t* str2)
		{
			return 0 == std::strcmp(str1, str2);
		}

		inline bool StrCmp(const Char16_t* str1, const Char16_t* str2)
		{
			return 0 == std::wcscmp(str1, str2);
		}
	}
}