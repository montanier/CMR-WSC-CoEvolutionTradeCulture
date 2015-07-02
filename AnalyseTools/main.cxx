/*
 * =====================================================================================
 *
 *       Filename:  main.cxx
 *
 *    Description:  on anlasyse
 *
 *        Version:  1.0
 *        Created:  27/02/15 13:27:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */



#include <analysis/GlobalAgentStats.hxx>
#include <analysis/GlobalRasterStats.hxx>
#include <analysis/AgentMean.hxx>
#include <analysis/AgentStdDev.hxx>
#include <analysis/AgentSum.hxx>
#include <analysis/RasterMean.hxx>
#include <analysis/RasterSum.hxx>
#include <analysis/AgentNum.hxx>
#include "NoState.hxx"
#include <iostream>

                                                                                                                                                                                                     
#include <SimulationRecord.hxx>

int main(int argc, char *argv[])
{       
	PostProcess::NoState agentResults;

	Engine::SimulationRecord simRecord( 1, false);
	simRecord.loadHDF5("data/province.h5", false, true);
	agentResults.apply(simRecord, "agents.csv", "Roman");



}


