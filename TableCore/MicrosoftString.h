/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#pragma once

#include <stdlib.h>

#include "TableCore/GenericString.h"

namespace tab
{
	namespace cstr
	{
		template<>
		inline void ToValue(int64& v, const Char8_t* str)
		{
			v = _strtoi64(str, NULL, 10);
		}

		template<>
		inline void ToValue(float64& v, const Char8_t* str)
		{
			v = atof(str);
		}

		template<>
		inline void ToValue(int64& v, const Char16_t* str)
		{
			v = _wtoi64((wchar_t*)str);
		}

		template<>
		inline void ToValue(float64& v, const Char16_t* str)
		{
			v = wcstod((wchar_t*)str, NULL);
		}


		template<>
		inline void ToValue(int32& v, const Char16_t* str)
		{
			v = (int32)ToValue<int64, Char16_t>(str);
		}

		template<>
		inline void ToValue(float32& v, const Char16_t* str)
		{
			v = (float32)ToValue<float64, Char16_t>(str);
		}
		template<>
		inline void ToValue(int32& v, const Char8_t* str)
		{
			v = (int32)ToValue<int64, Char8_t>(str);
		}

		template<>
		inline void ToValue(float32& v, const Char8_t* str)
		{
			v = (float32)ToValue<float64, Char8_t>(str);
		}
	};
}