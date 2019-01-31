/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#pragma once

#include "TableCore/TabType.h"
#include "GenericChar.h"

#if VS_PLATFORM
#include "TableCore/MicrosoftString.h"
#else
#include "TableCore/StandardString.h"
#endif




namespace tab
{
	namespace utils
	{
		template<typename _Type>
		int32_t FindFirstOfByChar(const _Type* baseFirst, const _Type* baseLast, _Type ch)
		{
			for (const _Type* it1 = baseFirst; it1 != baseLast; ++it1)
				if (ch == *it1)     
					return (int32_t)(it1 - baseFirst);
			return TAB_INDEX_NONE;
		}
		template<typename _Str>
		int32_t FindFirstOfByChar(const _Str& BaseString, const typename _Str::value_type ch, int32_t Start = 0)
		{
			const _Str::value_type* first1 = BaseString.c_str() + Start;
			const _Str::value_type* last1 = first1 + BaseString.size();
			return FindFirstOfByChar(first1 + Start, last1, ch);
		}

		template<typename _Type>
		int32_t FindFirstOfByCharKeys(const _Type* baseFirst, const _Type* baseLast, const _Type* keysFirst, const _Type* keysLast)
		{

			for (const _Type* it1 = baseFirst; it1 != baseLast; ++it1)
				for (const _Type* it2 = keysFirst; it2 != keysLast; ++it2)
					if (*it2 == *it1)          // or: if (comp(*it,*first)) for the pred version 
						return (int32_t)(it1 - baseFirst);

			return TAB_INDEX_NONE;
		}

		template<typename _Str>
		int32_t FindFirstOfByCharKeys(const _Str& BaseString, const _Str& keys, int32_t Start = 0)
		{
			const _Str::value_type* first1 = BaseString.c_str() + Start;
			const _Str::value_type* last1 = first1 + BaseString.size();
			const _Str::value_type* first2 = keys.c_str();
			const _Str::value_type* last2 = first2 + keys.size();
			return FindFirstOfByCharKeys(first1, last1, first2, last2);
		}

		template<typename _Str>
		int32_t FindFirstOfByCharKeys(const typename _Str::value_type* baseFirst, const typename _Str::value_type* baseLast, const _Str& keys)
		{
			const _Str::value_type* first2 = keys.c_str();
			const _Str::value_type* last2 = first2 + keys.size();
			return FindFirstOfByCharKeys(baseFirst, baseLast, first2, last2);
		}

		template <typename T>
		void CopyString(T& outStr, const typename StringClass<T>::CharType* strbeg, const typename StringClass<T>::CharType* strend)
		{
			if (strbeg < strend)
			{
				outStr.assign(strbeg, strend - strbeg);
			}
			else
			{
				outStr.empty();
			}
		}

		template<typename TStr, typename TCh>
		inline void SetString(TStr& name, const TCh* begin, const TCh* end)
		{
			TStr str;
			CopyString(str, begin, end);

			cstr::ToString(name, str);
		}

	}
}

