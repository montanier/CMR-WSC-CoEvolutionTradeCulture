#ifndef __RandomController_hxx__
#define __RandomController_hxx__

#include <AgentController.hxx>

namespace Epnet
{

class RandomController : public AgentController
{
public:
	RandomController();
	virtual ~RandomController();
	
	virtual void updateState();
	virtual std::list<Engine::Action*> selectActions();
	virtual void updateKnowledge();

	virtual void treatIncomingConnections();
	virtual void treatIncomingTrades();
};

} // namespace Epnet

#endif // __RandomController_hxx__

