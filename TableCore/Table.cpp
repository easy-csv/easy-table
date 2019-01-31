/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#include "TableCore/Table.h"
#include "StringUtils.h"
#include "TableCore/TableIndex.h"
#include "TableCore/HashTextString.h"


namespace tab
{

	//using FloatTableIndex = TableIndexMapping<FIELD_INT, FIELD_INT>;

	using NumberTableIndex = TableIndexMapping<TableIndexByInt, FIELD_NUM>;

	using TextTableIndex = TableIndexMapping<TableIndexByText, FIELD_TEXT>;


	template<typename _TableIndexMapping>
	static TableIndexBase* BuildIndex(const TabData& data, index_t index = 0)
	{

		_TableIndexMapping::TableIndexType* idx = new _TableIndexMapping::TableIndexType;

		idx->BuildIndexBy<_TableIndexMapping::DataType>(data, index);

		return idx;
	}

	Table::~Table()
	{

	}

	void Table::OnCache()
	{
		TableIndexBase* ptr = nullptr;
		const FieldDecl& decl = GetFieldDecl();

		switch (decl.TypeCode)
		{
		case EFT_Text:
		{
			ptr = BuildIndex<TextTableIndex>(mData);
			break;
		}
		case EFT_Num:
		{		
			ptr = BuildIndex<NumberTableIndex>(mData);

			break;
		}
		case EFT_Float:
		{		
			//ptr = BuildIndex<FloatTableIndex>(mData);
			break;
		}
		default:
			break;
		}

		mSearchIndices.reset(ptr);
	}
}