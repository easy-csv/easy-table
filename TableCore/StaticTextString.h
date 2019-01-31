/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#pragma once

#include "TableCore/TabBase.h"
#include "TableCore/GenericChar.h"

namespace tab
{
	class StaticTextString
	{
	public:

		inline StaticTextString(const TextChar * ptr)
			: mText(ptr)
		{

		}
		StaticTextString()
		{

		}

		inline void assign(const TextChar * ptr)
		{
			mText = ptr;
		}

		inline bool operator == (const StaticTextString& rv) const
		{
			return cstr::StrCmp(mText, rv.mText);
		}

		inline bool operator != (const StaticTextString& rv) const
		{
			return !cstr::StrCmp(mText, rv.mText);
		}
		inline const TextChar * operator * () const
		{
			return c_str();
		}

		inline const TextChar * c_str() const
		{
			return mText;
		}
	protected:
		const TextChar* mText = nullptr;
	};
}