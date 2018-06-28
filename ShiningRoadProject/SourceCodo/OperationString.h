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
	std::string ConsolidatedNumber( const std::string& s, const int				num ) const;
	std::string ConsolidatedNumber( const std::string& s, const unsigned int	num ) const;
	std::string ConsolidatedNumber( const std::string& s, const short			num	) const;
	std::string ConsolidatedNumber( const std::string& s, const unsigned short	num ) const;
	std::string ConsolidatedNumber( const std::string& s, const char			num ) const;
	std::string ConsolidatedNumber( const std::string& s, const unsigned char	num	) const;

private:

	template<class T>
	std::string ConsolidatedNumberProduct( std::string s, const T num ) const
	{
		ostringstream ss;
		ss << static_cast<int>( num );//数字を文字列に( intじゃないと事故が起こるさ ).

		s += ss.str();

		return s;
	};

		
};

#endif//#ifndef OPERATION_STRING_H_