#include <MacmillanController.hxx>
#include <iostream>
#include <algorithm>

#include <HarvestAction.hxx>
#include <ProposeConnectionAction.hxx>
#include <SendGoodsAction.hxx>
#include <ProposeTradeAction.hxx>
#include <FunAction.hxx>

namespace Epnet
{

	struct cmpStruct{
		double subjectivePrice;
		int utility;
		int id;
	};

	bool compareRank(struct cmpStruct i, struct cmpStruct j)
	{
		if ((i.subjectivePrice / i.utility) > (j.subjectivePrice / j.utility))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	MacmillanController::MacmillanController(void)
	{
		nbGoods = 3;
		lastIdEssentialGood = 1;

		for(int i = 0 ; i <= lastIdEssentialGood ; i++)
		{
			needEssential.push_back(1);
		}

		/*
		for(int i = 0 ; i <= nbGoods ; i++)
		{
			utility.push_back(1);
		}
		*/
		utility.push_back(0.3);
		utility.push_back(0.2);
		utility.push_back(0.5);

		/*
		for(int i = 0 ; i <= nbGoods ; i++)
		{
			subjectivePrice.push_back(0.0);
		}
		*/
		subjectivePrice.push_back(5.0);
		subjectivePrice.push_back(3.0);
		subjectivePrice.push_back(5.0);
	}

	MacmillanController::~MacmillanController()
	{
	}

	void MacmillanController::updateState()
	{
		treatIncomingConnections();
		treatIncomingTrades();
	}

	std::list<Engine::Action*> MacmillanController::selectActions()
	{
		int action = _agent->getMaxActions();
		std::list<Engine::Action*> actions;
		while (action >=0)
		{
			actions.push_back(new HarvestAction("ess-a",1));
			action--;
		}
		return actions;
	}

	void MacmillanController::updateKnowledge()
	{
	}

	void MacmillanController::treatIncomingConnections()
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

	void MacmillanController::treatIncomingTrades()
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

	std::vector<int> MacmillanController::consumptionPlan(void)
	{
		std::vector<int> goodWanted;

		for(int i = 0 ; i < nbGoods ; i++)
		{
			goodWanted.push_back(0);
		}

		std::vector<double> gamma = computeGamma();

		for(int i = 0 ; i <= lastIdEssentialGood ; i++ )
		{
			goodWanted[i] = ( utility[i] / (subjectivePrice[i] * gamma[i])) + needEssential[i] ;
		}

		return goodWanted;
	}


	std::vector<double> MacmillanController::computeGamma()
	{
		//rank goods
		std::vector<cmpStruct> rankedGoods;
		for (int i = lastIdEssentialGood ; i < nbGoods ; i++)
		{
			struct cmpStruct tmp;
			tmp.subjectivePrice = subjectivePrice[i];
			tmp.utility = utility[i];
			tmp.id = i;
			rankedGoods.push_back(tmp);
		}

		std::sort(rankedGoods.begin(),rankedGoods.end(),compareRank);
		std::vector<int> sortedId;
		for(std::vector<cmpStruct>::iterator it = rankedGoods.begin() ; it != rankedGoods.end() ; it++)
		{
			sortedId.push_back((*it).id);
		}
		int r = computeR(sortedId);

		std::vector<double> gamma;
		for(int i = 0 ; i < nbGoods ; i++)
		{
			gamma.push_back(0.0);
		}
		return gamma;
	}

	int MacmillanController::computeR(std::vector<int> orderedId)
	{
		int r = 0;
		double threshold = 0.0;
		for(int i = 0 ; i <= lastIdEssentialGood ; i++)
		{
			threshold += (subjectivePrice[i] * needEssential[i]);
		}

		for(int i = 0 ; i < orderedId.size() ; i ++)
		{
			double sumUntilI = 0.0;
			for (int j = 0 ; j < i ; j++)
			{
				sumUntilI += utility[j];
			}
			double sumFromI = 0.0;
			for (int j = i ; j < orderedId.size() ; j++)
			{
				sumFromI += subjectivePrice[j];
			}
			double value = ((subjectivePrice[i] / utility[i]) * (1 - sumUntilI) ) - sumFromI;
			if (value < threshold)
			{
				r = i;
				break;
			}
		}

		return r;
	}

} // namespace Roman

