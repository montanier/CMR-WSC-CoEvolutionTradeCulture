#ifndef __AgentController_hxx__
#define __AgentController_hxx__

#include <Roman.hxx>
#include <Action.hxx>

namespace Epnet
{

	class Roman;

class AgentController
{
protected:
	Roman* _agent;
	
public:
	AgentController(){};
	virtual ~AgentController(){};
	
	virtual void updateState() = 0;
	virtual std::list<Engine::Action*> selectActions() = 0;
	virtual void updateKnowledge() = 0;

	virtual void treatIncomingConnections() = 0;
	virtual void treatIncomingTrades() = 0;

	virtual void setAgent(Roman* ptr){_agent = ptr;};
};

} // namespace Epnet

#endif // __AgentController_hxx__

