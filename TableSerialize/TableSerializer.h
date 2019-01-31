/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#pragma once

#include "TableCore/TabType.h"
#include "TableCore/StringUtils.h"
#include "TableCore/SerializeUtils.h"

namespace tab
{

	class TableSerializeCache;

	class TABLE_API TableSerializer
	{
	public:
		template<typename _WriteStream>
		TableSerializer(_WriteStream& steam)
		{
			prepareCache(static_cast<IWriteStream*>(&steam));
		}

		virtual ~TableSerializer();

		template<typename _Table>
		bool Serialize(const _Table& table)
		{
			return serialize(static_cast<const Table*>(&table));
		}

		inline bool IsSerialized() const
		{
			return bSerialized;
		}

	protected:	
		TableSerializer(IWriteStream* stream);
		bool serialize(const Table* table);
		void prepareCache(IWriteStream* stream);
		void delCache();

		bool bSerialized = false;

		TableSerializeCache* mCache = nullptr;
	};
}

