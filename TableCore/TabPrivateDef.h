/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#pragma once

#include "TableCore/TabDef.h"
#include "TableCore/TabLog.h"

#include <cassert>

namespace tab
{
	template<typename T>
	class TScope
	{
	public:
		TScope(T& Val, T InScope)
			: valref(Val)
		{
			originval_ = valref;
			valref = InScope;
		}
		~TScope()
		{
			valref = originval_;
		}
	protected:
		
		T& valref;
		T originval_;
	};

}
