#ifndef __MacmillanController_hxx__
#define __MacmillanController_hxx__

#include <AgentController.hxx>

#include <vector>

namespace Epnet
{

	class MacmillanController : public AgentController
	{
		private:
			int nbGoods;
			int lastIdEssentialGood;
			std::vector<int> needEssential;
			std::vector<int> utility;
			std::vector<double> subjectivePrice;

			std::vector<double> computeGamma();
			int computeR(std::vector<int> sortedId);

			std::vector<int> consumptionPlan(void);

		public:
			// todo remove environment from here
			MacmillanController(void);
			virtual ~MacmillanController();

			virtual void updateState();
			virtual std::list<Engine::Action*> selectActions();
			virtual void updateKnowledge();

			virtual void treatIncomingConnections();
			virtual void treatIncomingTrades();
	};

} // namespace Epnet

#endif // __MacmillanController_hxx__

