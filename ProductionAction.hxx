#ifndef __ProductionAction_hxx__
#define __ProductionAction_hxx__

#include <Action.hxx>
#include <string>

namespace Engine
{
	class Agent;
}

namespace Epnet
{

class ProductionAction : public Engine::Action
{
public:
	ProductionAction();
	virtual ~ProductionAction();
	void execute( Engine::Agent & agent );
	std::string describe() const;

private:
	std::string type;
	int requested;
};

} // namespace Epnet

#endif // __ProductionAction_hxx__
