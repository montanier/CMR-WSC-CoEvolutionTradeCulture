#include <ProposeTradeAction.hxx>

#include <World.hxx>

#include <Logger.hxx>

namespace Epnet
{

ProposeTradeAction::ProposeTradeAction(std::string target, std::string type, int quantity, int currency): _target(target), _type(type), _quantity(quantity), _currency(currency)
{
}

ProposeTradeAction::~ProposeTradeAction()
{
}

void ProposeTradeAction::execute( Engine::Agent & agent )
{
	Roman & romanAgent = (Roman&)agent;
	romanAgent.proposeTradeTo(_target,_type,_quantity,_currency);
}

std::string ProposeTradeAction::describe() const
{
	return "ProposeTrade action";
}

} // namespace Epnet


