/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#include "TableSerialize/CsvTableParser.h"

namespace tab
{
	CsvTableParser::CsvTableParser(const ParseString& InSourceString)
		: mSourceString(InSourceString)
	{
		ParseRows();
	}

	CsvTableParser::CsvTableParser(ParseString&& InSourceString)
		: mSourceString(std::move(InSourceString))
	{
		ParseRows();
	}

	CsvTableParser::CsvTableParser(const ParseChar* InSourceString)
		: mSourceString(InSourceString)
	{
		ParseRows();
	}

	void CsvTableParser::ParseRows()
	{
		if (mSourceString.size())
		{
			mBufferStart = &mSourceString[0];
			mReadAt = mBufferStart;

			ParseResult Result;
			do
			{
				Result = ParseRow();
			} while (Result != ParseResult::EndOfString);
		}
	}

	CsvTableParser::ParseResult CsvTableParser::ParseRow()
	{
		// Check for an empty line
		int NewLineSize = MeasureNewLine(mReadAt);
		if (NewLineSize)
		{
			mReadAt += NewLineSize;
			return *mReadAt ? ParseResult::EndOfRow : ParseResult::EndOfString;
		}

		ParseResult Result;

		mRecords.emplace_back();
		do
		{
			Result = ParseCell();
		} while (Result == ParseResult::EndOfCell);

		return Result;
	}

	CsvTableParser::ParseResult CsvTableParser::ParseCell()
	{
		ParseChar* WriteAt = mBufferStart + (mReadAt - mBufferStart);


		bool bQuoted = (*mReadAt == cstr::CHAR_QUOT);

		if (bQuoted)
		{

			mReadAt = ++WriteAt;
		}

		mRecords.back().push_back(mReadAt);

		while (*mReadAt)
		{

			if (bQuoted)
			{
				if (*mReadAt == cstr::CHAR_QUOT)
				{
					// RFC 4180: double quotes are escaped as ""

					int32_t NumQuotes = 0;
					while (*(mReadAt + NumQuotes) == cstr::CHAR_QUOT) ++NumQuotes;

					if (NumQuotes % 2 != 0)
					{
						// No longer quoted if there are an even number of quotes before this one
						bQuoted = false;
					}

					mReadAt += NumQuotes;

					// Unescape the double quotes
					// We null terminate and leave the write pos pointing at the trailing closing quote 
					// if present so it gets overwritten by any subsequent text in the cell
					NumQuotes /= 2;
					while (NumQuotes-- > 0) *(WriteAt++) = cstr::CHAR_QUOT;
					*WriteAt = cstr::CHAR_NUL;

					continue;
				}
			}
			else
			{

				int NewLineSize = MeasureNewLine(mReadAt);
				if (NewLineSize != 0)
				{
					// Null terminate the cell
					*WriteAt = cstr::CHAR_NUL;
					mReadAt += NewLineSize;

					return *mReadAt ? ParseResult::EndOfRow : ParseResult::EndOfString;
				}
				else if (*mReadAt == cstr::CHAR_COMMA)
				{
					*WriteAt = cstr::CHAR_NUL;
					++mReadAt;

					// We always return EndOfCell here as we still have another (potentially empty) cell to add
					// In the case where ReadAt now points at the string terminator, the next call to ParseCell 
					// will add an empty cell and then return EndOfString
					return ParseResult::EndOfCell;
				}
			}
			if (WriteAt != mReadAt)
			{
				(*WriteAt++) = (*mReadAt++);
			}
			else
			{
				mReadAt = ++WriteAt;
			}
		}

		return ParseResult::EndOfString;
	}

	int CsvTableParser::MeasureNewLine(const ParseChar* At)
	{
		switch (*At)
		{
		case cstr::CHAR_CR:
			if (*(At + 1) == cstr::CHAR_LF)	// could be null
			{
				return 2;
			}
			return 1;
		case cstr::CHAR_LF:
			return 1;
		default:
			return 0;
		}
	}
}