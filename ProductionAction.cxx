
#include <ProductionAction.hxx>

#include <World.hxx>
#include <Province.hxx>

#include <Logger.hxx>

namespace Epnet
{

ProductionAction::ProductionAction()
{
}

ProductionAction::~ProductionAction()
{
}

void ProductionAction::execute(Engine::Agent& agent)
{
	Roman & romanAgent = (Roman&)agent;
	Engine::World * world = agent.getWorld();
	Province & provinceWorld = (Province&) *world;
	std::vector< std::tuple< std::string, double, double, double, double, double > > allGood= (romanAgent.getListGoods());
	
	for( auto it = allGood.begin(); it != allGood.end();it++)
	{
	 std::string good = std::get<0>(*it);
// 	 double producedQuantity = (double)(romanAgent.getProductionRate(good)*provinceWorld.getNumberOfAgents()*romanAgent.getNeed(good));
	
	 double producedQuantity = (double)(romanAgent.getProductionRate(good)*allGood.size());
	 romanAgent.setQuantity(good,producedQuantity);
	
	}
	 

}

std::string ProductionAction::describe() const
{
	return "Production action";
}

} // namespace Epnet


