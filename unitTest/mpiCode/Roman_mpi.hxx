
#ifndef __Roman_mpi_hxx
#define __Roman_mpi_hxx

#include <Point2D.hxx>

typedef struct
{
	char _idMpi[32];
	bool _existsMpi;
	Engine::Point2D<int> _positionMpi;

	int _resourcesMpi;
} RomanPackage;

#endif // __Roman_mpi_hxx

