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

BOOST_AUTO_TEST_SUITE( ConnectionFromSourceAgent )

BOOST_AUTO_TEST_CASE( AgentAskConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 1);
	BOOST_CHECK_EQUAL(proposedAgent0[0], "agent_1");

	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 1);
	BOOST_CHECK_EQUAL(receivedAgent1[0], "agent_0");

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AgentAsk2Times ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent0->proposeConnectionTo("agent_1");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 1);
	BOOST_CHECK_EQUAL(proposedAgent0[0], "agent_1");

	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 1);
	BOOST_CHECK_EQUAL(receivedAgent1[0], "agent_0");

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AgentProposeConnectionToNonExisting ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_!");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AgentProposeConnectionToSelf ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_0");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);

	std::vector<std::string> receivedAgent0 = myAgent0->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent0.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AgentAcceptConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], "agent_1");

	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent1[0], "agent_0");

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AgentsBuildTwoWayConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent1->proposeConnectionTo("agent_0");
	myAgent0->acceptConnectionFrom("agent_1");

	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<std::string> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<std::string> validRcvAgent0 = myAgent0->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], "agent_1");
	BOOST_CHECK_EQUAL(receivedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent0[0], "agent_1");

	std::vector<std::string> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<std::string> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent1[0], "agent_0");
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent1[0], "agent_0");

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( DuplicateConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], "agent_1");

	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent1[0], "agent_0");

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AgentRefuseConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->refuseConnectionFrom("agent_0");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);

	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( FailAcceptIfNotSent ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myAgent1->acceptConnectionFrom("agent_0");
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);

	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( RefuseConnectionNotSent ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myAgent1->refuseConnectionFrom("agent_0");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);

	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SourceKillOneWayConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");
	myAgent0->killConnectionTo("agent_1");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);

	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( TargetKillOneWayConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");
	myAgent1->killConnectionFrom("agent_0");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);

	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( killTwoWayConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent1->proposeConnectionTo("agent_0");
	myAgent0->acceptConnectionFrom("agent_1");

	myAgent1->killConnections("agent_0");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);

	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( killHalfOfTwoWayConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent1->proposeConnectionTo("agent_0");
	myAgent0->acceptConnectionFrom("agent_1");

	myAgent1->killConnectionTo("agent_0");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<std::string> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<std::string> validRcvAgent0 = myAgent0->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], "agent_1");
	BOOST_CHECK_EQUAL(receivedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 0);

	std::vector<std::string> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<std::string> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent1[0], "agent_0");

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( KillConnectionFromTests )

BOOST_AUTO_TEST_CASE( KillNonExistingConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 2), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->killConnectionFrom("agent_1");

	std::vector<std::string> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<std::string> validRcvAgent0 = myAgent0->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();

	BOOST_CHECK_EQUAL(receivedAgent0.size(),0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(),0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(),0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(),0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( KillConnectionProposedOneWay ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 2), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");

	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<std::string> validRcvAgent0 = myAgent0->getValidRcvConnections();
	std::vector<std::string> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();

	BOOST_CHECK_EQUAL(proposedAgent0.size(),1);
	BOOST_CHECK_EQUAL(receivedAgent0.size(),0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(),0);
	BOOST_CHECK_EQUAL(proposedAgent1.size(),0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(),1);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(),0);

	myAgent1->killConnectionFrom("agent_0");

	proposedAgent0 = myAgent0->getProposedConnections();
	receivedAgent0 = myAgent0->getReceivedConnections();
	validRcvAgent0 = myAgent0->getValidRcvConnections();
	proposedAgent1 = myAgent1->getProposedConnections();
	receivedAgent1 = myAgent1->getReceivedConnections();
	validRcvAgent1 = myAgent1->getValidRcvConnections();

	BOOST_CHECK_EQUAL(proposedAgent0.size(),0);
	BOOST_CHECK_EQUAL(receivedAgent0.size(),0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(),0);
	BOOST_CHECK_EQUAL(proposedAgent1.size(),0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(),0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(),0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( KillConnectionProposedTwoWays ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 2), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->proposeConnectionTo("agent_0");

	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<std::string> validRcvAgent0 = myAgent0->getValidRcvConnections();
	std::vector<std::string> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();

	BOOST_CHECK_EQUAL(proposedAgent0.size(),1);
	BOOST_CHECK_EQUAL(proposedAgent0[0],"agent_1");
	BOOST_CHECK_EQUAL(receivedAgent0.size(),1);
	BOOST_CHECK_EQUAL(receivedAgent0[0],"agent_1");
	BOOST_CHECK_EQUAL(validRcvAgent0.size(),0);
	BOOST_CHECK_EQUAL(proposedAgent1.size(),1);
	BOOST_CHECK_EQUAL(proposedAgent1[0],"agent_0");
	BOOST_CHECK_EQUAL(receivedAgent1.size(),1);
	BOOST_CHECK_EQUAL(receivedAgent1[0],"agent_0");
	BOOST_CHECK_EQUAL(validRcvAgent1.size(),0);

	myAgent1->killConnectionFrom("agent_0");

	proposedAgent0 = myAgent0->getProposedConnections();
	receivedAgent0 = myAgent0->getReceivedConnections();
	validRcvAgent0 = myAgent0->getValidRcvConnections();
	proposedAgent1 = myAgent1->getProposedConnections();
	receivedAgent1 = myAgent1->getReceivedConnections();
	validRcvAgent1 = myAgent1->getValidRcvConnections();

	BOOST_CHECK_EQUAL(proposedAgent0.size(),0);
	BOOST_CHECK_EQUAL(receivedAgent0.size(),1);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(),0);
	BOOST_CHECK_EQUAL(proposedAgent1.size(),1);
	BOOST_CHECK_EQUAL(receivedAgent1.size(),0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(),0);

	myAgent0->killConnectionFrom("agent_1");

	proposedAgent0 = myAgent0->getProposedConnections();
	receivedAgent0 = myAgent0->getReceivedConnections();
	validRcvAgent0 = myAgent0->getValidRcvConnections();
	proposedAgent1 = myAgent1->getProposedConnections();
	receivedAgent1 = myAgent1->getReceivedConnections();
	validRcvAgent1 = myAgent1->getValidRcvConnections();

	BOOST_CHECK_EQUAL(proposedAgent0.size(),0);
	BOOST_CHECK_EQUAL(receivedAgent0.size(),0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(),0);
	BOOST_CHECK_EQUAL(proposedAgent1.size(),0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(),0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(),0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( KillConnectionValidOneWay ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 2), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	std::vector<std::string> validRcvAgent0 = myAgent0->getValidRcvConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> validSendAgent1 = myAgent1->getValidSendConnections();

	BOOST_CHECK_EQUAL(validRcvAgent0.size(),0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(),1);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(),1);
	BOOST_CHECK_EQUAL(validSendAgent1.size(),0);

	myAgent1->killConnectionFrom("agent_0");

	validRcvAgent0 = myAgent0->getValidRcvConnections();
	validSendAgent0 = myAgent0->getValidSendConnections();
	validRcvAgent1 = myAgent1->getValidRcvConnections();
	validSendAgent1 = myAgent1->getValidSendConnections();

	BOOST_CHECK_EQUAL(validRcvAgent0.size(),0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(),0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(),0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(),0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( KillConnectionValidTwoWays ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 2), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");
	myAgent1->proposeConnectionTo("agent_0");
	myAgent0->acceptConnectionFrom("agent_1");

	std::vector<std::string> validRcvAgent0 = myAgent0->getValidRcvConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> validSendAgent1 = myAgent1->getValidSendConnections();

	BOOST_CHECK_EQUAL(validRcvAgent0.size(),1);
	BOOST_CHECK_EQUAL(validSendAgent0.size(),1);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(),1);
	BOOST_CHECK_EQUAL(validSendAgent1.size(),1);

	myAgent1->killConnectionFrom("agent_0");

	validRcvAgent0 = myAgent0->getValidRcvConnections();
	validSendAgent0 = myAgent0->getValidSendConnections();
	validRcvAgent1 = myAgent1->getValidRcvConnections();
	validSendAgent1 = myAgent1->getValidSendConnections();

	BOOST_CHECK_EQUAL(validRcvAgent0.size(),1);
	BOOST_CHECK_EQUAL(validSendAgent0.size(),0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(),0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(),1);

	myAgent0->killConnectionFrom("agent_1");

	validRcvAgent0 = myAgent0->getValidRcvConnections();
	validSendAgent0 = myAgent0->getValidSendConnections();
	validRcvAgent1 = myAgent1->getValidRcvConnections();
	validSendAgent1 = myAgent1->getValidSendConnections();

	BOOST_CHECK_EQUAL(validRcvAgent0.size(),0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(),0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(),0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(),0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( ConnectionFromWorld )

BOOST_AUTO_TEST_CASE( WorldProposeConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myWorld.proposeConnection("agent_0", "agent_1");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 1);
	BOOST_CHECK_EQUAL(proposedAgent0[0], "agent_1");

	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 1);
	BOOST_CHECK_EQUAL(receivedAgent1[0], "agent_0");

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( WorldBuildConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myWorld.buildConnection("agent_0", "agent_1");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], "agent_1");

	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent1[0], "agent_0");

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( WorldKillConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myWorld.buildConnection("agent_0", "agent_1");
	myWorld.killConnection("agent_0", "agent_1");

	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);

	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( WorldBuildSameConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myWorld.buildConnection("agent_0", "agent_1");
	myWorld.buildConnection("agent_0", "agent_1");
	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], "agent_1");

	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent1[0], "agent_0");

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( ProposeTwoWayConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myWorld.proposeTwoWayConnection("agent_0", "agent_1");

	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<std::string> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<std::string> validRcvAgent0 = myAgent0->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 1);
	BOOST_CHECK_EQUAL(proposedAgent0[0], "agent_1");
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent0.size(), 1);
	BOOST_CHECK_EQUAL(receivedAgent0[0], "agent_1");
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 0);

	std::vector<std::string> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<std::string> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 1);
	BOOST_CHECK_EQUAL(proposedAgent1[0], "agent_0");
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 1);
	BOOST_CHECK_EQUAL(receivedAgent1[0], "agent_0");
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( WorldBuildTwoWayConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myWorld.buildTwoWayConnection("agent_0", "agent_1");

	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<std::string> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<std::string> validRcvAgent0 = myAgent0->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent0[0], "agent_1");
	BOOST_CHECK_EQUAL(receivedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent0[0], "agent_1");

	std::vector<std::string> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<std::string> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent1[0], "agent_0");
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent1[0], "agent_0");

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( WorldBuildKillHalfOfTwoWayConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myWorld.buildTwoWayConnection("agent_0", "agent_1");
	myWorld.killConnection("agent_0", "agent_1");

	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<std::string> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<std::string> validRcvAgent0 = myAgent0->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 1);
	BOOST_CHECK_EQUAL(validRcvAgent0[0], "agent_1");

	std::vector<std::string> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<std::string> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(), 1);
	BOOST_CHECK_EQUAL(validSendAgent1[0], "agent_0");
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( WorldBuildKillTwoWayConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myWorld.buildTwoWayConnection("agent_0", "agent_1");
	myWorld.killTwoWayConnection("agent_0", "agent_1");

	std::vector<std::string> proposedAgent0 = myAgent0->getProposedConnections();
	std::vector<std::string> validSendAgent0 = myAgent0->getValidSendConnections();
	std::vector<std::string> receivedAgent0 = myAgent0->getReceivedConnections();
	std::vector<std::string> validRcvAgent0 = myAgent0->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent0.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent0.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent0.size(), 0);

	std::vector<std::string> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<std::string> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()




BOOST_AUTO_TEST_SUITE( ConnectionFromOther )

BOOST_AUTO_TEST_CASE( AgentProposeOtherConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myAgent0->proposeConnectionBetween("agent_1", "agent_2");
	std::vector<std::string> proposedAgent1 = myAgent1->getProposedConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 1);
	BOOST_CHECK_EQUAL(proposedAgent1[0], "agent_2");

	std::vector<std::string> receivedAgent2 = myAgent2->getReceivedConnections();
	BOOST_CHECK_EQUAL(receivedAgent2.size(), 1);
	BOOST_CHECK_EQUAL(receivedAgent2[0], "agent_1");

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AgentKillOtherConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	RomanTest* myAgent2 = new RomanTest("agent_2","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myWorld.addAgent(myAgent2);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();
	myAgent2->setRandomPosition();

	myAgent1->proposeConnectionTo("agent_2");
	myAgent2->acceptConnectionFrom("agent_1");
	myAgent0->killConnectionBetween("agent_1", "agent_2");

	std::vector<std::string> proposedAgent1 = myAgent1->getProposedConnections();
	std::vector<std::string> validSendAgent1 = myAgent1->getValidSendConnections();
	std::vector<std::string> receivedAgent1 = myAgent1->getReceivedConnections();
	std::vector<std::string> validRcvAgent1 = myAgent1->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent1.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent1.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent1.size(), 0);

	std::vector<std::string> proposedAgent2 = myAgent2->getProposedConnections();
	std::vector<std::string> validSendAgent2 = myAgent2->getValidSendConnections();
	std::vector<std::string> receivedAgent2 = myAgent2->getReceivedConnections();
	std::vector<std::string> validRcvAgent2 = myAgent2->getValidRcvConnections();
	BOOST_CHECK_EQUAL(proposedAgent2.size(), 0);
	BOOST_CHECK_EQUAL(validSendAgent2.size(), 0);
	BOOST_CHECK_EQUAL(receivedAgent2.size(), 0);
	BOOST_CHECK_EQUAL(validRcvAgent2.size(), 0);

	myWorld.run();
}
BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE( MessagesBtwAgents )

BOOST_AUTO_TEST_CASE( SourceSendMsg ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");
	myAgent0->sendMessageTo("agent_1","hello");

	std::vector<std::tuple<std::string,std::string> > receivedMsgAgent1 = myAgent1->getReceivedMessages();
	BOOST_CHECK_EQUAL(receivedMsgAgent1.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(receivedMsgAgent1[0]), "agent_0");
	BOOST_CHECK_EQUAL(std::get<1>(receivedMsgAgent1[0]), "hello");

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendMsgToProposed ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent0->sendMessageTo("agent_1","hello");

	std::vector<std::tuple<std::string,std::string> > receivedMsgAgent1 = myAgent1->getReceivedMessages();
	BOOST_CHECK_EQUAL(receivedMsgAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendMsgToUnconnected ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->sendMessageTo("agent_1","hello");

	std::vector<std::tuple<std::string,std::string> > receivedMsgAgent1 = myAgent1->getReceivedMessages();
	BOOST_CHECK_EQUAL(receivedMsgAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendMsgAfterConnectionKilled ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");
	myAgent0->killConnectionTo("agent_1");
	myAgent0->sendMessageTo("agent_1","hello");

	std::vector<std::tuple<std::string,std::string> > receivedMsgAgent1 = myAgent1->getReceivedMessages();
	BOOST_CHECK_EQUAL(receivedMsgAgent1.size(), 0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( GoodsSystem )

BOOST_AUTO_TEST_CASE( AddTypeGood ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0,0.0,0.0);

	std::vector<std::tuple<std::string, double, double, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AddTwoTimesTypeGood ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("A",99.0,0.0,0.0);

	std::vector<std::tuple<std::string, double, double, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AddTwoDifferentTypeGood ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("B",50.0,0.0,0.0);

	std::vector<std::tuple<std::string, double, double, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 2);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[1]), "B");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[1]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[1]), 50.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( RemoveTypeGood ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("B",50.0,0.0,0.0);
	myAgent0->removeGoodType("A");

	std::vector<std::tuple<std::string, double, double, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "B");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 50.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( RemoveNoneExistingGood ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->removeGoodType("B");

	std::vector<std::tuple<std::string, double, double, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}


BOOST_AUTO_TEST_CASE( AddGood ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",2.0);

	std::vector<std::tuple<std::string, double, double, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 2.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AddTooMuchGood ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",2.0);
	myAgent0->addGood("A",98.1);

	std::vector<std::tuple<std::string, double, double, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 100.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AddGoodToNotKnownType ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("B",2.0);

	std::vector<std::tuple<std::string, double, double, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( RemoveGood ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",50.0);
	myAgent0->removeGood("A",2.0);

	std::vector<std::tuple<std::string, double, double, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 48.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( RemoveTooMuchGood ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",10.0);
	myAgent0->removeGood("A",20.0);

	std::vector<std::tuple<std::string, double, double, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 0.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( RemoveGoodNotKnown ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",10.0);
	myAgent0->removeGood("B",20.0);

	std::vector<std::tuple<std::string, double, double, double, double> > listGoods = myAgent0->getListGoods();
	BOOST_CHECK_EQUAL(listGoods.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listGoods[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listGoods[0]), 10.0);
	BOOST_CHECK_EQUAL(std::get<2>(listGoods[0]), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( GetGood ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",2.0);

	std::tuple<double, double, double, double> goods = myAgent0->getGood("A");
	BOOST_CHECK_EQUAL(std::get<0>(goods), 2.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( GetGoodUnknwon ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	myWorld.addAgent(myAgent0);
	myAgent0->setRandomPosition();
	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",2.0);

	std::tuple<double, double, double, double> goods = myAgent0->getGood("B");
	BOOST_CHECK_EQUAL(std::get<0>(goods), -1.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods), -1.0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE( SendingGoods )

BOOST_AUTO_TEST_CASE( SendGoodToReceiver ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent1->addGoodType("A",100.0,0.0,0.0);

	myAgent0->sendGoodTo("agent_1","A",15);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 5.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 15.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendUnknownGood ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent1->addGoodType("A",100.0,0.0,0.0);

	myAgent0->sendGoodTo("agent_1","B",15);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendGoodUnknownToReceiver ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent0->addGoodType("B",100.0,0.0,0.0);
	myAgent0->addGood("B",20.0);
	myAgent1->addGoodType("A",100.0,0.0,0.0);

	myAgent0->sendGoodTo("agent_1","B",15);

	std::tuple<double, double, double, double> goodsA0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goodsB0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goodsA0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goodsA0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goodsB0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goodsB0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendGoodWithoutConnection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent1->addGoodType("A",100.0,0.0,0.0);

	myAgent0->sendGoodTo("agent_1","A",15);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendGoodToSender ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGood("A",20.0);

	myAgent1->sendGoodTo("agent_0","A",15);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendSaturatingGood ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",100.0);
	myAgent1->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGood("A",20.0);

	myAgent0->sendGoodTo("agent_1","A",90);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 10.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 100.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( SendGoodNotInStock ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent1->addGoodType("A",100.0,0.0,0.0);

	myAgent0->sendGoodTo("agent_1","A",30);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(goods1), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( TradeGoods )

BOOST_AUTO_TEST_CASE( GetListAllTrades ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);

	myAgent1->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGoodType("currency",100.0,0.0,0.0);

	myAgent0->proposeTradeTo("agent_1","A",10,10);

	std::vector<std::tuple<std::string,std::string,double,double> > listProposedTrades = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > listReceivedTrades = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(listProposedTrades.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listProposedTrades[0]), "agent_1");
	BOOST_CHECK_EQUAL(std::get<1>(listProposedTrades[0]), "A");
	BOOST_CHECK_EQUAL(std::get<2>(listProposedTrades[0]), 10);
	BOOST_CHECK_EQUAL(std::get<3>(listProposedTrades[0]), 10);

	BOOST_CHECK_EQUAL(listReceivedTrades.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listReceivedTrades[0]), "agent_0");
	BOOST_CHECK_EQUAL(std::get<1>(listReceivedTrades[0]), "A");
	BOOST_CHECK_EQUAL(std::get<2>(listReceivedTrades[0]), 10);
	BOOST_CHECK_EQUAL(std::get<3>(listReceivedTrades[0]), 10);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( GetListProposedTradesFromSource ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);

	myAgent1->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGoodType("currency",100.0,0.0,0.0);

	myAgent0->proposeTradeTo("agent_1","A",10,10);

	std::vector<std::tuple<std::string,double,double> > listProposedTrades = myAgent0->getProposedTradesTo("agent_1");
	std::vector<std::tuple<std::string,double,double> > listReceivedTrades = myAgent1->getReceivedTradesFrom("agent_0");

	BOOST_CHECK_EQUAL(listProposedTrades.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listProposedTrades[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listProposedTrades[0]), 10);
	BOOST_CHECK_EQUAL(std::get<2>(listProposedTrades[0]), 10);

	BOOST_CHECK_EQUAL(listReceivedTrades.size(), 1);
	BOOST_CHECK_EQUAL(std::get<0>(listReceivedTrades[0]), "A");
	BOOST_CHECK_EQUAL(std::get<1>(listReceivedTrades[0]), 10);
	BOOST_CHECK_EQUAL(std::get<2>(listReceivedTrades[0]), 10);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( TradeGoods ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent0->addGood("currency",50.0);

	myAgent1->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGoodType("currency",100.0,0.0,0.0);
	myAgent1->addGood("currency",50.0);

	myAgent0->proposeTradeTo("agent_1","A",10,10);
	myAgent1->acceptTradeFrom("agent_0","A",10,10);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");
	std::tuple<double, double, double, double> currency0 = myAgent0->getGood("currency");
	std::tuple<double, double, double, double> currency1 = myAgent1->getGood("currency");
	std::vector<std::tuple<std::string,std::string,double,double> > listProposedTrades = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > listReceivedTrades = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(listProposedTrades.size(), 0);
	BOOST_CHECK_EQUAL(listReceivedTrades.size(), 0);

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 10.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency0), 60.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency0), 100.0);

	BOOST_CHECK_EQUAL(std::get<0>(goods1), 10.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency1), 40.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( RefuseTrade ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent0->addGood("currency",50.0);

	myAgent1->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGoodType("currency",100.0,0.0,0.0);
	myAgent1->addGood("currency",50.0);

	myAgent0->proposeTradeTo("agent_1","A",10,10);
	myAgent1->refuseTradeFrom("agent_0","A",10,10);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");
	std::tuple<double, double, double, double> currency0 = myAgent0->getGood("currency");
	std::tuple<double, double, double, double> currency1 = myAgent1->getGood("currency");
	std::vector<std::tuple<std::string,std::string,double,double> > listProposedTrades = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > listReceivedTrades = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(listProposedTrades.size(), 0);
	BOOST_CHECK_EQUAL(listReceivedTrades.size(), 0);

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency0), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency0), 100.0);

	BOOST_CHECK_EQUAL(std::get<0>(goods1), 00.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency1), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( TradeGoodOfUnknownTypeToSender ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("currency",50.0);

	myAgent1->addGoodType("currency",100.0,0.0,0.0);
	myAgent1->addGood("currency",50.0);

	myAgent1->proposeConnectionTo("agent_0");
	myAgent0->acceptConnectionFrom("agent_1");

	myAgent1->proposeTradeTo("agent_0","A",10,10);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> currency1 = myAgent1->getGood("currency");
	std::tuple<double, double, double, double> currency0 = myAgent0->getGood("currency");
	std::vector<std::tuple<std::string,std::string,double,double> > listProposedTrades = myAgent1->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > listReceivedTrades = myAgent0->getReceivedTrades();

	BOOST_CHECK_EQUAL(listProposedTrades.size(), 0);
	BOOST_CHECK_EQUAL(listReceivedTrades.size(), 0);

	BOOST_CHECK_EQUAL(std::get<0>(currency1), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency1), 100.0);

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 00.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency0), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency0), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( TradeGoodOfUnknownTypeToReceiver ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("currency",50.0);

	myAgent1->addGoodType("currency",100.0,0.0,0.0);
	myAgent1->addGood("currency",50.0);

	myAgent0->proposeTradeTo("agent_1","A",10,10);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> currency0 = myAgent0->getGood("currency");
	std::tuple<double, double, double, double> currency1 = myAgent1->getGood("currency");
	std::vector<std::tuple<std::string,std::string,double,double> > listProposedTrades = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > listReceivedTrades = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(listProposedTrades.size(), 0);
	BOOST_CHECK_EQUAL(listReceivedTrades.size(), 0);

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency0), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency0), 100.0);

	BOOST_CHECK_EQUAL(std::get<0>(currency1), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( TradeGoodsWhenNoNetworkLink ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent0->addGood("currency",50.0);

	myAgent1->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGoodType("currency",100.0,0.0,0.0);
	myAgent1->addGood("currency",50.0);

	myAgent0->proposeTradeTo("agent_1","A",10,10);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");
	std::tuple<double, double, double, double> currency0 = myAgent0->getGood("currency");
	std::tuple<double, double, double, double> currency1 = myAgent1->getGood("currency");
	std::vector<std::tuple<std::string,std::string,double,double> > listProposedTrades = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > listReceivedTrades = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(listProposedTrades.size(), 0);
	BOOST_CHECK_EQUAL(listReceivedTrades.size(), 0);

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency0), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency0), 100.0);

	BOOST_CHECK_EQUAL(std::get<0>(goods1), 00.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency1), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( TradeGoodsWithLinkInWrongDirection ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent1->proposeConnectionTo("agent_0");
	myAgent0->acceptConnectionFrom("agent_1");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent0->addGood("currency",50.0);

	myAgent1->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGoodType("currency",100.0,0.0,0.0);
	myAgent1->addGood("currency",50.0);

	myAgent0->proposeTradeTo("agent_1","A",10,10);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");
	std::tuple<double, double, double, double> currency0 = myAgent0->getGood("currency");
	std::tuple<double, double, double, double> currency1 = myAgent1->getGood("currency");
	std::vector<std::tuple<std::string,std::string,double,double> > listProposedTrades = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > listReceivedTrades = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(listProposedTrades.size(), 0);
	BOOST_CHECK_EQUAL(listReceivedTrades.size(), 0);

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency0), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency0), 100.0);

	BOOST_CHECK_EQUAL(std::get<0>(goods1), 00.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency1), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency1), 100.0);


	myWorld.run();
}

BOOST_AUTO_TEST_CASE( AcceptNonExistingTrade ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent0->addGood("currency",50.0);

	myAgent1->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGoodType("currency",100.0,0.0,0.0);
	myAgent1->addGood("currency",50.0);

	myAgent1->acceptTradeFrom("agent_0","A",10,10);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");
	std::tuple<double, double, double, double> currency0 = myAgent0->getGood("currency");
	std::tuple<double, double, double, double> currency1 = myAgent1->getGood("currency");
	std::vector<std::tuple<std::string,std::string,double,double> > listProposedTrades = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > listReceivedTrades = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(listProposedTrades.size(), 0);
	BOOST_CHECK_EQUAL(listReceivedTrades.size(), 0);

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency0), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency0), 100.0);

	BOOST_CHECK_EQUAL(std::get<0>(goods1), 00.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency1), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( NotEnoughCurrencyInReceiver ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent0->addGood("currency",50.0);

	myAgent1->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGoodType("currency",100.0,0.0,0.0);

	myAgent0->proposeTradeTo("agent_1","A",10,10);
	myAgent1->acceptTradeFrom("agent_0","A",10,10);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");
	std::tuple<double, double, double, double> currency0 = myAgent0->getGood("currency");
	std::tuple<double, double, double, double> currency1 = myAgent1->getGood("currency");
	std::vector<std::tuple<std::string,std::string,double,double> > listProposedTrades = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > listReceivedTrades = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(listProposedTrades.size(), 0);
	BOOST_CHECK_EQUAL(listReceivedTrades.size(), 0);

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 20.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency0), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency0), 100.0);

	BOOST_CHECK_EQUAL(std::get<0>(goods1), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency1), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency1), 100.0);

	myWorld.run();
}


BOOST_AUTO_TEST_CASE( TradeGoodNotInStock ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("currency",50.0);

	myAgent1->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGoodType("currency",100.0,0.0,0.0);
	myAgent1->addGood("currency",50.0);

	myAgent0->proposeTradeTo("agent_1","A",10,10);
	myAgent1->acceptTradeFrom("agent_0","A",10,10);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");
	std::tuple<double, double, double, double> currency0 = myAgent0->getGood("currency");
	std::tuple<double, double, double, double> currency1 = myAgent1->getGood("currency");
	std::vector<std::tuple<std::string,std::string,double,double> > listProposedTrades = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > listReceivedTrades = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(listProposedTrades.size(), 0);
	BOOST_CHECK_EQUAL(listReceivedTrades.size(), 0);

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency0), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency0), 100.0);

	BOOST_CHECK_EQUAL(std::get<0>(goods1), 0.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency1), 50.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( NotEnoughStorageSpaceInReceiver ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent0->addGood("currency",50.0);

	myAgent1->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGoodType("currency",100.0,0.0,0.0);
	myAgent1->addGood("A",95.0);
	myAgent1->addGood("currency",50.0);

	myAgent0->proposeTradeTo("agent_1","A",10,10);
	myAgent1->acceptTradeFrom("agent_0","A",10,10);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");
	std::tuple<double, double, double, double> currency0 = myAgent0->getGood("currency");
	std::tuple<double, double, double, double> currency1 = myAgent1->getGood("currency");
	std::vector<std::tuple<std::string,std::string,double,double> > listProposedTrades = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > listReceivedTrades = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(listProposedTrades.size(), 0);
	BOOST_CHECK_EQUAL(listReceivedTrades.size(), 0);

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 10.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency0), 60.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency0), 100.0);

	BOOST_CHECK_EQUAL(std::get<0>(goods1), 100.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency1), 40.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_CASE( NotEnoughSpaceInSender ) 
{
	ProvinceTest myWorld(new ProvinceConfig(Engine::Size<int>(10,10), 1), ProvinceTest::useSpacePartition(1, false));
	myWorld.initialize(boost::unit_test::framework::master_test_suite().argc, boost::unit_test::framework::master_test_suite().argv);

	RomanTest* myAgent0 = new RomanTest("agent_0","random");
	RomanTest* myAgent1 = new RomanTest("agent_1","random");
	myWorld.addAgent(myAgent0);
	myWorld.addAgent(myAgent1);
	myAgent0->setRandomPosition();
	myAgent1->setRandomPosition();

	myAgent0->proposeConnectionTo("agent_1");
	myAgent1->acceptConnectionFrom("agent_0");

	myAgent0->addGoodType("A",100.0,0.0,0.0);
	myAgent0->addGoodType("currency",100.0,0.0,0.0);
	myAgent0->addGood("A",20.0);
	myAgent0->addGood("currency",95.0);

	myAgent1->addGoodType("A",100.0,0.0,0.0);
	myAgent1->addGoodType("currency",100.0,0.0,0.0);
	myAgent1->addGood("currency",50.0);

	myAgent0->proposeTradeTo("agent_1","A",10,10);
	myAgent1->acceptTradeFrom("agent_0","A",10,10);

	std::tuple<double, double, double, double> goods0 = myAgent0->getGood("A");
	std::tuple<double, double, double, double> goods1 = myAgent1->getGood("A");
	std::tuple<double, double, double, double> currency0 = myAgent0->getGood("currency");
	std::tuple<double, double, double, double> currency1 = myAgent1->getGood("currency");
	std::vector<std::tuple<std::string,std::string,double,double> > listProposedTrades = myAgent0->getProposedTrades();
	std::vector<std::tuple<std::string,std::string,double,double> > listReceivedTrades = myAgent1->getReceivedTrades();

	BOOST_CHECK_EQUAL(listProposedTrades.size(), 0);
	BOOST_CHECK_EQUAL(listReceivedTrades.size(), 0);

	BOOST_CHECK_EQUAL(std::get<0>(goods0), 10.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods0), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency0), 100.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency0), 100.0);

	BOOST_CHECK_EQUAL(std::get<0>(goods1), 10.0);
	BOOST_CHECK_EQUAL(std::get<1>(goods1), 100.0);
	BOOST_CHECK_EQUAL(std::get<0>(currency1), 40.0);
	BOOST_CHECK_EQUAL(std::get<1>(currency1), 100.0);

	myWorld.run();
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test

