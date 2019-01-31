/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#pragma once

#include "TableCore/TabType.h"
#include "TableCore/StringUtils.h"

namespace tab
{

	class Table;

	static const TextString HeadPropSplit = { (TextChar)cstr::CHAR_EQ, (TextChar)cstr::CHAR_AND };

	static const TextString TypeProp = TABLE_TEXT("type");

	static const TextChar HeadValueEnd = (TextChar)cstr::CHAR_AND;
	




	class IWriteStream
	{
	public:
		using StringClass = tab::StringClass<TextString>;

		using StringType = typename  StringClass::StringType;
		using CharType = typename StringClass::CharType;

		virtual ~IWriteStream() {}
		virtual void Write(const CharType* text) = 0;

		inline IWriteStream& StreamSelf()
		{
			return *this;
		}

		template<typename _Val>
		inline IWriteStream& operator << (const _Val& v)
		{
			Write(
				StringClass::GetBuffer(cstr::ToString<StringType, _Val>(v))
			);
			return *this;
		}

		template<typename _Char>
		inline IWriteStream& operator << (const _Char* text)
		{
			Write(
				StringClass::GetBuffer(cstr::ToString<StringType, _Char>(text))
			);
			return *this;
		}
	};

	template<typename _Stream>
	class TWriteStream : public _Stream, public IWriteStream
	{
	public:
		using Super = _Stream;

		virtual void Write(const IWriteStream::CharType* text) override
		{
			getStream() << text;
		}

	protected:
		inline _Stream& getStream()
		{
			_Stream* st = this;
			return *st;
		}
	};

}

