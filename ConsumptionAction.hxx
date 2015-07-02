#ifndef __ConsumptionAction_hxx__
#define __ConsumptionAction_hxx__

#include <Action.hxx>
#include <string>

namespace Engine
{
	class Agent;
}

namespace Epnet
{

class ConsumptionAction : public Engine::Action
{
public:
	ConsumptionAction();
	virtual ~ConsumptionAction();
	void execute( Engine::Agent & agent );
	std::string describe() const;

};

} // namespace Epnet

#endif // __ConsumptionAction_hxx__
