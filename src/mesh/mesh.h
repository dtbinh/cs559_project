#ifndef MESH_H
#define MESH_H

#include "cs559core.h"
#include <cstring>

namespace cs559 {
  //
  // basic structure to store mesh data
  //
  struct Mesh {
    float* vtx_data;
    int    vtx_size;
    uint*  idx_data;
    int    idx_size;
    int    num_indx;

    Mesh() :
      vtx_data(nullptr), vtx_size(0), idx_data(nullptr), idx_size(0), num_indx(0){
    }
    
    Mesh(Mesh const& rhs);
    Mesh& operator = (Mesh const& rhs);

    bool RemoveGEQ_Y (float thld);
    bool RemoveLEQ_Y (float thld);
    void ScaleY(float s);

    ~Mesh() {
      SafeDeleteArray(vtx_data);
      vtx_size = 0;
      SafeDeleteArray(idx_data);
      idx_size = 0;
    }  
  };// class Mesh

}//namespace cs559 {
#endif