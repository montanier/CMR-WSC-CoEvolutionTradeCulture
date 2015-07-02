
#ifndef __ProvinceTest_hxx
#define __ProvinceTest_hxx

#include <World.hxx>
#include <Roman.hxx>

#include <Province.hxx>

namespace Epnet
{

class ProvinceConfig;

class ProvinceTest : public Province
{

public:
	ProvinceTest(Engine::Config * config, Engine::Scheduler * scheduler = 0);
	virtual ~ProvinceTest();

	void stepTest();
	void removeAgents();

};

} // namespace Epnet

#endif // __ProvinceTest_hxx

