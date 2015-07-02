#ifndef __FunAction_hxx__
#define __FunAction_hxx__

#include <Action.hxx>
#include <string>
#include <Roman.hxx>

namespace Engine
{
	class Agent;
}

namespace Epnet
{

class FunAction : public Engine::Action
{
public:
	FunAction(std::string type, int quantity);
	virtual ~FunAction();
	void execute( Engine::Agent & agent );
	std::string describe() const;

private:
	std::string _type;
	int _requested;
};

} // namespace Epnet

#endif // __FunAction_hxx__

