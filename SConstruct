############# TEMPLATE FOR MODELS  ###################
######################################################
######################################################
################ CUSTOM INFORMATION  #################
##### please fill with information about your model ##
######################################################

nameProgram = 'province'

agents = ['Roman']
world = 'Province'
namespaceAgents = ['Epnet']

srcFiles = Split('main.cxx Roman.cxx Province.cxx ProvinceConfig.cxx HarvestAction.cxx ProposeConnectionAction.cxx SendGoodsAction.cxx FunAction.cxx ProposeTradeAction.cxx RandomController.cxx BasicController.cxx ControllerFactory.cxx MacmillanController.cxx ProductionAction.cxx TradeAction.cxx ConsumptionAction.cxx CulturalAction.cxx')

###################################################
########## END OF CUSTOM INFORMATION  #############
##### don't modify anything below these lines #####
###################################################

import os, sys
from subprocess import call

pandoraPath = os.getenv('PANDORAPATH', '/usr/local/pandora')
sys.path.append(pandoraPath+'/bin')

import generateMpi 

vars = Variables('custom.py')
vars.Add(BoolVariable('debug', 'compile with debug flags', 'no'))
vars.Add(BoolVariable('edebug', 'compile with extreme debug logs', 'no'))

env = Environment(variables=vars, ENV=os.environ, CXX='mpicxx')
Help(vars.GenerateHelpText(env))

generateMPICodeBuilder = Builder(action=generateMpi.execute)
env.Append( BUILDERS = {'GenerateMPICode' : generateMPICodeBuilder})

env.Append(LINKFLAGS = '-fopenmp')
env.Append(CCFLAGS = '-std=c++0x -DTIXML_USE_STL'.split())
if env['debug'] == True:
    env.Append(CCFLAGS = '-g -O0 -Wall -DPANDORADEBUG'.split())
    if env['edebug']==True:
        env.Append(CCFLAGS = '-DPANDORAEDEBUG')
    #env.Append(LIBS = 'pandorad')
    env.Append(LIBS = 'pandora')
else:
    env.Append(CCFLAGS = '-Ofast'.split())
    env.Append(LIBS = 'pandora')

env.Append(CPPPATH = ['.', pandoraPath+'/include'])
env.Append(LIBPATH = [pandoraPath+'/lib'])

# add the list of mpi code that must be generated & compiled
mpiAgentsSrc = ['mpiCode/FactoryCode.cxx']
agentsSrc = ['main.cxx']
for agent in agents:    
    if agent != '':
        agentsSrc.append(agent+".cxx")
        mpiAgentsSrc.append("mpiCode/"+agent+"_mpi.cxx")

env['namespaces'] = namespaceAgents
env.GenerateMPICode( target=mpiAgentsSrc, source=agentsSrc)
env.Depends(world+'.hxx',mpiAgentsSrc)
env.Program(nameProgram, srcFiles+mpiAgentsSrc)

