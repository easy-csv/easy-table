/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#pragma once

#include "TableCore/TabType.h"
#include "TableCore/TabData.h"
#include "TableCore/StringUtils.h"
#include "TableCore/TableRecord.h"
namespace tab
{
	namespace utils
	{
		inline void AddDefaultCellText(TabData& data)
		{
			data.Fields.push_back(FieldCell().SetTextPtr(&data.CacheZero));
		}

		inline void AddDefaultCellValue(TabData& data)
		{
			data.Fields.push_back(FieldCell());
		}

		template<typename T>
		inline void AddCellValue(TabData& data, const T& v)
		{
			data.Fields.emplace_back(FieldCell(v));
		}

		void AddDefaultCell(TabData& data, int32_t fieldIdx);

		template<typename _Stream>
		inline void SerializeCellEnd(_Stream& Stream)
		{
			Stream << (TABLE_TEXT(","));
		}

		template<typename _Stream>
		inline void SerializeLineEnd(_Stream& Stream)
		{
			Stream << (TABLE_TEXT("\r\n"));
		}


		template<typename _Stream>
		inline void SerializeCell(_Stream& Stream, const FieldDecl& def, const FieldCell& val)
		{
			switch (def.TypeCode)
			{
			case EFT_Float:
			{
				Stream << val.Val.fValue;
				break;
			}
			case EFT_Num:
			{
				Stream << val.Val.nValue;
				break;
			}			
			case EFT_Bool:
			{
				Stream << val.Val.bValue;
				break;
			}
			case EFT_Text:
			{
				Stream << val.Val.pText;
				break;
			}
			default:
				break;
			}
		}

		enum DeserResult
		{
			Succeed,
			TextType,
			UnknownType,
		};

		inline DeserResult DeserializeValueCell(TabData& data, int32_t index, const TextChar* val)
		{
			const FieldDecl& Field = data.TableHead[index];
			switch (Field.TypeCode)
			{
			case EFT_Num:
			{
				utils::AddCellValue(data, cstr::ToValue<FIELD_NUM>(val));
				break;
			}
			case EFT_Float:
			{
				utils::AddCellValue(data, cstr::ToValue<FIELD_FLOAT>(val));
				break;
			}
			case EFT_Bool:
			{
				utils::AddCellValue(data, cstr::ToValue<FIELD_BOOL>(val));
				break;
			}
			case EFT_Text:
			{
				return DeserResult::TextType;
				break;
			}
			default:
			{
				return DeserResult::UnknownType;
			}
			break;
			}
			return DeserResult::Succeed;
		}



		template<typename _Stream>
		inline void SerializeCells(_Stream& Stream, const FieldHead& head, const FieldCells& cells, int begin, int count)
		{
			utils::SerializeCell(Stream, head[0], cells[begin]);

			for (int fieldIdx = 1; fieldIdx < count; ++fieldIdx)
			{
				utils::SerializeCellEnd(Stream);
				utils::SerializeCell(Stream, head[fieldIdx], cells[begin + fieldIdx]);
			}
		}

		template<typename _Stream>
		inline void SerializeTableRecord(_Stream& Stream, const TableRecord& Record)
		{
			utils::SerializeCell(Stream, Record.GetHeadPtr()->GetDecl(0), Record.GetFieldCell(0));

			for (int idx = 1; idx < Record.GetHeadPtr()->size(); ++idx)
			{
				utils::SerializeCellEnd(Stream);
				utils::SerializeCell(Stream, Record.GetHeadPtr()->GetDecl(idx), Record.GetFieldCell(idx));
			}
		}



	}
}