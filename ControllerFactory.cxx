#include <ControllerFactory.hxx>

#include <RandomController.hxx>
#include <BasicController.hxx>
#include <MacmillanController.hxx>

namespace Epnet
{

	ControllerFactory* ControllerFactory::_instance = 0;

	ControllerFactory::ControllerFactory()
	{
	}

	ControllerFactory::~ControllerFactory()
	{
	}

	ControllerFactory& ControllerFactory::get()
	{
		if(!_instance)
		{
			_instance = new ControllerFactory;
		}
		return *_instance;
	}

	AgentController* ControllerFactory::makeController(const std::string &controllerName)
	{
		if (controllerName == "random")
		{
			return new RandomController();
		}

		if (controllerName == "macmillan")
		{
			return new MacmillanController();
		}
		if (controllerName == "basic")
		{
			return new BasicController();
		}
	}
	
	AgentController* ControllerFactory::makeController(const std::string &controllerName,double mutationRate,std::string selectionProcess,std::string  innovationProcess)
	{
		if (controllerName == "random")
		{
			return new RandomController();
		}

		if (controllerName == "macmillan")
		{
			return new MacmillanController();
		}
		if (controllerName == "basic")
		{
			return new BasicController(mutationRate,selectionProcess,innovationProcess);
		}
	}


} // namespace Epnet
