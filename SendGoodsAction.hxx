#ifndef __SendGoodsAction_hxx__
#define __SendGoodsAction_hxx__

#include <Action.hxx>
#include <string>
#include <Roman.hxx>

namespace Engine
{
	class Agent;
}

namespace Epnet
{

class SendGoodsAction : public Engine::Action
{
public:
	SendGoodsAction(std::string target,std::string type, int quantity);
	virtual ~SendGoodsAction();
	void execute( Engine::Agent & agent );
	std::string describe() const;

private:
	std::string _target;
	std::string _type;
	int _requested;
};

} // namespace Epnet

#endif // __SendGoodsAction_hxx__

