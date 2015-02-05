#ifndef LOAD_TERRAIN_H
#define LOAD_TERRAIN_H

#include "mesh/mesh.h"

namespace cs559 {
   namespace mesh {

     //
     // RGB color:
     //  R: abs(Height), sign in Blue color
     //  G: unused
     //  B: 255:+, 0:-
     //
     Mesh* LoadTerrainImg332(char const* filename, float r /*scale*/, float vscale /*vertical scale*/);


   }// namespace mesh
}// namespace cs559

#endif