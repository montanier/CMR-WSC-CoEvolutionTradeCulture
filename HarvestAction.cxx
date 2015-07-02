#include <HarvestAction.hxx>

#include <World.hxx>
#include <Roman.hxx>

#include <Logger.hxx>

namespace Epnet
{

HarvestAction::HarvestAction(std::string type, int requested): type(type), requested(requested)
{
}

HarvestAction::~HarvestAction()
{
}

void HarvestAction::execute( Engine::Agent & agent )
{
	Roman & romanAgent = (Roman&)agent;
	Engine::World * world = agent.getWorld();

	int inWorld = world->getValue(type, agent.getPosition());
	if(inWorld >= requested)
	{
		world->setValue(type, agent.getPosition(), inWorld - requested );
		romanAgent.addGood(type,requested);
	}
}

std::string HarvestAction::describe() const
{
	return "Harvest action";
}

} // namespace Epnet


