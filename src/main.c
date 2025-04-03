#include "fem.h"

void elasticity_solve(const char *meshfile, const char *outfile, double E, double nu, double rho, double g) {
    femGeo *theGeometry = geoMeshRead(meshfile);
    femProblem *theProblem = femElasticityCreate(theGeometry, E, nu, rho, g, PLANAR_STRAIN);

//    femElasticityAddBoundaryCondition(theProblem, "Entity 13 ", DIRICHLET_Y, 0.0);
    femElasticityAddBoundaryCondition(theProblem, "Entity 13 ", DIRICHLET_X, 0.0);
//
//    femElasticityAddBoundaryCondition(theProblem, "Entity 14 ", DIRICHLET_Y, 0.0);
    femElasticityAddBoundaryCondition(theProblem, "Entity 14 ", DIRICHLET_X, 0.0);

    femElasticityAddBoundaryCondition(theProblem, "Entity 17 ", DIRICHLET_Y, 0.0);
    femElasticityAddBoundaryCondition(theProblem, "Entity 18 ", DIRICHLET_Y, 0.0);

    femElasticityAddBoundaryCondition(theProblem, "Entity 21 ", DIRICHLET_Y, 0.0);
    femElasticityAddBoundaryCondition(theProblem, "Entity 22 ", DIRICHLET_Y, 0.0);

//    femElasticityAddBoundaryCondition(theProblem, "Entity 17 ", DIRICHLET_X, 0.0);
//    femElasticityAddBoundaryCondition(theProblem, "Entity 18 ", DIRICHLET_X, 0.0);
//
//    femElasticityAddBoundaryCondition(theProblem, "Entity 21 ", DIRICHLET_X, 0.0);
//    femElasticityAddBoundaryCondition(theProblem, "Entity 22 ", DIRICHLET_X, 0.0);

//    femElasticityAddBoundaryCondition(theProblem, "Entity 0 ", DIRICHLET_Y, 0.0);
//    femElasticityAddBoundaryCondition(theProblem, "Entity 1 ", DIRICHLET_Y, 0.0);
//    femElasticityAddBoundaryCondition(theProblem, "Entity 0 ", DIRICHLET_X, 0.0);
//    femElasticityAddBoundaryCondition(theProblem, "Entity 1 ", DIRICHLET_X, 0.0);

    double *theSoluce = femElasticitySolve(theProblem);

    int nNodes = theGeometry->theNodes->nNodes;
    femSolutionWrite(nNodes, 2, theSoluce, outfile);

    femElasticityFree(theProblem);
    femGeoFree(theGeometry);
}

int main(void) {
    double E = 211.e9;
    double nu = 0.3;
    double rho = 7.85e3;
    double f = -10;

    elasticity_solve("data/mesh.txt", "data/UV.txt", E, nu, rho, f);
}

 
