#include "fem.h"

void elasticity_solve(const char *meshfile, const char *outfile, double E, double nu, double rho, double f) {
    femGeo *theGeometry = geoMeshRead(meshfile);
    femProblem *theProblem = femElasticityCreate(theGeometry, E, nu, rho, f, PLANAR_STRAIN);

    // Fix the body of the anchor on the x axis
    femElasticityAddBoundaryCondition(theProblem, "Entity 13 ", DIRICHLET_X, 0.0);
    femElasticityAddBoundaryCondition(theProblem, "Entity 14 ", DIRICHLET_X, 0.0);

    // Fix the arms of the anchor on the y axis
    femElasticityAddBoundaryCondition(theProblem, "Entity 17 ", DIRICHLET_Y, 0.0);
    femElasticityAddBoundaryCondition(theProblem, "Entity 18 ", DIRICHLET_Y, 0.0);
    femElasticityAddBoundaryCondition(theProblem, "Entity 21 ", DIRICHLET_Y, 0.0);
    femElasticityAddBoundaryCondition(theProblem, "Entity 22 ", DIRICHLET_Y, 0.0);

    double *solution = femElasticitySolve(theProblem);

    int n_nodes = theGeometry->theNodes->nNodes;
    femSolutionWrite(n_nodes, 2, solution, outfile);

    femElasticityFree(theProblem);
    femGeoFree(theGeometry);
}

int main(void) {
    double E = 211.e9;
    double nu = 0.3;
    double rho = 7.85e3;
    double f = -1000;

    elasticity_solve("data/mesh.txt", "data/UV.txt", E, nu, rho, f);
}

 
