/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#include "TableCore/TabFieldType.h"
#include "TableCore/StringUtils.h"
namespace tab
{

	const FieldTypes FieldTypes::Dic;

	FieldTypes::FieldTypes()
	{
		FIELD_TYPE_DEFS.resize(EFT_Count);

		SetTypeDef(EFT_Text, TABLE_TEXT("error"));
		SetTypeDef(EFT_Text, TABLE_TEXT("text"));
		SetTypeDef(EFT_Num, TABLE_TEXT("num"));
		SetTypeDef(EFT_Float, TABLE_TEXT("float"));

	}

	void FieldTypes::SetTypeDef(FieldType ty, const TextChar* name)
	{
		FIELD_TYPE_DEFS[ty] = FieldTypeDef(ty, name);
	}


	const FieldTypeDef& FieldTypes::GetTypeDef(FieldType ty)
	{
		return Dic.FIELD_TYPE_DEFS[(int32_t)ty];
	}

	const FieldTypeDef& FieldTypes::GetTypeDef(const TextString& tyName)
	{
		return GetTypeDefOrDefault(tyName, EFT_Error);
	}

	const FieldTypeDef& FieldTypes::GetTypeDef(const TextChar* tyName)
	{
		return GetTypeDefOrDefault(tyName, EFT_Error);
	}

	const FieldTypeDef& FieldTypes::GetTypeDefOrDefault(const TextString& tyName, FieldType dftype)
	{
		return GetTypeDefOrDefault(tyName.c_str(), dftype);
	}
	const FieldTypeDef& FieldTypes::GetTypeDefOrDefault(const TextChar* tyName, FieldType dftype)
	{
		for (int i = EFT_Error + 1; i < EFT_Count; ++i)
		{
			if (Dic.FIELD_TYPE_DEFS[i].name == tyName)
			{
				return Dic.FIELD_TYPE_DEFS[i];
			}
		}
		return GetTypeDef(dftype);
	}	
}