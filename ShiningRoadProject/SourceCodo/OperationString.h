#ifndef OPERATION_STRING_H_
#define OPERATION_STRING_H_

#include <string>
#include <sstream>

//文字列操作クラス.
class clsOPERATION_STRING
{
public:
	clsOPERATION_STRING();
	~clsOPERATION_STRING();


	//数字と連結.
	//連結 : consolidated.
	//テンプレートの受け取る型を固定する.
	std::string ConsolidatedNumber( const std::string& s, const int				num, const char cFillZeroNum = -1 ) const;
	std::string ConsolidatedNumber( const std::string& s, const unsigned int	num, const char cFillZeroNum = -1 ) const;
	std::string ConsolidatedNumber( const std::string& s, const short			num, const char cFillZeroNum = -1 ) const;
	std::string ConsolidatedNumber( const std::string& s, const unsigned short	num, const char cFillZeroNum = -1 ) const;
	std::string ConsolidatedNumber( const std::string& s, const char			num, const char cFillZeroNum = -1 ) const;
	std::string ConsolidatedNumber( const std::string& s, const unsigned char	num, const char cFillZeroNum = -1 ) const;


	//マルチバイト文字列( char* )からワイドバイト文字列( wchat_t* )を作成.
	//wchar_t型を内部でnewしているので、使用後は必ずdelete[]すること.
	wchar_t* CreateWcharPtrFromCharPtr( const char *c ) const;

	//ワイドバイト文字列( wchat_t* )からマルチバイト文字列( char* )を作成.
	//char型を内部でnewしているので、使用後は必ずdelete[]すること.
	char* CreateCharPtrFromWcharPtr( const wchar_t *wc ) const;

private:

	//数字と連結.
	//連結 : consolidated.
	template< class T >//第三引数は何桁にするの?において何桁かを表す。(足りない分は0で埋める).
	std::string ConsolidatedNumberProduct( std::string s, const T num, const char cFillZeroNum ) const
	{
		ostringstream ss;
		int iTmpNum = static_cast<int>( num );

		//0埋めを行わないなら無視.
		if( cFillZeroNum != -1 ){
			//連結する数字の桁数.
			int iDigit = to_string( iTmpNum ).length();

			//足りない分を0で埋める.
			for( char i=0; i<cFillZeroNum - iDigit; i++ ){
				ss << 0;
			}
		}

		ss << iTmpNum;//数字を文字列に( intじゃないと事故が起こるさ ).

		s += ss.str();

		return s;
	};

		
};

#endif//#ifndef OPERATION_STRING_H_