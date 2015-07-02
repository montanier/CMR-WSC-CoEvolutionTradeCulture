#ifndef __TradeAction_hxx__
#define __TradeAction_hxx__

#include <Action.hxx>
#include <string>

namespace Engine
{
	class Agent;
}

namespace Epnet
{

class TradeAction : public Engine::Action
{
public:
	TradeAction();
	virtual ~TradeAction();
	void execute( Engine::Agent & agent );
	std::string describe() const;

private:
    double getRequestedQuantity(Engine::Agent& r, std::string goodWanted);
	std::string type;
	int requested;
};

} // namespace Epnet

#endif // __TradeAction_hxx__
