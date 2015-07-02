#include <Roman.hxx>

#include <Statistics.hxx>
#include <World.hxx>
#include <Logger.hxx>

#include <ProvinceConfig.hxx>
#include <ControllerFactory.hxx>

namespace Epnet
{

	Roman::Roman( const std::string & id, std::string controllerType ) : Agent(id), _resources(5), _maxActions(20), _nbTrades(0)
	{
		_controller = ControllerFactory::get().makeController(controllerType);
		_controller->setAgent(this);
		_score=0.0;
	}

	Roman::Roman( const std::string & id, std::string controllerType,double mutationRate,std::string selectionProcess, std::string innovationProcess) : Agent(id), _resources(5), _maxActions(20), _nbTrades(0)
	{
		_controller = ControllerFactory::get().makeController(controllerType,mutationRate,selectionProcess,innovationProcess);
		_controller->setAgent(this);
		_score=0.0;
		_mutationRate=mutationRate;
	}

	
	
	Roman::~Roman()
	{
	}

	void Roman::registerAttributes()
	{
 		registerFloatAttribute("scores");
		registerFloatAttribute("mu");

	  for( std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = listGoods.begin(); it != listGoods.end() ; it++)
		{
// 			std::ostringstream oss;
// 			oss <<std::get<0>(*it) << "_q";
// 			std::string name=oss.str();
// 			registerFloatAttribute(name);
			std::ostringstream ossb;
			ossb <<std::get<0>(*it) << "_p";
			std::string name=ossb.str();
			registerFloatAttribute(name);
			std::ostringstream ossc;
			ossc <<std::get<0>(*it) << "_n";
			name=ossc.str();
			registerFloatAttribute(name);
		
			
			
			/*std::ostringstream ossb;
			oss <<std::get<0>(*it) << "_q";
			 registerFloatAttribute(ossb.str());
			std::ostringstream ossc;
			oss <<std::get<0>(*it) << "_n";
			registerFloatAttribute(ossc.str());
*/
		}
		
	//	registerIntAttribute("nbConnectionsRcv");
	//	registerIntAttribute("nbConnectionsSend");
	//	registerIntAttribute("nbAchievedTrades");		

	}

	void Roman::serialize()
	{
		serializeAttribute("scores", (float)_score);
		serializeAttribute("mu", (float)_mutationRate);
		for( std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = listGoods.begin(); it != listGoods.end() ; it++)
		{
/*			std::ostringstream oss;
			oss <<std::get<0>(*it) << "_q";
			std::string name=oss.str();
			float value =(float)getQuantity(std::get<0>(*it));

			serializeAttribute(name,value); */
			std::ostringstream ossb;
			 ossb <<std::get<0>(*it) << "_p";
			std::string name=ossb.str();
			float value =(float)getPrice(std::get<0>(*it));
			
			serializeAttribute(name,value );

			std::ostringstream ossc;
			ossc <<std::get<0>(*it) << "_n";
			name=ossc.str();
			value =(float)getNeed(std::get<0>(*it));
			
			serializeAttribute(name,value); 
	
			/*			std::ostringstream ossb;
			oss <<std::get<0>(*it) << "_q";
			serializeAttribute(ossb.str(), (float)getQuantity(std::get<0>(*it)));
			std::ostringstream ossc;
			oss <<std::get<0>(*it) << "_n";
			serializeAttribute(ossc.str(), (float)getNeed(std::get<0>(*it)));
*/			
		}

		//serializeAttribute("nbConnectionsRcv", (int) validRcvConnections.size());
		//serializeAttribute("nbConnectionsSend", (int) validSendConnections.size());
		//serializeAttribute("nbAchievedTrades", _nbTrades);		
		

	
	}



	void Roman::updateKnowledge()
	{
	}

	void Roman::selectActions()
	{
		_actions = _controller->selectActions();
	}

	void Roman::updateState()
	{
		//the controller is responsible of the handling of connections and trades
		//and the consumption of non-essential resources
		_controller->updateState();

		//Roman Agent is responsible of the consumption of essential resources
		//and potenetial death
		//As we don't have "essential" food yet there is no consumption of them nor death.
		//consumeEssentialResources();
		//checkDeath();
	}





	void Roman::consumeEssentialResources()
	{
		for( std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = listGoods.begin(); it != listGoods.end() ; it++)
		{
			//search for a good with need equal to 1.0
			if (std::get<4>(*it) == 1.0)
			{
				//if found, consume one unit of it
				removeGood(std::get<0>(*it),1);
			}
		}
	}

	void Roman::checkDeath()
	{
		bool death = false;
		for( std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = listGoods.begin(); it != listGoods.end() ; it++)
		{
			//search for a good with need equal to 1.0
			if (std::get<4>(*it) == 1.0)
			{
				//if the quantity is less or equal to 0, triger the death
				if (std::get<1>(*it) <= 0.0)
				{
					death = true;
					break;
				}
			}
		}

		//if death detected, perform all the death actions
		if( death == true )
		{
			while(validSendConnections.size() > 0)
			{
				std::vector<std::string>::iterator it = validSendConnections.begin();

				killTradesTo(*it);
				killConnectionTo(*it);
			}

			//clean the agents in unfinished connections
			while (receivedConnections.size() > 0)
			{
				std::vector<std::string>::iterator it = receivedConnections.begin();
				killConnectionFrom(*it);
			}

			while (proposedConnections.size() > 0)
			{
				std::vector<std::string>::iterator it = proposedConnections.begin();
				killConnectionTo(*it);
			}

			//clean the agents and possible trades in built connections
			while(validRcvConnections.size() > 0)
			{
				std::vector<std::string>::iterator it = validRcvConnections.begin() ;
				killConnectionFrom(*it);
			}

			remove();
		}
	}







  


	void Roman::setResources( int resources )
	{
		_resources = resources;
	}

	int Roman::getResources() const
	{
		return _resources;
	}

	void Roman::resetNbTrades()
	{
		_nbTrades = 0;
	}

	int Roman::getMaxActions()
	{
		return _maxActions;
	}

	void Roman::increaseNbTrades(int value)
	{
		_nbTrades += value;
	}

	void Roman::proposeConnectionTo(std::string target)
	{
		//if do not ask to connect with self
		if( target != _id )
		{
			//if the connection is not already among the valid one, ask for it
			if( std::find(validSendConnections.begin(), validSendConnections.end(), target) == validSendConnections.end() )
			{
				Agent* uncastedPtr = _world->getAgent(target);
				if (uncastedPtr == NULL)
				{
					//std::cout << "function proposeConnectionTo. unable to get requested agent. pass" << std::endl;
					return;
				}
				Roman* targetPtr = dynamic_cast<Roman*> (uncastedPtr);
				if (targetPtr == NULL)
				{
					std::cout << "function proposeConnectionTo" << std::endl;
					std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
					exit(1);
				}
				targetPtr->requestConnectionFrom(_id);

				//if the connection is not already in the proposed one, add it
				if( std::find(proposedConnections.begin(), proposedConnections.end(), target) == proposedConnections.end() )
				{
					proposedConnections.push_back(target);
				}
			}
		}
	}

	void Roman::killConnectionTo(std::string target)
	{
		bool wasPresent = false;
		//remove traces from the proposedConnections
		if( std::find(proposedConnections.begin(), proposedConnections.end(), target) != proposedConnections.end() )
		{
			proposedConnections.erase(std::remove(proposedConnections.begin(), proposedConnections.end(), target), proposedConnections.end());
			wasPresent = true;
		}

		//remove trances from valid connections
		if( std::find(validSendConnections.begin(), validSendConnections.end(), target) != validSendConnections.end() )
		{
			validSendConnections.erase(std::remove(validSendConnections.begin(), validSendConnections.end(), target), validSendConnections.end());
			wasPresent = true;
		}

		//if the target was present there is a chance that we are in its lists
		if (wasPresent == true)
		{
			Agent* uncastedPtr = _world->getAgent(target);
			if (uncastedPtr == NULL)
			{
				std::cout << "function killConnectionTo" << std::endl;
				std::cout << "unable to get requested agent. clean the connection" << std::endl;
				exit(1);
			}
			Roman* targetPtr = dynamic_cast<Roman*> (uncastedPtr);
			if (targetPtr == NULL)
			{
				std::cout << "function killConnectionTo" << std::endl;
				std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
				exit(1);
			}
			targetPtr->killConnectionFrom(_id);
		}
	}

	void Roman::killConnectionFrom(std::string source)
	{
		bool wasPresent = false;
		//remove traces from the receivedConnections
		if( std::find(receivedConnections.begin(), receivedConnections.end(), source) != receivedConnections.end() )
		{
			receivedConnections.erase(std::remove(receivedConnections.begin(), receivedConnections.end(), source), receivedConnections.end());
			wasPresent = true;
		}

		//remove trances from valid connections
		if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) != validRcvConnections.end() )
		{
			validRcvConnections.erase(std::remove(validRcvConnections.begin(), validRcvConnections.end(), source), validRcvConnections.end());
			wasPresent = true;
		}

		//if the source was present there is a chance that we are in its lists
		if (wasPresent == true)
		{
			Agent* uncastedPtr = _world->getAgent(source);
			if (uncastedPtr == NULL)
			{
				std::cout << "function killConnectionFrom" << std::endl;
				std::cout << "unable to get requested agent. clean the connection" << std::endl;
				exit(1);
			}
			Roman* sourcePtr = dynamic_cast<Roman*> (uncastedPtr);
			if (sourcePtr == NULL)
			{
				std::cout << "function killConnectionFrom" << std::endl;
				std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
				exit(1);
			}
			sourcePtr->killConnectionTo(_id);
		}
	}

	void Roman::killConnections(std::string target)
	{
		killConnectionTo(target);
		killConnectionFrom(target);
	}

	void Roman::requestConnectionFrom(std::string source)
	{
		//if the connection has not been received, put it among the received ones
		if( std::find(receivedConnections.begin(), receivedConnections.end(), source) == receivedConnections.end() )
		{
			receivedConnections.push_back(source);
		}
	}

	void Roman::acceptConnectionFrom(std::string source)
	{
		//if the source has not been already validated
		if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) == validRcvConnections.end() )
		{
			Agent* uncastedPtr = _world->getAgent(source);
			if (uncastedPtr == NULL)
			{
				std::cout << "function acceptConnectionFrom" << std::endl;
				std::cout << "unable to get requested agent. clean the connection" << std::endl;
				exit(1);
			}
			Roman* sourcePtr = dynamic_cast<Roman*> (uncastedPtr);
			if (sourcePtr == NULL)
			{
				std::cout << "function acceptConnectionFrom" << std::endl;
				std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
				exit(1);
			}

			//if the source is effectively in the received connection
			if( std::find(receivedConnections.begin(), receivedConnections.end(), source) != receivedConnections.end() )
			{
				receivedConnections.erase(std::remove(receivedConnections.begin(), receivedConnections.end(), source), receivedConnections.end());
				//confirm the connection to source, and verify that source is aware of it
				if (sourcePtr->ackConnectionFrom(_id))
				{
					validRcvConnections.push_back(source);
				}
			}
			//if the source is not in the lit of received, something went wrong, kill things
			else
			{
				sourcePtr->killConnectionTo(_id);
			}
		}
	}

	void Roman::refuseConnectionFrom(std::string source)
	{
		Agent* uncastedPtr = _world->getAgent(source);
		if (uncastedPtr == NULL)
		{
			std::cout << "function refuseConnectionFrom" << std::endl;
			std::cout << "unable to get requested agent. clean the connection" << std::endl;
			exit(1);
		}
		Roman* sourcePtr = dynamic_cast<Roman*> (uncastedPtr);
		if (sourcePtr == NULL)
		{
			std::cout << "function refuseConnectionFrom" << std::endl;
			std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		//refuse connection from source
		sourcePtr->nackConnectionFrom(_id);
		//remove source
		receivedConnections.erase(std::remove(receivedConnections.begin(), receivedConnections.end(), source), receivedConnections.end());
	}

	void Roman::proposeConnectionBetween(std::string source, std::string target)
	{
		Agent* uncastedPtr = _world->getAgent(source);
		if (uncastedPtr == NULL)
		{
			std::cout << "function proposeConnectionBetween" << std::endl;
			std::cout << "unable to get requested agent. clean the connection" << std::endl;
			exit(1);
		}
		Roman* sourcePtr = dynamic_cast<Roman*> (uncastedPtr);
		if (sourcePtr == NULL)
		{
			std::cout << "function proposeConnectionBetween" << std::endl;
			std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		sourcePtr->proposeConnectionTo(target);
	}

	void Roman::killConnectionBetween(std::string source, std::string target)
	{
		Agent* uncastedPtr = _world->getAgent(source);
		if (uncastedPtr == NULL)
		{
			std::cout << "function killConnectionBetween" << std::endl;
			std::cout << "unable to get requested agent. clean the connection" << std::endl;
			exit(1);
		}
		Roman* sourcePtr = dynamic_cast<Roman*> (uncastedPtr);
		if (sourcePtr == NULL)
		{
			std::cout << "function killConnectionBetween" << std::endl;
			std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		sourcePtr->killConnectionTo(target);
	}

	int Roman::ackConnectionFrom(std::string target)
	{
		//if we had sent a connection we acknowledge it, otherwise we just return an error
		if( std::find(proposedConnections.begin(), proposedConnections.end(), target) != proposedConnections.end() )
		{
			proposedConnections.erase(std::remove(proposedConnections.begin(), proposedConnections.end(), target), proposedConnections.end());
			validSendConnections.push_back(target);
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int Roman::nackConnectionFrom(std::string target)
	{
		//if we had sent a connection we kill it, otherwise we just return an error
		if( std::find(proposedConnections.begin(), proposedConnections.end(), target) != proposedConnections.end() )
		{
			proposedConnections.erase(std::remove(proposedConnections.begin(), proposedConnections.end(), target), proposedConnections.end());
			return 1;
		}
		else
		{
			return 0;
		}
	}

	void Roman::receiveMessageFrom(std::string source, std::string msg)
	{
		if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) != validRcvConnections.end() )
		{
			receivedMessages.push_back(std::make_tuple(source,msg));
		}
	}

	void Roman::sendMessageTo(std::string target, std::string msg)
	{
		// if the connection with the target has been valideted
		if( std::find(validSendConnections.begin(), validSendConnections.end(), target) != validSendConnections.end() )
		{
			Agent* uncastedPtr = _world->getAgent(target);
			if (uncastedPtr == NULL)
			{
				std::cout << "function sendMessageTo" << std::endl;
				std::cout << "unable to get requested agent. clean the connection" << std::endl;
				exit(1);
			}
			Roman* targetPtr = dynamic_cast<Roman*> (uncastedPtr);
			if (targetPtr == NULL)
			{
				std::cout << "function sendMessageTo" << std::endl;
				std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
				exit(1);
			}
			targetPtr->receiveMessageFrom(_id, msg);
		}
	}

	void Roman::addGoodType(std::string type,double max,double price,double need,double productionRate)
	{
		//check if a good of that type is already in that list
		if ( std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double,double,double,double>& good) {return std::get<0>(good) == type;}) == listGoods.end() )
		{
			//if not, add it
			listGoods.push_back(std::make_tuple(type,0,max,price,need,productionRate));
		}
	}

	void Roman::removeGoodType(std::string type)
	{
		//check if a good of that type exist in that list
		std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double,double,double,double>& good) {return std::get<0>(good) == type;});
		while ( it != listGoods.end() )
		{
			//if not, add it
			listGoods.erase(it);
			//update the presence of good of that type in the list
			it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double,double,double,double>& good) {return std::get<0>(good) == type;});
		}
	}

	std::tuple<double,double,double,double,double> Roman::getGood(std::string type)
	{
		//check if a good of that type exist in the list
		std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double,double,double,double>& good) {return std::get<0>(good) == type;});
		if ( it != listGoods.end() )
		{
			//return quantity, maxquantity,price, need and productionRate good
			return std::make_tuple(std::get<1>(*it),std::get<2>(*it),std::get<3>(*it),std::get<4>(*it),std::get<5>(*it));
		}

		//return something impossible as an error
		return std::make_tuple(-1.0,-1.0,-1.0,-1.0,-1.0);
	}

	
	//TODO: actually this function return the good that is produced. But should return a vector of goods if there are more than one good produced
	std::tuple<std::string,double,double,double,double,double>  Roman::getProducedGood()
	{
		std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double,double,double,double>& good) {return std::get<5>(good) > 0 ;});

		return *it;
	}
	
	void Roman::addGood(std::string type,double value)
	{
		//check if a good of that type exists in that list
		std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double,double,double,double>& good) {return std::get<0>(good) == type;});
		//if yes add the value to it within the bound
		if ( it != listGoods.end() )
		{
			double toSet = std::min(std::get<1>(*it) + value, std::get<2>(*it));
			std::get<1>(*it) = toSet;
		}
	}
	
	void Roman::setPrice(std::string type,double value)
	{
		//check if a good of that type exists in that list
		std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double,double,double,double>& good) {return std::get<0>(good) == type;});
		//if yes add the value to it within the bound
		if ( it != listGoods.end() )
		{
			std::get<3>(*it) = value;
		}
	}
	
	void Roman::setQuantity(std::string type,double value)
	{
		//check if a good of that type exists in that list
		std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double,double,double,double>& good) {return std::get<0>(good) == type;});
		//if yes add the value to it within the bound
		if ( it != listGoods.end() )
		{
			std::get<1>(*it) = value;
		}
	}

	void Roman::setNeed(std::string type,double value)
	{
		//check if a good of that type exists in that list
		std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double,double,double,double>& good) {return std::get<0>(good) == type;});
		//if yes add the value to it within the bound
		if ( it != listGoods.end() )
		{
			std::get<4>(*it) = value;
		}
	}

	void Roman::setProductionRate(std::string type,double value)
	{
		//check if a good of that type exists in that list
		std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double,double,double,double>& good) {return std::get<0>(good) == type;});
		//if yes add the value to it within the bound
		if ( it != listGoods.end() )
		{
			std::get<5>(*it) = value;
		}
		
	}

	
	void Roman::removeGood(std::string type,double value)
	{
		//check if a good of that type exists in that list
		std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double,double,double,double>& good) {return std::get<0>(good) == type;});
		//if yes remove the value to it as long as it's superior to 0
		if ( it != listGoods.end() )
		{
			double toSet = std::max(std::get<1>(*it) - value, 0.0);
			std::get<1>(*it) = toSet;
		}
	}

	std::vector<std::tuple<std::string,double,double,double,double,double> >  Roman::getListGoodsFrom(std::string target)
	{
		Agent* uncastedPtr = _world->getAgent(target);
		if (uncastedPtr == NULL)
		{
			std::cout << "function getListGoodsFrom" << std::endl;
			std::cout << "unable to get requested agent. clean the connection" << std::endl;
			exit(1);
		}
		Roman* targetPtr = dynamic_cast<Roman*> (uncastedPtr);
		if (targetPtr == NULL)
		{
			std::cout << "function getListGoodsFrom" << std::endl;
			std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}
		return targetPtr->getListGoods();
	}
	
	void Roman::printInventory()
	{
	  std::cout<<"-------------------------------"<<std::endl;
	  std::cout<<"Inventory of :"<<_id<<" with score "<<_score<<std::endl;

	  for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator it = this->listGoods.begin() ; it != this->listGoods.end() ; it++)
		{
		  std::cout<<std::get<0>(*it)<<"\t"<<std::get<1>(*it)<<"\t"<<std::get<2>(*it)<<"\t"<<std::get<3>(*it)<<"\t"<<std::get<4>(*it)<<"\t"<<std::get<5>(*it)<<std::endl;
		}
	  std::cout<<"-------------------------------"<<std::endl;

	  
	}

	void Roman::sendGoodTo(std::string target, std::string type, double value)
	{
		// if the connection with the target has been validated
		if( std::find(validSendConnections.begin(), validSendConnections.end(), target) != validSendConnections.end() )
		{
			//check if a good of that type exists in that list
			std::vector<std::tuple<std::string,double,double,double,double,double> >::iterator it = std::find_if(listGoods.begin(), listGoods.end(), [=](const std::tuple<std::string,double,double,double,double,double>& good) {return std::get<0>(good) == type;});
			//if yes remove the value to it as long as it's superior to 0
			if ( it != listGoods.end() )
			{
				if ( std::get<1>(*it) >= value)
				{
					Agent* uncastedPtr = _world->getAgent(target);
					if (uncastedPtr == NULL)
					{
						std::cout << "function sendGoodTo" << std::endl;
						std::cout << "unable to get requested agent. clean the connection" << std::endl;
						exit(1);
					}
					Roman* targetPtr = dynamic_cast<Roman*> (uncastedPtr);
					if (targetPtr == NULL)
					{
						std::cout << "function sendGoodTo" << std::endl;
						std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
						exit(1);
					}

					if(targetPtr->receiveGoodFrom(_id, type, value))
					{
						removeGood(type,value);
					}
				}
			}
		}
	}

	int Roman::receiveGoodFrom(std::string source, std::string type, double value)
	{
		//if in the list of validated senders receive the good
		if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) != validRcvConnections.end() )
		{
			//TODO modify add good to add a return value. return 0 if was not able to add the good (good type does not exist)
			addGood(type,value);
			return 1;
		}

		//if arrive to that point, the agent was not a valid sender and we return an error
		return 0;
	}

	int Roman::receiveTradeFrom(std::string source, std::string type, double value, double currency)
	{
		auto it = listGoods.begin();
		while( it != listGoods.end() )
		{
			if (std::get<0>(*it) == type) break;
			it++;
		}
		//if arrived at the end of the list of goods without finding the specific good, quit the function now
		if (it == listGoods.end()) return 0;

		//if in the list of validated senders receive the trade offer
		if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) != validRcvConnections.end() )
		{
			listReceivedTrades.push_back(make_tuple(source,type,value,currency));
			return 1;
		}

		//if arrive to that point, the agent was not a valid sender and we return an error
		return 0;
	}

	void Roman::proposeTradeTo(std::string target, std::string type, double valueGood, double valueCurrency)
	{ 
		auto it = listGoods.begin();
		while( it != listGoods.end() )
		{
			if (std::get<0>(*it) == type) break;
			it++;
		}
		//if arrived at the end of the list of goods without finding the specific good, quit the function now
		if (it == listGoods.end()) return;

		// if the connection with the target has been validated
		if( std::find(validSendConnections.begin(), validSendConnections.end(), target) != validSendConnections.end() )
		{
			Agent* uncastedPtr = _world->getAgent(target);
			if (uncastedPtr == NULL)
			{
				std::cout << "function proposeTradeTo" << std::endl;
				std::cout << "unable to get requested agent. clean the connection" << std::endl;
				exit(1);
			}
			Roman* targetPtr = dynamic_cast<Roman*> (uncastedPtr);
			if (targetPtr == NULL)
			{
				std::cout << "function proposeTradeTo" << std::endl;
				std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
				exit(1);
			}

			//if the offer is effectively received, add it
			if (targetPtr->receiveTradeFrom(_id,type,valueGood,valueCurrency))
			{
				listProposedTrades.push_back(make_tuple(target,type,valueGood,valueCurrency));
			}
		}
	}

	void Roman::acceptTradeFrom(std::string source, std::string type, double valueGood, double valueCurrency)
	{
		if( std::find(validRcvConnections.begin(), validRcvConnections.end(), source) != validRcvConnections.end() )
		{
			for (auto it = listReceivedTrades.begin() ; it != listReceivedTrades.end() ; it ++)
			{
				if ((*it) == std::make_tuple(source,type,valueGood,valueCurrency))
				{
					Agent* uncastedPtr = _world->getAgent(source);
					if (uncastedPtr == NULL)
					{
						std::cout << "function acceptTradeFrom" << std::endl;
						std::cout << "unable to get requested agent. clean the connection" << std::endl;
						exit(1);
					}
					Roman* sourcePtr = dynamic_cast<Roman*> (uncastedPtr);
					if (sourcePtr == NULL)
					{
						std::cout << "function acceptTradeFrom" << std::endl;
						std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
						exit(1);
					}

					if( (std::get<0>(getGood("currency")) >= valueCurrency) && (std::get<0>(sourcePtr->getGood(type)) >= valueGood) )
					{
						sourcePtr->sendGoodTo(_id,type,valueGood);
						sourcePtr->addGood("currency", valueCurrency);
						removeGood("currency",valueCurrency);
					}
					sourcePtr->removeProposedTrade(_id,type,valueGood,valueCurrency);
					removeReceivedTrade(source,type,valueGood,valueCurrency);
					break;
				}
			}
		}
	}

	void Roman::refuseTradeFrom(std::string source, std::string type, double valueGood, double valueCurrency)
	{
		Agent* uncastedPtr = _world->getAgent(source);
		if (uncastedPtr == NULL)
		{
			std::cout << "function refuseTradeFrom" << std::endl;
			std::cout << "unable to get requested agent. clean the connection" << std::endl;
			exit(1);
		}
		Roman* sourcePtr = dynamic_cast<Roman*> (uncastedPtr);
		if (sourcePtr == NULL)
		{
			std::cout << "function refuseTradeFrom" << std::endl;
			std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		sourcePtr->removeProposedTrade(_id,type,valueGood,valueCurrency);
		removeReceivedTrade(source,type,valueGood,valueCurrency);
	}

	std::vector<std::tuple<std::string,double,double> > Roman::getReceivedTradesFrom(std::string source)
	{
		std::vector<std::tuple<std::string,double,double> > tmp;
		for (auto it = listReceivedTrades.begin() ; it != listReceivedTrades.end() ; it ++)
		{
			if (std::get<0>(*it) == source)
			{
				tmp.push_back(std::make_tuple(std::get<1>(*it),std::get<2>(*it),std::get<3>(*it)));
			}
		}
		return tmp;
	}

	std::vector<std::tuple<std::string,double,double> > Roman::getProposedTradesTo(std::string target)
	{
		std::vector<std::tuple<std::string,double,double> > tmp;
		for (auto it = listProposedTrades.begin() ; it != listProposedTrades.end() ; it ++)
		{
			if (std::get<0>(*it) == target)
			{
				tmp.push_back(std::make_tuple(std::get<1>(*it),std::get<2>(*it),std::get<3>(*it)));
			}
		}
		return tmp;
	}

	void Roman::removeReceivedTrade(std::string source, std::string type, double value, double currency)
	{
		for (auto it = listReceivedTrades.begin() ; it != listReceivedTrades.end() ;)
		{
			if ((*it) == std::make_tuple(source,type,value,currency))
			{
				listReceivedTrades.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}

	void Roman::removeProposedTrade(std::string source, std::string type, double value, double currency)
	{
		for (auto it = listProposedTrades.begin() ; it != listReceivedTrades.end() ;)
		{
			if ((*it) == std::make_tuple(source,type,value,currency))
			{
				listProposedTrades.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}

	void Roman::killTradesFrom(std::string source)
	{
		//remove traces from the receivedTrades
		for(auto it = listReceivedTrades.begin(); it != listReceivedTrades.end();)
		{
			if(std::get<0>(*it) == source)
			{
				it = listReceivedTrades.erase(it); 
			}
			else
			{
				++it;
			}
		}
	}

	void Roman::killTradesTo(std::string target)
	{
		//remove traces from the proposedTrades
		for(auto it = listProposedTrades.begin(); it != listProposedTrades.end();)
		{
			if(std::get<0>(*it) == target)
			{
				it = listProposedTrades.erase(it); 
			}
			else
			{
				++it;
			}
		}

		Agent* uncastedPtr = _world->getAgent(target);
		if (uncastedPtr == NULL)
		{
			std::cout << "function killTradesTo" << std::endl;
			std::cout << "unable to get requested agent. clean the connection" << std::endl;
			exit(1);
		}
		Roman* targetPtr = dynamic_cast<Roman*> (uncastedPtr);
		if (targetPtr == NULL)
		{
			std::cout << "function killTradesTo" << std::endl;
			std::cout << "dynamic_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		targetPtr->killTradesFrom(_id);
	}

} // namespace Roman

