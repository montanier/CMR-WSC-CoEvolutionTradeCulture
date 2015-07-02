#include <FunAction.hxx>

#include <World.hxx>

#include <Logger.hxx>

namespace Epnet
{

FunAction::FunAction(std::string type, int requested): _type(type), _requested(requested)
{
}

FunAction::~FunAction()
{
}

void FunAction::execute( Engine::Agent & agent )
{
	Roman & romanAgent = (Roman&)agent;
	romanAgent.removeGood(_type,_requested);
}

std::string FunAction::describe() const
{
	return "Fun action";
}

} // namespace Epnet


