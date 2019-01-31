/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#pragma once

#include "TableCore/FieldIndex.h"

namespace tab
{
	class TableIndexBase : public IFieldIndex
	{
	public:
		virtual void Clear() = 0;

		virtual index_t SearchRecord(const FIELD_NUM& key) const = 0;
		virtual index_t SearchRecord(const TextString& text) const = 0;
		virtual index_t GetFieldIndex() const = 0;
	};
}