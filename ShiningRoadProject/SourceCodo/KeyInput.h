#ifndef KEY_INPUT_H_
#define KEY_INPUT_H_

#include <vector>
//#include <deque>

//ヘッダーではインクルードしないでください.
class clsKEY_INPUT
{
public:
	clsKEY_INPUT();
	~clsKEY_INPUT();

	void Update();

	bool isEnter( unsigned char ucKey );
	bool isStay( unsigned char ucKey );
	bool isExit( unsigned char ucKey );

private:

	int GetKeyIndex( unsigned char ucKey );

	std::vector< bool > m_bPushs;
	std::vector< bool > m_bPushsOld;

	std::vector< int > m_iKeyCodes;

	//生成フレームか否か.
	bool m_isStartFrame;

};
#endif//#ifndef KEY_INPUT_H_