#ifndef __RomanTest_hxx__
#define __RomanTest_hxx__

#include "Roman.hxx"

namespace Epnet
{

class RomanTest : public Roman
{

public:
	// todo remove environment from here
	RomanTest( const std::string & id , std::string controllerType);
	virtual ~RomanTest();
	
	void selectActions();
};

} // namespace Epnet

#endif // __RomanTest_hxx__

