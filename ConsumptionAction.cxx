#include <ConsumptionAction.hxx>

#include <World.hxx>
#include <Province.hxx>
#include <cmath>

#include <Logger.hxx>

namespace Epnet
{

	ConsumptionAction::ConsumptionAction()
	{
	}

	ConsumptionAction::~ConsumptionAction()
	{
	}

	void ConsumptionAction::execute(Engine::Agent& agent)
	{
		Roman & romanAgent = (Roman&)agent;
		Engine::World * world = agent.getWorld();
		Province & provinceWorld = (Province&) *world;


		std::vector< std::tuple< std::string, double, double, double, double, double > > allGood= romanAgent.getListGoods();
		std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator it = allGood.begin();
		double utilityFunction=0.0;
		while(it!=allGood.end())
		{
			std::string good=std::get<0>(*it);
 			if(good == std::get<0>(romanAgent.getProducedGood())) 
 				romanAgent.setQuantity(good,romanAgent.getPrice(good)); //use the estimeted value for its the production's good
				
			//fit= |a-b|/euclideDist(a,b) my favorite one
 			if(romanAgent.getQuantity(good)==(romanAgent.getNeed(good)))utilityFunction+=0; //undefined fitness function for 0
			else utilityFunction+=std::abs((romanAgent.getQuantity(good))-(romanAgent.getNeed(good)) )/(std::sqrt(std::abs((romanAgent.getQuantity(good))*(romanAgent.getQuantity(good))+(romanAgent.getNeed(good))*(romanAgent.getNeed(good)))));

			
			//fit= |a-b|/b : that one I cuts it too long right leg.
			// 	double cur=std::abs((romanAgent.getQuantity(good))-(romanAgent.getNeed(good)))/romanAgent.getNeed(good);
			// 	  if(cur>1)cur=1;
			// 	 utilityFunction+=cur;
			it++;
		}

		double score=romanAgent.getScore()+utilityFunction;
		romanAgent.setScore(score);

		if(score >= provinceWorld.getMaxScore(std::get<0>(romanAgent.getProducedGood())))provinceWorld.setMaxScore(std::get<0>(romanAgent.getProducedGood()),score);
		if(score <= provinceWorld.getMinScore(std::get<0>(romanAgent.getProducedGood())))provinceWorld.setMinScore(std::get<0>(romanAgent.getProducedGood()),score);


	}

	std::string ConsumptionAction::describe() const
	{
		return "Consumption action";
	}

} // namespace Epnet


