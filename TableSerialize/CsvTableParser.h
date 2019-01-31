/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */

#pragma once

#include "TableCore/TabType.h"

namespace tab {

	class CsvTableParser
	{
	public:

		using ParseChar = TextChar;
		using ParseString = TextString;

		using RecordData =  TabArray<const ParseChar*>;
		using Records = TabArray<RecordData>;

		CsvTableParser(const ParseString& InSourceString);
		CsvTableParser(ParseString&& InSourceString);
		CsvTableParser(const ParseChar* InSourceString);

		const Records& GetRecords() const
		{
			return mRecords;
		}

	private:

		CsvTableParser(const CsvTableParser&);
		CsvTableParser& operator=(const CsvTableParser&);

		enum ParseResult 
		{ 
			EndOfCell, 
			EndOfRow, 
			EndOfString
		};


		void ParseRows();


		ParseResult ParseRow();


		ParseResult ParseCell();

	private:

		ParseString mSourceString;

		ParseChar* mBufferStart;

		const ParseChar* mReadAt;

		Records mRecords;

	private:

		int MeasureNewLine(const ParseChar* At);
	};
}