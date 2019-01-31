/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#include "TableSerialize/TableDeserializer.h"
#include "TableSerialize/SerializeUtils.h"
#include "TableCore/TabPrivateDef.h"
#include "TableCore/Table.h"

#include "TableSerialize/CsvTableParser.h"

namespace tab
{

	using TableRecordList = CsvTableParser::Records;
	using TableRecordData = CsvTableParser::RecordData;


	struct FindPropResult
	{
		bool bVal = false;
	};

	struct FindIterator
	{
		FindIterator(const TextChar* pbeg, const TextChar* pend)
			: pCurrent(pbeg)
			, pEnd(pend)
		{

		}

		inline explicit operator bool () const
		{
			return pCurrent != pEnd;
		}
	
		const TextChar* pCurrent = nullptr;
		const TextChar* pEnd;
	};

	class TableDeserializeCache
	{
	public:
		TableDeserializeCache()
		{

		}
		struct StrInfo
		{
			const TextChar* str = nullptr;
			int size = 0;
			int index;

		};
		Table* mTable = nullptr;
		int mCacheSize = 0;

		TabArray<StrInfo> mStrInfos;

		void SetTable(Table* t)
		{
			mTable = t;
		}

		void Clear()
		{
			mStrInfos.clear();
			mCacheSize = 0;
		}

		FindPropResult FindPropName(TextString& propName, FindIterator& pIt, const TextString& splits)
		{
			int32_t pos = utils::FindFirstOfByCharKeys(pIt.pCurrent, pIt.pEnd, splits);

			FindPropResult fr;

			const TextChar* nextIt = (pos == TAB_INDEX_NONE) ? pIt.pEnd : (pIt.pCurrent + pos);

			utils::CopyString(propName, pIt.pCurrent, nextIt);

			if (nextIt != pIt.pEnd)
			{
				fr.bVal = (*nextIt == cstr::CHAR_EQ);
				++nextIt;
			}

			pIt.pCurrent = nextIt;

			return fr;
		}


		bool FindValueString(TextString& vStr, FindIterator& pIt)
		{
			int32_t pos = utils::FindFirstOfByChar(pIt.pCurrent, pIt.pEnd, HeadValueEnd);

			const TextChar* nextIt = (pos == TAB_INDEX_NONE) ? pIt.pEnd : (pIt.pCurrent + pos);

			utils::CopyString(vStr, pIt.pCurrent, nextIt);

			if (*nextIt == HeadValueEnd)
			{
				++nextIt;
			}

			pIt.pCurrent = nextIt;

			return vStr.size() > 0;
		}


		void ParseFIELD_DEF(FieldDecl& Def, const TextChar* cell)
		{
			const TextChar* pit = cell;
			bool bNoProps = true;
			while (*pit)
			{
				if (*pit == cstr::CHAR_LPAREN)
				{
					bNoProps = false;
					break;
				}
				++pit;
			}

			utils::SetString(Def.FieldName, cell, pit);

			if (bNoProps)
			{
				return;
			}

			const TextChar* propBegin = pit + 1;

			const TextChar* propEnd = pit;
			while (*propEnd)
			{
				if (*propEnd == cstr::CHAR_RPAREN)
				{
					break;
				}
				++propEnd;
			}

			if (propBegin >= propEnd)
			{
				return;
			}

			// find props.
			// for example: (prop1name=val1|prop2name=val2)

			TextString propName;
			TextString valStr;

			FindIterator iter(propBegin, propEnd);

			while (iter)
			{
				FindPropResult fr = FindPropName(propName, iter, HeadPropSplit);

				if (TypeProp == propName && fr.bVal)
				{

					if (FindValueString(valStr, iter))
					{
						Def.TypeCode = FieldTypes::GetTypeDefOrDefault(valStr, EFT_Text).type;
					}

				}
				else
				{
					LOG_START(Log) << LOG_TEXT("unknown propterty '") << propName << LOG_TEXT("' in field '") << Def.FieldName << LOG_TEXT("'") << LOG_END;
				}

			}
		}



		void DeserializeHead(const TableRecordData& headRecord)
		{
			for (int i = 0; i < headRecord.size(); ++i)
			{
				mTable->mData.TableHead.Decls.emplace_back();
				FieldDecl& def = mTable->mData.TableHead.Decls.back();

				ParseFIELD_DEF(def, headRecord[i]);
				def.HeadIndex = i;


			}
		}

		void DeserializeFinish()
		{
			FixStringCache();

			mTable->mData.FieldCount = (int32_t)mTable->mData.TableHead.size();
			mTable->mData.RecordCount = (int32_t)mTable->mData.Fields.size() / (int32_t)mTable->mData.FieldCount;

			mTable->OnCache();
		}

		void AddTextCache(const TextChar* val)
		{
			if (nullptr != val && *val != cstr::CHAR_NUL)
			{
				TableDeserializeCache::StrInfo info;

				info.str = val;
				info.size = (int)cstr::StrLen(val);
				info.index = (int)mTable->mData.Fields.size();

				mCacheSize += (info.size + 1);

				mStrInfos.emplace_back(info);
			}
			utils::AddDefaultCellText(mTable->mData);
		}



		void DeserializeCell(int32_t index, const TextChar* val)
		{
			const FieldDecl& Field = mTable->mData.TableHead[index];
			switch (Field.TypeCode)
			{
			case EFT_Num:
			{
				utils::AddCellValue(mTable->mData, cstr::ToValue<FIELD_NUM>(val));
				break;
			}
			case EFT_Float:
			{
				utils::AddCellValue(mTable->mData, cstr::ToValue<FIELD_FLOAT>(val));
				break;
			}
			case EFT_Text:
			{
				AddTextCache(val);
				break;
			}
			default:
			{
				LOG_START(Warn) << LOG_TEXT("unknown field type:") << (int32_t)Field.TypeCode << LOG_END;
				utils::AddDefaultCellValue(mTable->mData);
			}
			break;
			}

		}

		void FixStringCache()
		{
			if (mCacheSize == 0)
			{
				return;
			}

			// fix string cache.
			mTable->mData.NewCache(mCacheSize);

			TextChar* textIt = mTable->mData.TextCache;

			for (int i = 0; i < mStrInfos.size(); ++i)
			{
				TableDeserializeCache::StrInfo& info = mStrInfos[i];

				const TextChar* first = info.str;
				const TextChar* last = first + info.size;

				std::copy(first, last, textIt);

				mTable->mData.Fields[info.index] = FieldCell().SetTextPtr(textIt);

				textIt += info.size;

				*textIt = (TextChar)cstr::CHAR_NUL;

				++textIt;
			}
		}

		void Deserialize(const TableRecordList& records)
		{
			Clear();
			mTable->mData.Clear();
			const TableRecordData& headRecord = records[0];
			DeserializeHead(headRecord);

			size_t fieldCount = mTable->mData.TableHead.size();

			if (0 == fieldCount)
			{
				LOG_START(Warn) << LOG_TEXT("empty table") << LOG_END;
				return;
			}

			// deserialize table.

			for (int line = 1; line < records.size(); ++line)
			{
				const TableRecordData& recordLine = records[line];

				if (recordLine.size() < fieldCount)
				{
					int ielem = 0;
					for (; ielem < recordLine.size(); ++ielem)
					{
						DeserializeCell(ielem, recordLine[ielem]);
					}

					for (; ielem < fieldCount; ++ielem) // add empty field values.
					{
						utils::AddDefaultCell(mTable->mData, ielem);
					}
				}
				else
				{
					int ielem = 0;
					for (; ielem < fieldCount; ++ielem)
					{
						DeserializeCell(ielem, recordLine[ielem]);
					}

				}
			}

			DeserializeFinish();
		}

	};

	TableDeserializer::TableDeserializer(Table& t)
	{
		PrepareCache(&t);
	}

	TableDeserializer::~TableDeserializer()
	{
		DelCache();
	}

	bool TableDeserializer::DeserializeFromImpl(TextString&& text)
	{
		if (text.size() == 0)
		{
			return false;
		}

		TScope<bool> Serializing(bDeserializing, true);

		CsvTableParser Parser(std::move(text));

		return DeserializeImpl(Parser);
	}

	bool TableDeserializer::DeserializeImpl(const CsvTableParser& parser)
	{
		const TableRecordList& Records = parser.GetRecords();

		if (0 == Records.size())
		{
			return false;
		}
		bDeserialized = true;
		mCache->Deserialize(Records);
		return bDeserialized;
	}

	void TableDeserializer::PrepareCache(Table* t)
	{
		if (nullptr == mCache)
		{
			mCache = new TableDeserializeCache();
		}

		mCache->SetTable(t);
		mCache->Clear();
	}
	void TableDeserializer::DelCache()
	{
		if (mCache)
		{
			delete mCache;
			mCache = nullptr;
		}

	}
}