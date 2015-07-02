#ifndef __ProposeTradeAction_hxx__
#define __ProposeTradeAction_hxx__

#include <Action.hxx>
#include <string>
#include <Roman.hxx>

namespace Engine
{
	class Agent;
}

namespace Epnet
{

class ProposeTradeAction : public Engine::Action
{
public:
	ProposeTradeAction(std::string target,std::string type, int quantity, int currency);
	virtual ~ProposeTradeAction();
	void execute( Engine::Agent & agent );
	std::string describe() const;

private:
	std::string _target;
	std::string _type;
	int _quantity;
	int _currency;
};

} // namespace Epnet

#endif // __ProposeTradeAction_hxx__

