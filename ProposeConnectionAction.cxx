#include <ProposeConnectionAction.hxx>

#include <World.hxx>

#include <Logger.hxx>

namespace Epnet
{

ProposeConnectionAction::ProposeConnectionAction(std::string target): _target(target)
{
}

ProposeConnectionAction::~ProposeConnectionAction()
{
}

void ProposeConnectionAction::execute( Engine::Agent & agent )
{
	Roman & romanAgent = (Roman&)agent;
	romanAgent.proposeConnectionTo(_target);
}

std::string ProposeConnectionAction::describe() const
{
	return "ProposeConnection action";
}

} // namespace Epnet


