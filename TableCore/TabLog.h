/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */


#pragma once

#include "TableCore/TabType.h"

namespace tab
{
	class LogStreamBase
	{
	public:

		using LogStringClass = StringClass<U8String>;

		using LogChar = typename LogStringClass::CharType;
		using LogString = typename LogStringClass::StringType;
		virtual ~LogStreamBase() {}
	protected:
#if USE_STANDARD_LOG
		static void _print_log(TabLogType t, const LogString& msg);
#endif

	};

#define LOG_TEXT(msg) static_cast<const tab::LogStreamBase::LogChar*>(msg)

}


namespace tab{

	class LogStreamEmpty : public LogStreamBase
	{
	public:
		LogStreamEmpty(TabLogType )
		{

		}
		inline void operator << (const LogEndFlag& )
		{
			// empty
		}
		template<typename _Val>
		inline LogStreamEmpty& operator << (const _Val& )
		{

			return *this;
		}

		template<typename _Char>
		inline LogStreamEmpty& operator << (const _Char*)
		{

			return *this;
		}
		const LogChar* GetLog() const
		{
			return nullptr;
		}
	};

	class LogStream : public LogStreamBase
	{
	public:
		inline LogStream(TabLogType t)
			: mType(t)
		{
		}

		inline void operator << (const LogEndFlag&)
		{
			// empty
		}

		template<typename _Val>
		inline LogStream& operator << (const _Val& v)
		{
			write(
				LogStringClass::GetBuffer(cstr::ToString<LogString, _Val>(v))
			);
			return *this;
		}

		template<typename _Char>
		inline LogStream& operator << (const _Char* text)
		{
			write(
				LogStringClass::GetBuffer(cstr::ToString<LogString, _Char>(text))
			);
			return *this;
		}
		template<>
		inline LogStream& operator << (const LogChar* text)
		{
			write(text);

			return *this;
		}

		const LogChar* GetLog() const
		{
			return LogStringClass::GetBuffer(mStream);
		}

		virtual ~LogStream()
		{
#if USE_STANDARD_LOG
			_print_log(mType, mStream);
#endif
		}
	protected:
		virtual void write(const LogChar* text) 
		{
			mStream += text;
		}

		TabLogType mType;
		LogString mStream;
	};

	inline tab::LogStream Newlog(TabLogType t) 
	{
		return LogStream(t);
	}

	inline tab::LogStreamEmpty NewEmptylog(TabLogType t)
	{
		return LogStreamEmpty(t);
	}
}
#if HIDE_TABLE_LOG
#define LOG_START(_Log) { tab::LogStreamEmpty( tab::LogType_ ## _Log)
#else
#	define LOG_START(_Log) { tab::Newlog( tab::LogType_ ## _Log)
#endif

#define LOG_END tab::LogEnd;}

