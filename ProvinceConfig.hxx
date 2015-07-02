#ifndef __ProvinceConfig_hxx__
#define __ProvinceConfig_hxx__

#include <Config.hxx>
#include <vector>
#include <tuple>

namespace Epnet
{

class ProvinceConfig : public Engine::Config
{	
	int _numAgents;

	int _numGoods;
	std::vector<std::tuple<std::string,double,double,double,double,double> > _paramGoods;

	int _numRasters;
	std::vector<std::tuple<std::string,double,double,double> > _paramRasters;
	
	std::string _controllerType;
	std::string _controllerRandom;
	std::tuple<std::string,double,double,double,double,double>  _protoGood;
	std::string _selectionProcess;
	std::string _goodsParam;
	float _mutationRate;
	std::string _innovationProcess;

public:
	ProvinceConfig( const std::string & xmlFile );
	ProvinceConfig( const Engine::Size <int> & size, const int & numSteps );
	virtual ~ProvinceConfig();

	void loadParams();

	friend class Province;
};

} // namespace Epnet

#endif // __ProvinceConfig_hxx__

