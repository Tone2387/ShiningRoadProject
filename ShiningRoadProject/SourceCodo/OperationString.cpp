#include "OperationString.h"

#define LOCAL_OF_THIS_PROJECT "japanese"
using namespace std;

namespace{


}

clsOPERATION_STRING::clsOPERATION_STRING()
{
	//ロケールの指定.
	setlocale( LC_ALL, LOCAL_OF_THIS_PROJECT );
}

clsOPERATION_STRING::~clsOPERATION_STRING()
{
}



//数字と連結.
//連結 : consolidated.
string clsOPERATION_STRING::ConsolidatedNumber( const string& s, const int num, const char cFillZeroNum ) const
{
	return ConsolidatedNumberProduct( s, num, cFillZeroNum );
};

string clsOPERATION_STRING::ConsolidatedNumber( const string& s, const unsigned int num, const char cFillZeroNum ) const
{
	return ConsolidatedNumberProduct( s, num, cFillZeroNum );
};

string clsOPERATION_STRING::ConsolidatedNumber( const string& s, const short num, const char cFillZeroNum ) const
{
	return ConsolidatedNumberProduct( s, num, cFillZeroNum );
};

string clsOPERATION_STRING::ConsolidatedNumber( const string& s, const unsigned short num, const char cFillZeroNum ) const
{
	return ConsolidatedNumberProduct( s, num, cFillZeroNum );
};

string clsOPERATION_STRING::ConsolidatedNumber( const string& s, const char num, const char cFillZeroNum ) const
{
	return ConsolidatedNumberProduct( s, num, cFillZeroNum );
};

string clsOPERATION_STRING::ConsolidatedNumber( const string& s, const unsigned char num, const char cFillZeroNum ) const
{
	return ConsolidatedNumberProduct( s, num, cFillZeroNum );
};


//マルチバイト文字列( char* )からワイドバイト文字列( wchat_t* )を作成.
//wchar_t型を内部でnewしているので、使用後は必ずdelete[]すること.
wchar_t* clsOPERATION_STRING::CreateWcharPtrFromCharPtr( const char *c ) const
{
	//配列のサイズ確定.
	size_t NewSize = strlen( c ) + 1;	//何文字ですか( null文字含む ).
	wchar_t *wc = new wchar_t[ NewSize ];

	//charからwcharへ.
	size_t ConvChars = 0;
	mbstowcs_s( &ConvChars, wc, NewSize, c, _TRUNCATE );

	//newやりっぱなしなのでちゃんと使用先でdelete[]してね.
	return wc;
}

//ワイドバイト文字列( wchat_t* )からマルチバイト文字列( char* )を作成.
//char型を内部でnewしているので、使用後は必ずdelete[]すること.
char* clsOPERATION_STRING::CreateCharPtrFromWcharPtr( const wchar_t *wc ) const
{
	//配列のサイズ確定.
	size_t NewSize = wcslen( wc );	//何文字ですか.
	NewSize *= sizeof( wchar_t );	//これをしないと「何バイトか」の値にならない.
	NewSize += 1;					//( null文字含める ).
	char *c = new char[ NewSize ];

	//charからwcharへ.
	size_t ConvChars = 0;
	wcstombs_s( &ConvChars, c, NewSize, wc, _TRUNCATE );
	
	//newやりっぱなしなのでちゃんと使用先でdelete[]してね.
	return c;
}

//#ifdef HIYOSHI
const int clsOPERATION_STRING::GetNumDigit(const unsigned int iNum)
{
	int iResult = 0;//結果.
	int iDigit = 1;//桁数.
	unsigned int iNumTmp = iNum;//数字.

	const unsigned int iDecimal = 10;//十進数.

	while (iResult == 0)
	{
		//10以上か?.
		if (iNumTmp < iDecimal)
		{
			//桁を確定.
			iResult = iDigit;
		}

		else
		{
			//桁を増やす.
			iNumTmp = iNumTmp / iDecimal;
			++iDigit;
		}
	}

	return iResult;
}
//endif