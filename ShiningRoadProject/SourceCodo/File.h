#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <assert.h>
#include <vector>

namespace MyFile{
	const int ERR_LINE = -1;
}


class clsFILE
{
public:
	clsFILE();
	~clsFILE();

	void Open( std::string sFileName );
	void Close();


	//引数無しだとデータすべて渡す.
	//第一引数は横のライン( 何行目?の指定 ).
	//第二引数はその行の何個目のやつ?( カンマ区切り ).
	std::string GetDataString( 
		const int iRow = MyFile::ERR_LINE, 
		const int iCol = MyFile::ERR_LINE );

	int GetDataInt(
		const int iRow, 
		const int iCol );

	float GetDataFloat(
		const int iRow, 
		const int iCol );

	double GetDataDouble(
		const int iRow, 
		const int iCol );


private:


	std::string m_sFileName;
	std::vector< std::vector< std::string > > m_vvsData;

	//文字列分割読み込み( 第二引数は区切り文字 ).
	std::vector< std::string > Split( const std::string &sStr, char cSep );

	bool m_bUsing;//使用中ならtrue.

};
