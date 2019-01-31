/**
 *  @file
 *  @copyright defined in easy-table/LICENSE
 */
#include "TableCore/GenericString.h"

#if USE_UE4_CONVERT
# include "CoreMinimal.h"
#endif 

#if (0 == USE_UE4_CONVERT)
#	include <codecvt>
#endif

namespace tab
{

	namespace cstr
	{
		template<class Facet>
		struct deletable_facet : Facet
		{
			template<class ...Args>
			deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
			~deletable_facet() {}
		};

		void Convert(U16String& v, const Char8_t* str)
		{

#if USE_UE4_CONVERT
			{
				
				//UTF8_TO_TCHAR
				FUTF8ToTCHAR cvt((const ANSICHAR*)str);
				v = (U16StringClass::CharType*)cvt.Get();
			}

#else
			{

				std::wstring_convert<
					deletable_facet<std::codecvt<U16StringClass::CharType, char, std::mbstate_t>>, U16StringClass::CharType> conv16;

				//std::wstring_convert< std::codecvt_utf8_utf16<U16StringType::CharType>, U16StringType::CharType > conv16;

				v = conv16.from_bytes((const char*)str);
			}

#endif // !USE_UE4_CONVERT

		}

		void Convert(U8String& v, const Char16_t* str)
		{
#if USE_UE4_CONVERT
			{

				//TCHAR_TO_UTF8
				FTCHARToUTF8 cvt((const TCHAR*)str);
				v = (U8StringClass::CharType*)cvt.Get();
			}

#else
			{
				std::wstring_convert<
					deletable_facet<std::codecvt<U16StringClass::CharType, char, std::mbstate_t>>, U16StringClass::CharType> conv16;

				//std::wstring_convert< std::codecvt_utf8_utf16<U16StringType::CharType>, U16StringType::CharType > conv16;

				v = conv16.to_bytes((const U16StringClass::CharType*)str);


			}

#endif // !USE_UE4_CONVERT

		}
	}

}