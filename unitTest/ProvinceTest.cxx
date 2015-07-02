#include <ProvinceTest.hxx>
#include <Scheduler.hxx>

namespace Epnet
{

ProvinceTest::ProvinceTest(Engine::Config * config, Engine::Scheduler * scheduler ) : Province(config, scheduler)
{
}

ProvinceTest::~ProvinceTest()
{
}

void ProvinceTest::stepTest()
{
	step();
}

void ProvinceTest::removeAgents()
{
	_scheduler->removeAgents();
}


} // namespace Roman

