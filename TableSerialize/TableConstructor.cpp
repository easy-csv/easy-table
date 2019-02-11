/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#pragma once

#include "TableSerialize/TableConstructor.h"
#include "TableSerialize/SerializeUtils.h"
#include "TableCore/TabPrivateDef.h"
#include "TableCore/Table.h"

namespace tab
{
	class TableConstructorCache
	{
	public:
		TableConstructorCache()
		{

		}

		struct StringInfo
		{
			TextString str;
			int index;
		};

		Table* mTable = nullptr;
		TabArray<StringInfo> mInfos;
		int mCacheSize = 0;


		void SetTable(Table* t)
		{
			mTable = t;
		}

		void Begin()
		{
			mTable->mData.Clear();
		}

		void Clear()
		{
			mInfos.clear();
			mCacheSize = 0;
		}

		inline FieldType getNextCellType() const
		{
			index_t idx = mTable->mData.Fields.size() % mTable->mData.TableHead.size();
			return mTable->mData.TableHead[idx].TypeCode;
		}

		template<typename _Value>
		void AddValue(_Value v)
		{
			utils::AddCellValue(mTable->mData, v);
		}

		template<>
		void AddValue(TextString str)
		{
			if (str.size())
			{
				StringInfo info;

				info.str = std::move(str);
				info.index = (int)mTable->mData.Fields.size();

				mCacheSize += ((int32_t)info.str.size() + 1);

				mInfos.emplace_back(info);
			}	

			utils::AddDefaultCellText(mTable->mData);
		}

		bool EmplaceBackField(TextString&& name, FieldType code)
		{
			mTable->mData.TableHead.Decls.emplace_back();
			FieldDecl& def = mTable->mData.TableHead.Decls.back();

			def.FieldName = std::move(name);
			def.TypeCode = code;
			return true;

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

			for (int i = 0; i < mInfos.size(); ++i)
			{
				StringInfo& info = mInfos[i];

				const TextChar* first = info.str.c_str();
				const TextChar* last = first + info.str.size();

				std::copy(first, last, textIt);

				mTable->mData.Fields[info.index] = FieldCell::MakeTextCell(textIt);

				textIt += info.str.size();

				*textIt = (TextChar)cstr::CHAR_NUL;

				++textIt;
			}
		}

		void FinishHead()
		{
			if (mTable->mData.TableHead.size())
			{
				EmplaceBackField(TABLE_TEXT(""), EFT_Text);
			}
		}
		void FinishData()
		{
			FinishAll();
		}
		void FinishAll()
		{
			int32_t headSize = (int32_t)mTable->mData.TableHead.size();
			int32_t cellSize = (int32_t)mTable->mData.Fields.size();

			if (headSize)
			{
				int32_t fixCount = cellSize % headSize;

				if (fixCount)
				{
					int32_t fixBegin = headSize - fixCount;

					for (int i = 0; i < fixCount; ++headSize)
					{
						utils::AddDefaultCell(mTable->mData, fixBegin + i);
					}
				}

			}

			FixStringCache();

			mTable->mData.FieldCount = (int32_t)mTable->mData.TableHead.size();
			mTable->mData.RecordCount = (int32_t)mTable->mData.Fields.size() / (int32_t)mTable->mData.FieldCount;

			mTable->OnCache();
		}

	};

	TableConstructor::TableConstructor(Table& t)
	{
		prepareCache(&t);
	}

	bool TableConstructor::BeginHead()
	{
		if (bEditHead)
		{
			return false;
		}
		mCache->Begin();

		bEditHead = true;
		bHeadFinish = false;
		bEditData = false;
		bFinishAll = false;

		return true;
	}

	bool TableConstructor::FinishHead()
	{
		if (!bEditHead || bHeadFinish)
		{
			return false;
		}
		mCache->FinishHead();
		bEditHead = false;
		bHeadFinish = true;
		bEditData = false;
		bFinishAll = false;
		return true;
	}

	bool TableConstructor::BeginData()
	{
		if (!bHeadFinish || bEditData)
		{
			return false;
		}
		bEditHead = false;
		bHeadFinish = true;
		bEditData = true;
		bFinishAll = false;

		return true;
	}

	bool TableConstructor::FinishData()
	{
		if (!bEditData)
		{
			return false;
		}
		mCache->FinishData();
		bEditHead = false;
		bHeadFinish = true;
		bEditData = false;
		bFinishAll = true;

		return true;
	}

	bool TableConstructor::emplaceBackField(TextString&& name, FieldType typeCode)
	{
		return mCache->EmplaceBackField(std::move(name), typeCode);
	}

	FieldType TableConstructor::getNextCellType() const
	{
		return mCache->getNextCellType();
	}

	bool TableConstructor::emplaceBackFieldCell(TextString val)
	{
		 mCache->AddValue(std::move(val));
		 return true;
	}
	bool TableConstructor::emplaceBackFieldCell(FIELD_NUM val)
	{
		mCache->AddValue(val);
		return true;

	}
	bool TableConstructor::emplaceBackFieldCell(FIELD_FLOAT val)
	{
		mCache->AddValue(val);
		return true;
	}
	bool TableConstructor::emplaceBackFieldCell(FIELD_BOOL val)
	{
		mCache->AddValue(val);
		return true;
	}
	void TableConstructor::prepareCache(Table* t)
	{
		if (nullptr == mCache)
		{
			mCache = new TableConstructorCache();
		}

		mCache->SetTable(t);
		mCache->Clear();
	}
	void TableConstructor::delCache()
	{
		if (mCache)
		{
			delete mCache;
			mCache = nullptr;
		}

	}
}

