/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#include "TableSerialize/SerializeUtils.h"
namespace tab
{
	namespace utils
	{
		void AddDefaultCell(TabData& data, int32_t fieldIdx)
		{
			const FieldDecl& Field = data.TableHead[fieldIdx];

			if (Field.TypeCode == EFT_Text)
			{
				AddDefaultCellText(data);
			}
			else
			{
				AddDefaultCellValue(data);
			}
		}
	}
}