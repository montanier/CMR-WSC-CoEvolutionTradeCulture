
#include "Roman_mpi.hxx"
#include <Roman.hxx>
#include <cstring>
#include <mpi.h>
#include <typedefs.hxx>

namespace Epnet
{

void * Roman::fillPackage()
{
	RomanPackage * package = new RomanPackage;
	memcpy(&package->_idMpi, _id.c_str(), std::min((unsigned int)32,(unsigned int)(sizeof(char)*_id.size())));
	package->_idMpi[std::min((unsigned int)32,(unsigned int)_id.size())] = '\0';
	package->_existsMpi = _exists;
	package->_positionMpi = _position;

	package->_resourcesMpi = _resources;
	return package;
}

Roman::Roman( void * package ) : Engine::Agent(((RomanPackage*)package)->_idMpi)
{
	RomanPackage * particularPackage = (RomanPackage*)package;
	_exists = particularPackage->_existsMpi;
	_position = particularPackage->_positionMpi;

	_resources = particularPackage->_resourcesMpi;
}


void Roman::sendVectorAttributes( int target )
{
}

void Roman::receiveVectorAttributes( int origin )
{
}

} // namespace Epnet

