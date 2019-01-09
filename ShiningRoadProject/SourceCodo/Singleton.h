#ifndef SINGLETON_FINALIZER_H_
#define SINGLETON_FINALIZER_H_

#include <mutex>
#include <assert.h>


//シングルトンの消去.
class clsSINGLETON_FINALIZER
{
public:
	using FinalizerFunc = void(*)();

	static void AddFinalizer( FinalizerFunc func );
	static void Finalize();

private:

};




template<typename T>
class clsSINGLETON final
{
public:
	static T& GetInstance(){
		std::call_once( initFlag, Create );
		assert( instance );
		return *instance;
	}


private:

	static void Create(){
		instance = new T();//シングルトンにするクラスのコンストラクタに引数を持たせることはできない.
		clsSINGLETON_FINALIZER::AddFinalizer( &clsSINGLETON<T>::Destroy );
	}

	static void Destroy(){
		if( instance ){ delete instance; }
		instance = nullptr;
	}

private:

	static std::once_flag initFlag;
	static T* instance;

};


template <typename T> std::once_flag clsSINGLETON<T>::initFlag;
template <typename T> T* clsSINGLETON<T>::instance = nullptr;

#endif//#ifndef SINGLETON_FINALIZER_H_