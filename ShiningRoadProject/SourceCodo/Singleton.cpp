#include "Singleton.h"
#include <vector>


namespace{

	static std::vector< clsSINGLETON_FINALIZER::FinalizerFunc > g_vecFinalizer;
}


void clsSINGLETON_FINALIZER::AddFinalizer( FinalizerFunc func )
{
	g_vecFinalizer.emplace_back( func );


}

void clsSINGLETON_FINALIZER::Finalize()
{
	//ãtèáÇ≈è¡ÇµÇƒÇ¢Ç≠.
	auto Index = g_vecFinalizer.size() - 1;

	for( unsigned int i=0; i<g_vecFinalizer.size(); i++ ){
		( *g_vecFinalizer[ Index-- ] )();
	}

	g_vecFinalizer.clear();
	g_vecFinalizer.shrink_to_fit();
}
