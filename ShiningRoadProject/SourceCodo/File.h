#pragma once

#include <string>
#include <vector>

namespace MyFile{
	const int ERR_LINE = -1;
}

//CSVファイルの読み書きを行うクラス.
class clsFILE
{
public:
	clsFILE();
	~clsFILE();

	//ファイルデータの型.
	using FILE_DATA = std::vector< std::vector< std::string > >;

	//----- 読み込み関係 -----//.
	//開けなかったらfalseが返る.
	bool Open( const std::string &sFileName );
	void Close();


	//引数無しだとデータすべて渡す.
	//第一引数は横のライン( 何行目?の指定 ).
	//第二引数はその行の何個目のやつ?( カンマ区切り ).
	std::string GetDataString( 
		const int iRow = MyFile::ERR_LINE, 
		const int iCol = MyFile::ERR_LINE ) const;

	int GetDataInt(
		const int iRow, 
		const int iCol ) const;

	float GetDataFloat(
		const int iRow, 
		const int iCol ) const;

	double GetDataDouble(
		const int iRow, 
		const int iCol ) const;


	//何行あるか.
	unsigned int GetSizeRow() const;
	//その行は何列あるか.
	unsigned int GetSizeCol( unsigned int uiRow = 0 ) const;


	//----- 吐き出し関係 -----//.
	//CSVに吐き出し.
	bool OutPutCsv( const FILE_DATA &data ) const;
	//OutPutCsvの引数の枠づくり.
	void CreateFileDataForOutPut( FILE_DATA &Outdata, const int iRow, const int iCol ) const;

private:


	//ファイル読み込み時に使用.
	//文字列分割( 第一引数の文字列を分割して返す )( 第二引数は区切り文字 ).
	std::vector< std::string > Split( const std::string &sStr, const char cSep ) const;

	//出力前に使う.
	//出力用文字列作成.
	//連結.Concatenation : 連結
	std::string ConcForOutPut( const FILE_DATA &data ) const;

	//ファイルパス.
	std::string m_sFileName;

	//二次元配列として考えます.
	FILE_DATA m_vvsData;

	bool m_bUsing;//使用中ならtrue.

};
