#include <SendGoodsAction.hxx>

#include <World.hxx>

#include <Logger.hxx>

namespace Epnet
{

SendGoodsAction::SendGoodsAction(std::string target, std::string type, int requested): _target(target), _type(type), _requested(requested)
{
}

SendGoodsAction::~SendGoodsAction()
{
}

void SendGoodsAction::execute( Engine::Agent & agent )
{
	Roman & romanAgent = (Roman&)agent;
	romanAgent.sendGoodTo(_target,_type,_requested);
}

std::string SendGoodsAction::describe() const
{
	return "SendGoods action";
}

} // namespace Epnet


