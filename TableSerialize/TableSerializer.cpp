/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */

#include "TableSerialize/TableSerializer.h"

#include "TableCore/TabPrivateDef.h"
#include "TableCore/Table.h"
#include "TableCore/StringUtils.h"

#include <sstream>


namespace tab
{
	using cachetype = StringClass<TextString>;
	using cachestream = std::basic_stringstream<cachetype::CharType, std::char_traits<cachetype::CharType>, std::allocator<cachetype::CharType>>;

	class TableSerializeCache
	{

	public:

		const Table* mTable = nullptr;
		IWriteStream* mStream;

		inline IWriteStream& Stream()
		{
			return *mStream;
		}

		void SetStream(IWriteStream* stream)
		{
			mStream = stream;
		}

		void Clear()
		{
		}

		void SerializeField(const FieldDecl& def)
		{

			Stream() << def.FieldName;

			const FieldTypeDef& type = FieldTypes::GetTypeDef(def.TypeCode);

			if (def.TypeCode != EFT_Text)
			{
				// eg. name(type=float)
				Stream() << TABLE_TEXT("(") << TypeProp << TextChar(cstr::CHAR_EQ) << type.name << TABLE_TEXT(")");
			}
		}
		void SerializeCell(const FieldDecl& def, const FieldCell& val)
		{
			switch (def.TypeCode)
			{
			case EFT_Float:
			{
				Stream() << val.Val.fValue;
				break;
			}
			case EFT_Num:
			{
				Stream() << val.Val.nValue;
				break;
			}
			case EFT_Text:
			{
				Stream() << val.Val.pText;
				break;
			}
			default:
				break;
			}
		}


		void SerializeCells(int begin, int count)
		{
			SerializeCell(mTable->mData.TableHead[0], mTable->mData.Fields[begin]);

			for (int fieldIdx = 1; fieldIdx < count; ++fieldIdx)
			{
				SerializeCellEnd();
				SerializeCell(mTable->mData.TableHead[fieldIdx], mTable->mData.Fields[begin + fieldIdx]);
			}
		}

		void SerializeCellEnd()
		{
			Stream() << (TABLE_TEXT(","));
		}

		void SerializeLineEnd()
		{
			Stream() << (TABLE_TEXT("\r\n"));
		}

		void Serialize(const Table* table)
		{
			Clear();
			mTable = table;

			int fieldCount = (int)mTable->mData.TableHead.size();

			if (fieldCount <= 0)
			{
				return;
			}
			{	// serialize head
				SerializeField(mTable->mData.TableHead[0]);

				for (int fieldIdx = 1; fieldIdx < fieldCount; ++fieldIdx)
				{
					SerializeCellEnd();
					SerializeField(mTable->mData.TableHead[fieldIdx]);
				}
			}


			int lineCount = mTable->GetRecordCount();
			if(lineCount)// serialize cells	
			{
				int beginIdx = 0;
				int lineIdx = 0;
				for (; lineIdx < lineCount; ++lineIdx, beginIdx += fieldCount)
				{
					SerializeLineEnd();
					SerializeCells(beginIdx, fieldCount);
				}
			}
		}

	};


	TableSerializer::TableSerializer(IWriteStream* stream)
	{
		prepareCache(stream);
	} 

	TableSerializer::~TableSerializer()
	{
		delCache();
	}

	void TableSerializer::prepareCache(IWriteStream* stream)
	{
		if (nullptr == mCache)
		{
			mCache = new TableSerializeCache();
		}

		mCache->SetStream(stream);
	}

	void TableSerializer::delCache()
	{
		if (mCache)
		{
			delete mCache;
			mCache = nullptr;
		}

	}

	bool TableSerializer::serialize(const Table* table)
	{
		mCache->Serialize(table);
		return true;
	}

}