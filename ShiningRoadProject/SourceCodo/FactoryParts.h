#ifndef FACTORY_PARTS_H_
#define FACTORY_PARTS_H_

class clsPARTS_BASE;

#include "Global.h"

//フラグに応じてパーツを吐き出すクラス.
class clsFACTORY_PARTS
{
public:

	//パーツを吐き出す.
	clsPARTS_BASE* Create( const enPARTS enParts ) const;

};

#endif//#ifndef FACTORY_PARTS_H_