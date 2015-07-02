#include <BasicController.hxx>

#include <iostream>

#include <HarvestAction.hxx>
#include <ProposeConnectionAction.hxx>
#include <SendGoodsAction.hxx>
#include <ProductionAction.hxx>
#include <ConsumptionAction.hxx>
#include <ProposeTradeAction.hxx>
#include <FunAction.hxx>
#include <TradeAction.hxx>
#include <CulturalAction.hxx>



namespace Epnet
{
	BasicController::BasicController()
	{
	}

	BasicController::BasicController(double mutationRate,std::string selectionProcess,std::string innovationProcess)
	{  
	  _mutationRate = mutationRate;
	  _selectionProcess = selectionProcess;
	  _innovationProcess = innovationProcess;
	}

	
	BasicController::~BasicController()
	{
	}

	void BasicController::updateState()
	{
		treatIncomingConnections();
		treatIncomingTrades();
	}

	std::list<Engine::Action*> BasicController::selectActions()
	{
		int action = _agent->getMaxActions();
		std::list<Engine::Action*> actions;
		int timestep = _agent->getWorld()->getCurrentStep();
		if(timestep%3 == 0 && _selectionProcess == "trade" )actions.push_back(new ProductionAction());
		if(timestep%3 == 1 && _selectionProcess == "trade" )actions.push_back(new TradeAction());
		if(timestep%3 == 2 && _selectionProcess == "trade" )actions.push_back(new ConsumptionAction());
		if(timestep%30 == 0)actions.push_back(new CulturalAction(_mutationRate,_selectionProcess,_innovationProcess));
		if(timestep%31 == 0)_agent->setScore(0.0);

		return actions;
	}

	void BasicController::updateKnowledge()
	{
	}

	void BasicController::treatIncomingConnections()
	{
		std::vector<std::string> receivedConnections = _agent->getReceivedConnections();
		std::vector<std::string>::iterator it = receivedConnections.begin();
		while(it != receivedConnections.end())
		{
			//accept and refuse remove the connection from receivedConnections
			//as a consequence there is no use to increment it
			int dice = std::rand()%70;
			if (dice <= 0)
			{
				_agent->acceptConnectionFrom(*it);
			}
			else
			{
				_agent->refuseConnectionFrom(*it);
			}
			receivedConnections = _agent->getReceivedConnections();
			it = receivedConnections.begin();
		}
	}

	void BasicController::treatIncomingTrades()
	{
		_agent->resetNbTrades();
		std::vector<std::tuple<std::string, std::string, double, double> > listReceivedTrades = _agent->getReceivedTrades();
		std::vector<std::tuple<std::string, std::string, double, double> >::iterator it = listReceivedTrades.begin();
		while(it != listReceivedTrades.end())
		{
			//accept and refuse remove the trade from listReceivedTrades
			//as a consequence there is no use to increment it
			int dice = std::rand()%70;
			if (dice <= 0)
			{
				_agent->acceptTradeFrom(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it), std::get<3>(*it));
				_agent->increaseNbTrades(1);
			}
			else
			{
				_agent->refuseTradeFrom(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it), std::get<3>(*it));
			}
			listReceivedTrades = _agent->getReceivedTrades();
			it = listReceivedTrades.begin();
		}
	}
} // namespace Epnet
