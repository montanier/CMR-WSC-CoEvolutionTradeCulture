#include <CulturalAction.hxx>

#include <World.hxx>
#include <Province.hxx>

#include <Logger.hxx>

namespace Epnet
{

	CulturalAction::CulturalAction()
	{
	}

	
	CulturalAction::CulturalAction(double mutationRate,std::string selectionProcess,std::string innovationProcess)
	{
	  
	  _mutationRate = mutationRate;
	  _selectionProcess = selectionProcess;
	  _innovationProcess = innovationProcess;
	  
	}

	CulturalAction::~CulturalAction()
	{
	}

	void CulturalAction::execute(Engine::Agent& agent)
	{
		Roman & romanAgent = (Roman&)agent;
		Engine::World * world = agent.getWorld();
		Province & provinceWorld = (Province&) *world;

		std::vector< std::string > allAgents = romanAgent.getValidRcvConnections();


		if(_selectionProcess == "random"){

			if( (Engine::GeneralState::statistics().getUniformDistValue(0,1000))/1000.0 > _mutationRate){

				int wsize = allAgents.size();
				int agId=Engine::GeneralState::statistics().getUniformDistValue(0,wsize) ;
				std::string rId = allAgents[agId];

				Roman & r= (Roman&)(*world->getAgent(rId));

				std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= romanAgent.getListGoods();


				for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
					std::string ressource= std::get<0>(*ot);

					romanAgent.setPrice(ressource,r.getPrice(ressource));
				}
			}
			
		}
		else{
			std::vector< std::string >::iterator it = allAgents.begin();
			bool reproductionDone = 0;
			
			int toGet= provinceWorld.getNumberOfAgents()/(provinceWorld.getTypesOfGood().size())*5;
			
// 			std::cout<<"tg:"<<toGet<<"maxS:"<<max_score<<std::endl;
			
			while(it!= allAgents.end() && !reproductionDone )
				{
					Roman & r= (Roman&)(*world->getAgent(*it));
					double max_score=provinceWorld.getMaxScore(std::get<0>(r.getProducedGood()));	
					if(max_score == 0) max_score = 1; //first it;



			if(Engine::GeneralState::statistics().getUniformDistValue(0,100) < toGet  &&  std::get<0>(r.getProducedGood()) == std::get<0>(romanAgent.getProducedGood())){
					
// 			      std::cout<<"max:"<<max_score<< ","<<r.getScore()/max_score<< "," <<(1-romanAgent.getScore()/max_score)<<std::endl;
					//std::cout<<"good"<<std::get<0>(r.getProducedGood())<<" good "<<std::get<0>(romanAgent.getProducedGood())<<std::endl;
					if(Engine::GeneralState::statistics().getUniformDistValue(0,RAND_MAX)/(double)RAND_MAX < r.getScore()/max_score && (double)Engine::GeneralState::statistics().getUniformDistValue(0,RAND_MAX)/(double)RAND_MAX <(1-romanAgent.getScore()/max_score) ){
						reproductionDone = 1;
						std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= romanAgent.getListGoods();

// 						std::cout<<" done"<<std::endl;

						for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
							std::string ressource= std::get<0>(*ot);

							romanAgent.setPrice(ressource,r.getPrice(ressource));

						}  
					  
					}
					}
					it++;
				
			}
					
		}
		  
			std::vector< std::tuple< std::string, double, double, double, double, double > > allGoods= romanAgent.getListGoods();
			for(std::vector< std::tuple< std::string, double, double, double, double, double > >::iterator ot = allGoods.begin();ot != allGoods.end();ot ++){
				if((Engine::GeneralState::statistics().getUniformDistValue(0,1000))/1000.0 < _mutationRate)
				{
				  
				std::string ressource= std::get<0>(*ot);
				double oldPrice = romanAgent.getPrice(ressource);
				 if(_innovationProcess == "random")
					romanAgent.setPrice(ressource,(double)(Engine::GeneralState::statistics().getUniformDistValue(0,RAND_MAX)/(double)RAND_MAX));//*.95
				  else{

				    if(Engine::GeneralState::statistics().getUniformDistValue(0,2) < 1)
						romanAgent.setPrice(ressource,oldPrice*.95);//
					else
						romanAgent.setPrice(ressource,oldPrice/.95);//
				 }				   
				}
			}  




	}

	std::string CulturalAction::describe() const
	{
		return "Cultural action";
	}

} // namespace Epnet


