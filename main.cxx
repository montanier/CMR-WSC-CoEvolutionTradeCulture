#include <Province.hxx>
#include <ProvinceConfig.hxx>
#include <Exception.hxx>
#include <Config.hxx>

#include <iostream>
#include <cstdlib>
#include <time.h>       /* time */

int main(int argc, char *argv[])
{
	try
	{	
		if(argc>2)
		{
			throw Engine::Exception("USAGE: trade [config file]");
		}		
	
		std::string fileName("config.xml");
		if(argc!=1)
		{
			fileName = argv[1];
		}
		Epnet::Province tradeWorld(new Epnet::ProvinceConfig(fileName));
	
		 std::srand (time(NULL));
		tradeWorld.initialize(argc, argv);
		tradeWorld.run();
	}
	catch( std::exception & exceptionThrown )
	{
		std::cout << "exception thrown: " << exceptionThrown.what() << std::endl;
	}
	return 0;
}

