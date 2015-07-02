
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Pandora Sequential Unit Tests"

#include "RomanTest.hxx"
#include "ProvinceTest.hxx"
#include "ProvinceConfig.hxx"
#include <Config.hxx>
#include <World.hxx>
#include <Point2D.hxx>
#include <Size.hxx>
#include <ShpLoader.hxx>
#include <GeneralState.hxx>
#include <Exception.hxx>

#include <boost/test/unit_test.hpp>

namespace Epnet
{


BOOST_AUTO_TEST_SUITE( KillTradeTests )

BOOST_AUTO_TEST_CASE( KillNonExistingTrade ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 2), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->killTradesTo("agent_1");

	std::vector<std::tuple<std::string,std::string,double,double> > proposedTradesAgent0 = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > proposedTradesAgent1 = myAgent1->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > receivedTradesAgent0 = myAgent0->getReceivedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > receivedTradesAgent1 = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(proposedTradesAgent0.size(),0);
	BOOST_CHECK_EQUAL(proposedTradesAgent1.size(),0);
	BOOST_CHECK_EQUAL(receivedTradesAgent0.size(),0);
	BOOST_CHECK_EQUAL(receivedTradesAgent1.size(),0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( KillProposedTrade ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 2), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->addGoodType("ess-a",100.0,0.0,0.0);
	myAgent0->addGood("ess-a",50.0);
	myAgent0->addGoodType("ess-b",100.0,0.0,0.0);
	myAgent0->addGood("ess-b",50.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("currency",50.0);

	myAgent1->addGoodType("ess-a",100.0,0.0,0.0);
	myAgent1->addGood("ess-a",50.0);
	myAgent1->addGoodType("ess-b",100.0,0.0,0.0);
	myAgent1->addGood("ess-b",50.0);
	myAgent1->addGoodType("currency",100.0,0.0,0.0);
	myAgent1->addGood("currency",50.0);

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent1->proposeConnectionTo("agent_0");
	myAgent0->acceptConnectionFrom("agent_1");

	myAgent0->proposeTradeTo("agent_1","ess-a",10,10);
	myAgent1->proposeTradeTo("agent_0","ess-a",10,10);

	std::vector<std::tuple<std::string,std::string,double,double> > proposedTradesAgent0 = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > proposedTradesAgent1 = myAgent1->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > receivedTradesAgent0 = myAgent0->getReceivedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > receivedTradesAgent1 = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(proposedTradesAgent0.size(),1);
	BOOST_CHECK_EQUAL(proposedTradesAgent1.size(),1);
	BOOST_CHECK_EQUAL(receivedTradesAgent0.size(),1);
	BOOST_CHECK_EQUAL(receivedTradesAgent1.size(),1);

	myAgent0->killTradesTo("agent_1");

	proposedTradesAgent0 = myAgent0->getProposedTrades();
	proposedTradesAgent1 = myAgent1->getProposedTrades();
	receivedTradesAgent0 = myAgent0->getReceivedTrades();
	receivedTradesAgent1 = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(proposedTradesAgent0.size(),0);
	BOOST_CHECK_EQUAL(proposedTradesAgent1.size(),1);
	BOOST_CHECK_EQUAL(receivedTradesAgent0.size(),1);
	BOOST_CHECK_EQUAL(receivedTradesAgent1.size(),0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( KillTwoProposedTrade ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 2), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->addGoodType("ess-a",100.0,0.0,0.0);
	myAgent0->addGood("ess-a",50.0);
	myAgent0->addGoodType("ess-b",100.0,0.0,0.0);
	myAgent0->addGood("ess-b",50.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("currency",50.0);

	myAgent1->addGoodType("ess-a",100.0,0.0,0.0);
	myAgent1->addGood("ess-a",50.0);
	myAgent1->addGoodType("ess-b",100.0,0.0,0.0);
	myAgent1->addGood("ess-b",50.0);
	myAgent1->addGoodType("currency",100.0,0.0,0.0);
	myAgent1->addGood("currency",50.0);

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->proposeTradeTo("agent_1","ess-a",10,10);
	myAgent0->proposeTradeTo("agent_1","ess-a",10,10);

	std::vector<std::tuple<std::string,std::string,double,double> > proposedTradesAgent0 = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > proposedTradesAgent1 = myAgent1->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > receivedTradesAgent0 = myAgent0->getReceivedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > receivedTradesAgent1 = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(proposedTradesAgent0.size(),2);
	BOOST_CHECK_EQUAL(proposedTradesAgent1.size(),0);
	BOOST_CHECK_EQUAL(receivedTradesAgent0.size(),0);
	BOOST_CHECK_EQUAL(receivedTradesAgent1.size(),2);

	myAgent0->killTradesTo("agent_1");

	proposedTradesAgent0 = myAgent0->getProposedTrades();
	proposedTradesAgent1 = myAgent1->getProposedTrades();
	receivedTradesAgent0 = myAgent0->getReceivedTrades();
	receivedTradesAgent1 = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(proposedTradesAgent0.size(),0);
	BOOST_CHECK_EQUAL(proposedTradesAgent1.size(),0);
	BOOST_CHECK_EQUAL(receivedTradesAgent0.size(),0);
	BOOST_CHECK_EQUAL(receivedTradesAgent1.size(),0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()




BOOST_AUTO_TEST_SUITE( DeathTests )

BOOST_AUTO_TEST_CASE( DeathHappens ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 3), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent0->addGoodType("ess-a",100.0,0.0,1.0);
	myAgent1->addGoodType("ess-a",100.0,0.0,1.0);

	int count = 0;
	for(auto it = myWorld.beginAgents() ; it != myWorld.endAgents() ; it++)
	{
		count++;
	}
	BOOST_CHECK_EQUAL(count,2);

	myWorld.stepTest();

	count = 0;
	for(auto it = myWorld.beginAgents() ; it != myWorld.endAgents() ; it++)
	{
		count++;
	}
	BOOST_CHECK_EQUAL(count,0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( DeathCleanProposition ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 2), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->addGoodType("ess-a",100.0,0.0,1.0);
	myAgent0->addGoodType("ess-b",100.0,0.0,1.0);

	myAgent1->addGoodType("ess-a",100.0,0.0,1.0);
	myAgent1->addGood("ess-a",50.0);
	myAgent1->addGoodType("ess-b",100.0,0.0,1.0);
	myAgent1->addGood("ess-b",50.0);

	myAgent0->proposeConnectionTo("agent_1");

	//myWorld.stepTest();
	myAgent0->updateKnowledge();
	myAgent0->updateState();
	myWorld.removeAgents();

	int count = 0;
	for(auto it = myWorld.beginAgents() ; it != myWorld.endAgents() ; it++)
	{
		count++;
	}

	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();

	BOOST_CHECK_EQUAL(receivedAgent1.size(),0);
	BOOST_CHECK_EQUAL(count,1);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( DeathCleanConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 2), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->addGoodType("ess-a",100.0,0.0,1.0);
	myAgent0->addGoodType("ess-b",100.0,0.0,1.0);

	myAgent1->addGoodType("ess-a",100.0,0.0,1.0);
	myAgent1->addGood("ess-a",50.0);
	myAgent1->addGoodType("ess-b",100.0,0.0,1.0);
	myAgent1->addGood("ess-b",50.0);

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myWorld.stepTest();

	int count = 0;
	for(auto it = myWorld.beginAgents() ; it != myWorld.endAgents() ; it++)
	{
		count++;
	}

	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();

	BOOST_CHECK_EQUAL(receivedAgent1.size(),0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(),0);
	BOOST_CHECK_EQUAL(count,1);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( DeathCleanTradeProposition ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 2), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->addGoodType("ess-a",100.0,0.0,1.0);
	myAgent0->addGood("ess-a",0.5);
	myAgent0->addGoodType("ess-b",100.0,0.0,1.0);
	myAgent0->addGood("ess-b",0.5);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("currency",1.0);

	myAgent1->addGoodType("ess-a",100.0,0.0,1.0);
	myAgent1->addGood("ess-a",50.0);
	myAgent1->addGoodType("ess-b",100.0,0.0,1.0);
	myAgent1->addGood("ess-b",50.0);

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");
	myAgent0->proposeTradeTo("agent_1","ess-a",50,50);


	std::vector<std::tuple<std::string,std::string,double,double> > receivedTradesAgent1Before = myAgent1->getReceivedTrades();
	BOOST_CHECK_EQUAL(receivedTradesAgent1Before.size(),1);

	myAgent0->updateKnowledge();
	myAgent0->updateState();
	myWorld.removeAgents();
	//myWorld.stepTest();

	int count = 0;
	for(auto it = myWorld.beginAgents() ; it != myWorld.endAgents() ; it++)
	{
		count++;
	}
	BOOST_CHECK_EQUAL(count,1);

	std::vector<std::tuple<std::string,std::string,double,double> > receivedTradesAgent1 = myAgent1->getReceivedTrades();
	BOOST_CHECK_EQUAL(receivedTradesAgent1.size(),0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( DeathCleanTradeProposed ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 2), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->addGoodType("ess-a",100.0,0.0,1.0);
	myAgent0->addGood("ess-a",50.0);
	myAgent0->addGoodType("ess-b",100.0,0.0,1.0);
	myAgent0->addGood("ess-b",50.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("currency",1.0);

	myAgent1->addGoodType("ess-a",100.0,0.0,1.0);
	myAgent1->addGood("ess-a",0.5);
	myAgent1->addGoodType("ess-b",100.0,0.0,1.0);
	myAgent1->addGood("ess-b",0.5);

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");
	myAgent0->proposeTradeTo("agent_1","ess-a",50,50);


	std::vector<std::tuple<std::string,std::string,double,double> > receivedTradesAgent1Before = myAgent1->getReceivedTrades();
	BOOST_CHECK_EQUAL(receivedTradesAgent1Before.size(),1);

	myAgent1->updateKnowledge();
	myAgent1->updateState();
	myWorld.removeAgents();
	//myWorld.stepTest();

	int count = 0;
	for(auto it = myWorld.beginAgents() ; it != myWorld.endAgents() ; it++)
	{
		count++;
	}
	BOOST_CHECK_EQUAL(count,1);

	std::vector<std::tuple<std::string,std::string,double,double> > proposedTradesAgent0 = myAgent0->getProposedTrades();
	BOOST_CHECK_EQUAL(proposedTradesAgent0.size(),0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( VisibleGoods )

BOOST_AUTO_TEST_CASE( getGoods ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 3), Province::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent0->addGoodType("B",50.0,0.0,0.0);
	myAgent0->addGood("B",30.0);

	std::vector<std::tuple<std::string, double, double, double, double> > listAgent0 = myAgent1->getListGoodsFrom("agent_0");

	BOOST_CHECK_EQUAL(std::get<0>(listAgent0[0]),"A");
	BOOST_CHECK_EQUAL(std::get<1>(listAgent0[0]),20.0);
	BOOST_CHECK_EQUAL(std::get<2>(listAgent0[0]),100.0);
	BOOST_CHECK_EQUAL(std::get<0>(listAgent0[1]),"B");
	BOOST_CHECK_EQUAL(std::get<1>(listAgent0[1]),30.0);
	BOOST_CHECK_EQUAL(std::get<2>(listAgent0[1]),50.0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test

