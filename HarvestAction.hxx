#ifndef __HarvestAction_hxx__
#define __HarvestAction_hxx__

#include <Action.hxx>
#include <string>

namespace Engine
{
	class Agent;
}

namespace Epnet
{

class HarvestAction : public Engine::Action
{
public:
	HarvestAction(std::string type, int quantity);
	virtual ~HarvestAction();
	void execute( Engine::Agent & agent );
	std::string describe() const;

private:
	std::string type;
	int requested;
};

} // namespace Epnet

#endif // __HarvestAction_hxx__

