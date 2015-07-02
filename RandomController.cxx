#include <RandomController.hxx>
#include <iostream>

#include <HarvestAction.hxx>
#include <ProposeConnectionAction.hxx>
#include <SendGoodsAction.hxx>
#include <ProposeTradeAction.hxx>
#include <FunAction.hxx>

namespace Epnet
{
	RandomController::RandomController()
	{
	}

	RandomController::~RandomController()
	{
	}

	void RandomController::updateState()
	{
		treatIncomingConnections();
		treatIncomingTrades();
	}

	std::list<Engine::Action*> RandomController::selectActions()
	{
		int action = _agent->getMaxActions();
		std::list<Engine::Action*> actions;
		while (action >=0)
		{
			int dice = std::rand()%7;
			switch (dice)
			{
				case 0:
					actions.push_back(new HarvestAction("ess-a",1));
					action--;
					break;

				case 1:
					actions.push_back(new HarvestAction("ess-b",1));
					action--;
					break;

				case 2:
					actions.push_back(new HarvestAction("nonEss-a",1));
					action--;
					break;

				case 3:
					actions.push_back(new HarvestAction("nonEss-b",1));
					action--;
					break;

				case 4:
					for(auto it = _agent->getWorld()->beginAgents() ; it != _agent->getWorld()->endAgents() ; it++)
					{
						std::string roman_id = (*it)->getId();;
						if(roman_id != _agent->getId())
						{
							int dice2 = std::rand()%100;
							if(dice2 < 80)
							{
								actions.push_back(new ProposeConnectionAction(roman_id));
								action--;
							}
						}
					}
					break;

				case 5:
					if(_agent->getValidSendConnections().size() > 0)
					{
						int target = std::rand()%_agent->getValidSendConnections().size();
						static const std::string types[] = {"ess-a","ess-b","nonEss-a","nonEss-b"};
						std::string type = types[std::rand()%4];
						int quantity = std::rand()%10;
						int currency = std::rand()%50;
						actions.push_back(new ProposeTradeAction(_agent->getValidSendConnections()[target],type,quantity,currency));
						action --;
					}
					break;

				case 6:
					actions.push_back(new FunAction("nonEss-a",1));
					actions.push_back(new FunAction("nonEss-b",2));
					action --;
					break;
			}
		}
		return actions;
	}

	void RandomController::updateKnowledge()
	{
	}

	void RandomController::treatIncomingConnections()
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

	void RandomController::treatIncomingTrades()
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
