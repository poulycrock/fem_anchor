#include "gmshc.h"
#include "assert.h"
#include "fem.h"
#include "glfem.h"

double geoSize(double x, double y) {
    return 0.7;
}

double gmshSize(int dim, int tag, double x, double y, double z, double lc, void *data) {
    return geoSize(x, y);
}

int main() {
    int err;
    femGeo *theGeometry = geoGetGeometry();

    gmshInitialize(0, NULL, 1, 0, &err);
    assert(err == 0);

    gmshOpen("anchor.geo", &err);
    assert(err == 0);

    gmshModelMeshSetSizeCallback(gmshSize, NULL, &err);
    assert(err == 0);

    geoSetSizeCallback(geoSize);

    gmshOptionSetNumber("Mesh.SaveAll", 1, &err);
    assert(err == 0);
    gmshModelMeshGenerate(2, &err);
    assert(err == 0);

    geoMeshImport();

    // Downscale the geometry to fit real size
    for (int i = 0; i < theGeometry->theNodes->nNodes; i++) {
        theGeometry->theNodes->X[i] /= 4;
        theGeometry->theNodes->Y[i] /= 4;
    }

    geoMeshWrite("anchor.txt");

    double *meshSizeField = malloc(theGeometry->theNodes->nNodes * sizeof(double));
    femNodes *theNodes = theGeometry->theNodes;
    for (int i = 0; i < theNodes->nNodes; ++i)
        meshSizeField[i] = geoSize(theNodes->X[i], theNodes->Y[i]);
    double hMin = femMin(meshSizeField, theNodes->nNodes);
    double hMax = femMax(meshSizeField, theNodes->nNodes);
    printf(" ==== Global requested h : %14.7e \n", theGeometry->h);
    printf(" ==== Minimum h          : %14.7e \n", hMin);
    printf(" ==== Maximum h          : %14.7e \n", hMax);

    assert(err == 0);
    int mode = 1; // Change mode by pressing "j", "k", "l"
    int domain = 0;
    int freezingButton = FALSE;
    double t, told = 0;
    char theMessage[256];
    double pos[2] = {20, 460};


    GLFWwindow *window = glfemInit("EPL1110 : Mesh generation ");
    glfwMakeContextCurrent(window);

    do {
        int w, h;


        glfwGetFramebufferSize(window, &w, &h);
        glfemReshapeWindows(theGeometry->theNodes, w, h);

        t = glfwGetTime();
        //    glfemChangeState(&mode, theMeshes->nMesh);
        if (glfwGetKey(window, 'D') == GLFW_PRESS) { mode = 0; }
        if (glfwGetKey(window, 'V') == GLFW_PRESS) { mode = 1; }
        if (glfwGetKey(window, 'N') == GLFW_PRESS && freezingButton == FALSE) {
            domain++;
            freezingButton = TRUE;
            told = t;
        }


        if (t - told > 0.5) { freezingButton = FALSE; }


        if (mode == 1) {
            glfemPlotField(theGeometry->theElements, meshSizeField);
            glfemPlotMesh(theGeometry->theElements);
            sprintf(theMessage, "Number of elements : %d ", theGeometry->theElements->nElem);


            glColor3f(1.0, 0.0, 0.0);
            glfemMessage(theMessage);


        }
        if (mode == 0) {
            domain = domain % theGeometry->nDomains;
            glfemPlotDomain(theGeometry->theDomains[domain]);


            sprintf(theMessage, "%s : %d ", theGeometry->theDomains[domain]->name, domain);


            glColor3f(1.0, 0.0, 0.0);
            glfemMessage(theMessage);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) != 1);


    free(meshSizeField);
    geoFinalize();
}