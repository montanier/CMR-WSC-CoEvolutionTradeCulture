#ifndef __ProposeConnectionAction_hxx__
#define __ProposeConnectionAction_hxx__

#include <Action.hxx>
#include <string>
#include <Roman.hxx>

namespace Engine
{
	class Agent;
}

namespace Epnet
{

class ProposeConnectionAction : public Engine::Action
{
public:
	ProposeConnectionAction(std::string target);
	virtual ~ProposeConnectionAction();
	void execute( Engine::Agent & agent );
	std::string describe() const;

private:
	std::string _target;
};

} // namespace Epnet

#endif // __ProposeConnectionAction_hxx__

