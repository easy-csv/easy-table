/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#pragma once

#include "TableCore/StaticTextString.h"

namespace tab
{
	class HashTextString
	{
	public:
		using TextType = StaticTextString;

		inline HashTextString()
		{
		}

		inline HashTextString(const TextChar* str)
		{
			assign(str);
		}

		inline HashTextString(const TextString& str)
		{
			assign(str.c_str());
		}
		inline HashTextString(const StaticTextString& str)
		{
			assign(str.c_str());
		}

		inline HashTextString(const HashTextString& v)
			: mHashCode(v.mHashCode)
			, mString(v.mString)
		{
		}
		inline HashTextString& operator = (const HashTextString& str)
		{
			mHashCode = str.mHashCode;
			mString = str.mString;
		}

		inline HashTextString& operator = (const TextChar* str)
		{
			assign(str);
			return *this;
		}

		void assign(const TextChar* str)
		{
			mString.assign(str);
			CalHash();
		}

		inline bool operator == (const HashTextString& rv) const
		{
			return (HashCode() == rv.HashCode()) && cstr::StrCmp(mString.c_str(), rv.mString.c_str());
		}

		inline hash_t HashCode() const
		{
			return mHashCode;
		}

		inline const TextChar* c_str() const
		{
			return mString.c_str();
		}

	protected:

		inline void CalHash()
		{
			mHashCode = cstr::TextHash(mString.c_str());
		}
		StaticTextString mString;
		hash_t mHashCode = 0;
	};

	namespace cstr
	{
		template<typename _String>
		void ToString(_String& str, const HashTextString& v)
		{
			ToString(str, v.c_str());
		}
	}

}



namespace std
{
	template<> 
	struct hash<tab::HashTextString>
	{
		size_t operator()(const tab::HashTextString& _Keyval) const
		{
			return (size_t)_Keyval.HashCode();
		}
	};


}