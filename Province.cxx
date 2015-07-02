#include <Province.hxx>

#include <ProvinceConfig.hxx>
#include <Roman.hxx>
#include <ProductionAction.hxx>
#include <TradeAction.hxx>
#include <ConsumptionAction.hxx>
#include <DynamicRaster.hxx>
#include <Point2D.hxx>
#include <GeneralState.hxx>
#include <Logger.hxx>
#include <Scheduler.hxx>

namespace Epnet
{

	Province::Province(Engine::Config * config, Engine::Scheduler * scheduler ) : World(config, scheduler, false)
	{
		// _maxscore=0.0;
	}

	Province::~Province()
	{
	}

	void Province::createRasters()
	{
		//registerStaticRaster("resources", false);

		const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();
		for (auto it = provinceConfig._paramRasters.begin(); it != provinceConfig._paramRasters.end() ; it++)
		{
			registerDynamicRaster(std::get<0>(*it), true);
			getDynamicRaster(std::get<0>(*it)).setInitValues(std::get<1>(*it), std::get<2>(*it), std::get<3>(*it));
			for(auto index:getBoundaries())
			{
				int value = Engine::GeneralState::statistics().getUniformDistValue(std::get<1>(*it), std::get<2>(*it));
				setMaxValue(std::get<0>(*it), index, value);
			}
			updateRasterToMaxValues(std::get<0>(*it));
		}
	}


	void Province::createAgents()
	{


		std::stringstream logName;
		logName << "agents_" << getId();
		const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();


		//initialize some usefull stuff to know about good : the differents type that exist and the their associated need value
		if(provinceConfig._goodsParam== "random"){
			for (int i = 0; i < provinceConfig._numGoods ; i++)
			{

				std::ostringstream sgoodType;
				sgoodType << "g"<< i;				
				std::string goodType = sgoodType.str();
				_needs.push_back(std::make_tuple(goodType,(double)(Engine::GeneralState::statistics().getUniformDistValue(0,100))/100.0));  
				_maxscore.push_back(std::make_tuple(goodType,0.0));
				_minscore.push_back(std::make_tuple(goodType,0.0));
				_typesOfGood.push_back(goodType);
			}	  
		}
		else
		{
			//In that case each good and the properties of thoses good have to be manually set by the user in the config file


			for (auto it = provinceConfig._paramGoods.begin(); it != provinceConfig._paramGoods.end() ; it++)
			{
				_needs.push_back(std::make_tuple(std::get<0>(*it),(double)(Engine::GeneralState::statistics().getUniformDistValue(0,100))/100.0));  
				_maxscore.push_back(std::make_tuple(std::get<0>(*it),0.0));  
				_minscore.push_back(std::make_tuple(std::get<0>(*it),0.0));
				_typesOfGood.push_back(std::get<0>(*it));

			}
		}	

		for(int i=0; i<provinceConfig._numAgents; i++)
		{

			if((i%getNumTasks())==getId())
			{
				std::ostringstream oss;
				oss << "Roman_" << i;
				Roman * agent = new Roman(oss.str(),provinceConfig._controllerType,provinceConfig._mutationRate,provinceConfig._selectionProcess,provinceConfig._innovationProcess);
				addAgent(agent);
				//position is actually not interesting
				agent->setRandomPosition();
				//currency is not interesting in itself. that may be changed
				//currency has no price by itself
				if(provinceConfig._goodsParam== "random")
				{

					std::tuple< std::string, double, double, double, double, double > protoGood = provinceConfig._protoGood;
					for (int i = 0; i < provinceConfig._numGoods ; i++)
					{

						std::ostringstream sgoodType;
						sgoodType << "g"<< i;				
						std::string goodType = sgoodType.str();
						//id, maxQuantity, price, need and production rate of the good
						agent->addGoodType(goodType,std::get<2>(protoGood),std::get<3>(protoGood),std::get<4>(protoGood),std::get<5>(protoGood));
						//add init quantity to new good
						agent->addGood(goodType,std::get<1>(protoGood));
						//the protoGood is used to calibrate all other goods. 

						//set a random properties for each goods
						if(agent->getPrice(goodType)<0)agent->setPrice(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);// market clearing price : 1.0/(i+1)
						if(agent->getQuantity(goodType)<0)agent->setQuantity(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);
						if(agent->getProductionRate(goodType)<0)agent->setProductionRate(goodType,(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0);
						//---------------/*
						//set the need value for each good. Remember: in Basic Simulation the need is the same for everyone
						agent->setNeed(goodType,std::get<1>(_needs[i]));
					}			



					//Set producedGood
					int randg = i%provinceConfig._numGoods;
					std::tuple< std::string, double, double, double, double, double > producedGood = agent->getListGoods()[randg];
					agent->setProductionRate(std::get<0>(producedGood),1.0);

				}
				else{
					for (auto it = provinceConfig._paramGoods.begin(); it != provinceConfig._paramGoods.end() ; it++)
					{
						//id, maxQuantity, price, need and production rate of the good
						agent->addGoodType(std::get<0>(*it),std::get<2>(*it),std::get<3>(*it),std::get<4>(*it),std::get<5>(*it));
						//add init quantity to new good
						agent->addGood(std::get<0>(*it),std::get<1>(*it));

						//set a random price for each goods

						agent->setPrice(std::get<0>(*it),(double)Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0); //TODO:demiurge's job


						//---------------
						//set the need value for each good. Remember: in Basic Simulation the need is the same for everyone
						std::vector<std::tuple<std::string,double> >::iterator need = std::find_if(_needs.begin(), _needs.end(), [=](const std::tuple<std::string,double>& good) {return std::get<0>(good) == std::get<0>(*it);});

						if ( need != _needs.end() )
						{				  
							agent->setNeed(std::get<0>(*it),std::get<1>(*need));
						}
					}			

				}
				log_INFO(logName.str(), getWallTime() << " new agent: " << agent);


				//loop for initialize the connection of the current agent with all previously created agents.
				for(int j=(i-1); j>=0; j--)
				{
					std::ostringstream ossb;
					ossb << "Roman_" << j;
					this->buildTwoWayConnection(oss.str(),ossb.str());//TODO here check the this->network
				}
			}
		}

	}

	double Province::getMaxScore(std::string good)
	{
		std::vector< std::tuple< std::string, double > >::iterator it = _maxscore.begin();

		while(it!= _maxscore.end()){
			if(std::get<0>(*it)== good)
				return std::get<1>(*it);
			it++;

		}
		return -1; //error good not found
	}


	void Province::setMaxScore(std::string good, double score)
	{
		std::vector< std::tuple< std::string, double > >::iterator it = _maxscore.begin();

		while(it!= _maxscore.end()){
			if(std::get<0>(*it)== good){
				std::get<1>(*it) = score ;
				return;
			}
			it++;
		}

	}

	double Province::getMinScore(std::string good)
	{
		std::vector< std::tuple< std::string, double > >::iterator it = _minscore.begin();

		while(it!= _minscore.end()){
			if(std::get<0>(*it)== good)
				return std::get<1>(*it);
			it++;

		}
		return -1; //error good not found
	}


	void Province::setMinScore(std::string good, double score)
	{
		std::vector< std::tuple< std::string, double > >::iterator it = _minscore.begin();

		while(it!= _minscore.end()){
			if(std::get<0>(*it)== good){
				std::get<1>(*it) = score ;
				return;
			}
			it++;
		}

	}



	void Province::proposeConnection(std::string source, std::string target)
	{
		Roman* sourcePtr = dynamic_cast<Roman*> (getAgent(source));
		if (sourcePtr == NULL)
		{
			std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}
		sourcePtr->proposeConnectionTo(target);
	}

	void Province::buildConnection(std::string source, std::string target)
	{
		Roman* sourcePtr = dynamic_cast<Roman*> (getAgent(source));
		if (sourcePtr == NULL)
		{
			std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		Roman* targetPtr = dynamic_cast<Roman*> (getAgent(target));
		if (targetPtr == NULL)
		{
			std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		sourcePtr->proposeConnectionTo(target);
		targetPtr->acceptConnectionFrom(source);
	}

	void Province::killConnection(std::string source, std::string target)
	{
		Roman* sourcePtr = dynamic_cast<Roman*> (getAgent(source));
		if (sourcePtr == NULL)
		{
			std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		sourcePtr->killConnectionTo(target);
	}

	void Province::proposeTwoWayConnection(std::string source, std::string target)
	{
		Roman* sourcePtr = dynamic_cast<Roman*> (getAgent(source));
		if (sourcePtr == NULL)
		{
			std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		Roman* targetPtr = dynamic_cast<Roman*> (getAgent(target));
		if (targetPtr == NULL)
		{
			std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		sourcePtr->proposeConnectionTo(target);
		targetPtr->proposeConnectionTo(source);
	}

	void Province::step(){

		std::stringstream logName;
		logName << "simulation_" << getId();
		log_INFO(logName.str(), getWallTime() << " executing step: " << _step );

		if(_step%_config->getSerializeResolution()==0)
		{
			_scheduler->serializeRasters(_step);
			_scheduler->serializeAgents(_step);
			log_DEBUG(logName.str(), getWallTime() << " step: " << _step << " serialization done");
		}
		stepEnvironment();
		log_DEBUG(logName.str(), getWallTime() << " step: " << _step << " has executed step environment");


		std::vector<int>randomizer; //a list where all agent_id is stored and that we will use to randomize
		int i =0;
		const ProvinceConfig & provinceConfig = (const ProvinceConfig&)getConfig();


		//	if(provinceConfig._selectionProcess == "trade"){
		//Good's Production
		for( std::list< Engine::AgentPtr >::iterator ag=_agents.begin(); ag != _agents.end();ag++){
			Engine::AgentPtr oneA = *ag;
			Roman * r = (Roman *) (oneA.get());
			ProductionAction * PA = new ProductionAction();
			PA->execute(*r);
			randomizer.push_back(i);
			i++;
		}


		std::random_shuffle(randomizer.begin(),randomizer.end());


		//Trade Action
		for( std::vector< int >::iterator it = randomizer.begin();it != randomizer.end();it++){
			std::ostringstream oss;
			oss << "Roman_" << *it;
			Roman * r= (Roman *) getAgent(oss.str());
			TradeAction * TA = new TradeAction();
			TA->execute(*r);

		}


		//Good's consumption
		for( std::list< Engine::AgentPtr >::iterator ag=_agents.begin(); ag != _agents.end();ag++){
			Engine::AgentPtr oneA = *ag;
			Roman * r = (Roman *) (oneA.get());
			ConsumptionAction * CA = new ConsumptionAction();
			CA->execute(*r);
		}

		//	}

		//Cultural innovation and transmission
		if(_step%10 == 0 && _step > 0){


			if( provinceConfig._selectionProcess== "random"){

				for( std::list< Engine::AgentPtr >::iterator ag=_agents.begin(); ag != _agents.end();ag++){
					Engine::AgentPtr oneA = *ag;
					Roman * r1 = (Roman *) (oneA.get());

					if( Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0 > provinceConfig._mutationRate){
						std::vector< std::string > allAgents = r1->getValidRcvConnections();

						int wsize = allAgents.size();
						int agId=Engine::GeneralState::statistics().getUniformDistValue(0,wsize-1) ;

						std::string rId = allAgents[agId];
						Roman & r2= (Roman&)(*getAgent(rId));

						std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= r1->getListGoods();


						for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
							std::string ressource= std::get<0>(*ot);
							r1->setPrice(ressource,r2.getPrice(ressource));
						}
					}
				}


			}

			else{

				std::random_shuffle(_typesOfGood.begin(),_typesOfGood.end());


				for(std::vector<std::string>::iterator good = _typesOfGood.begin(); good != _typesOfGood.end();good++){
					int toGet= getNumberOfAgents()/(getTypesOfGood().size()) * .02;

					std::vector<std::string> toChange;

					int MaxTry=2*getNumberOfAgents();
					int tr=0;
					
					//Following Gintis 2006, we look for 2% of the best agent for each market. 
					//This is highly time consuming if only so we set a max limit and it's badly implementend (should have list of the agent sorted by the agent's score)
					while(toChange.size() < toGet && tr < MaxTry ){

						std::ostringstream oss;
						int rint = (Engine::GeneralState::statistics().getUniformDistValue(0,_agents.size()-1));
						oss << "Roman_" << rint;
						Roman * r= (Roman *) getAgent(oss.str());
						if(std::get<0>(r->getProducedGood()) == *good){
							double relScore = (r->getScore()-getMinScore(*good))/(getMaxScore(*good)-getMinScore(*good));

							if((Engine::GeneralState::statistics().getUniformDistValue(0,RAND_MAX)/(double)RAND_MAX) <= (relScore) )
								toChange.push_back(r->getId());
						}
						tr++;

					}

					while(toChange.size() > 0){

						std::ostringstream oss;
						int rint = (Engine::GeneralState::statistics().getUniformDistValue(0,_agents.size()-1));
						oss << "Roman_" << rint;

						Roman * r= (Roman *) getAgent(oss.str());
						if(std::get<0>(r->getProducedGood()) == *good){

							double relScore = (r->getScore()-getMinScore(*good))/(getMaxScore(*good)-getMinScore(*good));

							if(Engine::GeneralState::statistics().getUniformDistValue(0,RAND_MAX)/(double)RAND_MAX <= 1-relScore){

								std::string replaced= toChange.back();

								toChange.pop_back();
								copyPrice(replaced,r->getId()); 
							}
						}

					}
				}
			}

			
			//During the cultural step, there is also the mutation of the prices 
			for( std::list< Engine::AgentPtr >::iterator ag=_agents.begin(); ag != _agents.end();ag++){
		
				Engine::AgentPtr oneA = *ag;
				Roman * r = (Roman *) (oneA.get());
		
				std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= r->getListGoods();
				for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
					std::string ressource= std::get<0>(*ot);
					if(Engine::GeneralState::statistics().getUniformDistValue(0,1000)/1000.0 < provinceConfig._mutationRate)
					{

						double oldPrice = r->getPrice(ressource);
						if(provinceConfig._innovationProcess == "random")
							r->setPrice(ressource,(double)(Engine::GeneralState::statistics().getUniformDistValue(0,RAND_MAX)/(double)RAND_MAX));//*.95
						else{

							if(Engine::GeneralState::statistics().getUniformDistValue(0,2) < 1)
								r->setPrice(ressource,oldPrice+Engine::GeneralState::statistics().getUniformDistValue(0,50)/1000.0 );//
							else
								r->setPrice(ressource,oldPrice-(Engine::GeneralState::statistics().getUniformDistValue(0,50))/1000.0);//
							if(r->getPrice(ressource)<0)r->setPrice(ressource,0.0);
						}				   
					}
					setMaxScore(ressource,0.0);
					setMinScore(ressource,RAND_MAX);

				}  
				r->setScore(0.0);

			}
		}	


		// 	_scheduler->executeAgents(); //the scheduler's job is done sequentially before
		_scheduler->removeAgents();
		log_INFO(logName.str(), getWallTime() << " finished step: " << _step);
	}


	//Jsut a small function to hard copy all the prices of on agents into another agent
	void Province::copyPrice(const std::string replaced, std::string replacer)
	{
		Roman & r1= (Roman&) *getAgent(replaced);
		Roman & r2= (Roman&) *getAgent(replacer);
		std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= r1.getListGoods();
		for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
			std::string ressource= std::get<0>(*ot);

			r1.setPrice(ressource,r2.getPrice(ressource));
		}
	}


	void Province::buildTwoWayConnection(std::string source, std::string target)
	{
		buildConnection(source,target);
		buildConnection(target,source);
	}


	void Province::killTwoWayConnection(std::string source, std::string target)
	{
		Roman* sourcePtr = dynamic_cast<Roman*> (getAgent(source));
		if (sourcePtr == NULL)
		{
			std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		Roman* targetPtr = dynamic_cast<Roman*> (getAgent(target));
		if (targetPtr == NULL)
		{
			std::cout << "dynamice_cast from Agent* to Roman* fail" << std::endl;
			exit(1);
		}

		sourcePtr->killConnectionTo(target);
		targetPtr->killConnectionTo(source);
	}

} // namespace Roman

