//
//  gen_model.cpp
//  cs559fltk
//
//  Created by Leon Chu on 11/14/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#include "mesh/gen_primitive.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include <cassert>

#include "misc/util.h"

namespace cs559 {

//
// generate position, normal and texture: 3 + 3 + 2
//
int GenSphere(float r, int nx, int ny, float*& vtx_data, int& vtx_size, uint*& idx_data, int& idx_size) {
  int const vtx_unit = 3 + 3 + 2;
  int const idx_unit = 3;
  int const vtx_stride = vtx_unit * nx;
  int const idx_stride = idx_unit * nx;
  int num_v = vtx_stride * ny;
  vtx_size = num_v*sizeof(float);
  vtx_data = new float[num_v];
  int num_i = 2 * idx_stride * ny;
  idx_size = num_i*sizeof(uint);
  idx_data = new uint[num_i]; //two triangle form a rect face
  
  for (int i = 0; i < ny; ++i) {
    for (int j = 0; j < nx; ++j) {
      float* p = vtx_data + vtx_stride * i + vtx_unit * j;
      float* n = p + 3;
      float* t = n + 3;
      unsigned int* idx1 = idx_data + idx_stride*i + idx_unit*j;
      unsigned int* idx2 = idx1     + idx_stride*ny;
      
      // sphere vertex
      int   const ny_1  = ny - 1; // to cover the pole
      int   const nx_1  = nx - 1; // to cover the pole
      t[1] = (float)i/(float)ny_1;
      t[0] = (float)j/(float)nx_1;
      float PI = (float)M_PI;
      float const theta = (2.f*PI)*t[0];
      float const phi   =     (PI)*t[1];
      p[0] = r*sin(phi)*cos(theta);
      p[1] = r*cos(phi);
      p[2] = r*sin(phi)*sin(theta);
      n[0] = p[0];
      n[1] = p[1];
      n[2] = p[2];
      
      // sphere index
      if (i == ny_1) {
        idx1[0] = idx1[1] = idx1[2] = 0; // neglect last row
      } else {
        int head = i*nx;
        int curr = head + j;
        int next = curr + nx;
        int rght = curr + 1;
        if (rght == next) rght = head;
        idx1[0] = curr;
        idx1[1] = rght;
        idx1[2] = next;
        
        int next_rght = rght + nx;
        idx2[0] = next;
        idx2[1] = rght;
        idx2[2] = next_rght;
      }
    }
  }
  
  return num_i;
}//end GenSphere()

  
  int GenCube33(float r, float*& vtx_data, int& vtx_size, unsigned int*& idx_data, int& idx_size) {
    
    float vtxarray[] = {
      // face +y
      r, r, r, 0, 1, 0, // 0
      r, r,-r, 0, 1, 0, // 1
     -r, r,-r, 0, 1, 0, // 2
     -r, r, r, 0, 1, 0, // 3
 
      // face -y
      r,-r, r, 0,-1, 0, // 4
      r,-r,-r, 0,-1, 0, // 5
     -r,-r,-r, 0,-1, 0, // 6
     -r,-r, r, 0,-1, 0, // 7
      
      // face +x
      r, r, r, 1, 0, 0, // 8
      r,-r, r, 1, 0, 0, // 9
      r,-r,-r, 1, 0, 0, //10
      r, r,-r, 1, 0, 0, //11

      // face -x
     -r, r, r,-1, 0, 0, //12
     -r,-r, r,-1, 0, 0, //13
     -r,-r,-r,-1, 0, 0, //14
     -r, r,-r,-1, 0, 0, //15
      
      // face +z
      r, r, r, 0, 0, 1, //16
     -r, r, r, 0, 0, 1, //17
     -r,-r, r, 0, 0, 1, //18
      r,-r, r, 0, 0, 1, //19
      
      // face -z
      r, r,-r, 0, 0,-1, //20
     -r, r,-r, 0, 0,-1, //21
     -r,-r,-r, 0, 0,-1, //22
      r,-r,-r, 0, 0,-1, //24
    };
    
    unsigned int idxarray[] = {
      // face +y
      0, 1, 2,
      2, 3, 0,
      
      // face -y
      4, 7, 6,
      6, 5, 4,
      
      // face +x
      8, 9, 10,
      10, 11, 8,
      
      // face -x
      12, 15, 14,
      14, 13, 12,
      
      // face +z
      16, 17, 18,
      18, 19, 16,
      
      // face -z
      20, 23, 22,
      22, 21, 20
    };
    
    //
    int num_v = 144;
    vtx_data = new float[num_v];
    vtx_size = sizeof(float)*num_v;
    memcpy(vtx_data, vtxarray, sizeof(vtxarray));

    int num_i = 36;
    idx_data = new uint[num_i];
    idx_size = sizeof(uint)*num_i;
    memcpy(idx_data, idxarray, sizeof(idxarray));
    
    return num_i;
  }// GenCube33()
  


  namespace mesh {

    Mesh* PlaneXZ332(float r) {
      float vtex[] = {
        //position,  normal,   texture
         r, 0,  r,  0, 1, 0,  0, 0,
         r, 0, -r,  0, 1, 0,  0, 1,
        -r, 0, -r,  0, 1, 0,  1, 1,
        -r, 0,  r,  0, 1, 0,  1, 0
      };

      uint idxa[] = {
        0, 1, 2,
        2, 3, 0
      };

      Mesh* mesh = new Mesh;
      int num_v = 32;
      int num_i = 6;
      mesh->vtx_data = new float[num_v];
      mesh->vtx_size = num_v * sizeof(float);
      mesh->idx_data = new uint[num_i];
      mesh->idx_size = num_i * sizeof(uint);
      mesh->num_indx = num_i;
      memcpy(mesh->vtx_data, vtex, mesh->vtx_size);
      memcpy(mesh->idx_data, idxa, mesh->idx_size);

      return mesh;
    }//GenPlaneXZ

    //
    // the texture coordinate is corresponding to skybox
    //
    Mesh*  GenCube332(float r) {
      float vtex[] = {
        // face +y
        r, r, r, 0.f, 1.f, 0.f,    0.50f, 0.33f,// 0
        r, r,-r, 0.f, 1.f, 0.f,    0.50f, 0.00f,// 1
        -r, r,-r, 0.f, 1.f, 0.f,   0.25f, 0.00f,// 2
        -r, r, r, 0.f, 1.f, 0.f,   0.25f, 0.33f,// 3

        // face -y
        r,-r, r, 0,-1, 0,    0.50f, 0.67f,// 4
        r,-r,-r, 0,-1, 0,    0.50f, 1.00f,// 5
        -r,-r,-r, 0,-1, 0,   0.25f, 1.00f,// 6
        -r,-r, r, 0,-1, 0,   0.25f, 0.67f,// 7

        // face +x
        r, r, r, 1, 0, 0,    0.50f, 0.33f,// 8
        r,-r, r, 1, 0, 0,    0.50f, 0.67f,// 9
        r,-r,-r, 1, 0, 0,    0.75f, 0.67f,//10
        r, r,-r, 1, 0, 0,    0.75f, 0.33f,//11

        // face -x
        -r, r, r,-1, 0, 0,   0.25f, 0.33f,//12
        -r,-r, r,-1, 0, 0,   0.25f, 0.67f,//13
        -r,-r,-r,-1, 0, 0,   0.00f, 0.67f,//14
        -r, r,-r,-1, 0, 0,   0.00f, 0.33f,//15

        // face +z
        r, r, r, 0, 0, 1,    0.50f, 0.33f,//16
        -r, r, r, 0, 0, 1,   0.25f, 0.33f,//17
        -r,-r, r, 0, 0, 1,   0.25f, 0.67f,//18
        r,-r, r, 0, 0, 1,    0.50f, 0.67f,//19

        // face -z
        r, r,-r, 0, 0,-1,    0.75f, 0.33f,//20
        -r, r,-r, 0, 0,-1,   1.00f, 0.33f,//21
        -r,-r,-r, 0, 0,-1,   1.00f, 0.67f,//22
        r,-r,-r, 0, 0,-1,    0.75f, 0.67f//24
      };

      unsigned int idxa[] = {
        // face +y
        0, 1, 2,
        2, 3, 0,

        // face -y
        4, 7, 6,
        6, 5, 4,

        // face +x
        8, 9, 10,
        10, 11, 8,

        // face -x
        12, 15, 14,
        14, 13, 12,

        // face +z
        16, 17, 18,
        18, 19, 16,

        // face -z
        20, 23, 22,
        22, 21, 20
      };

      Mesh* mesh = new Mesh;
      int num_v = 144+48;
      int num_i = 36;
      mesh->vtx_data = new float[num_v];
      mesh->vtx_size = num_v * sizeof(float);
      mesh->idx_data = new uint[num_i];
      mesh->idx_size = num_i * sizeof(uint);
      mesh->num_indx = num_i;
      memcpy(mesh->vtx_data, vtex, mesh->vtx_size);
      memcpy(mesh->idx_data, idxa, mesh->idx_size);

      return mesh;
    }// GenCube33()

    Mesh* GenSphere(float r, int nx, int ny) {            
      int const vtx_unit = 3 + 3 + 2;
      int const idx_unit = 3;
      int const vtx_stride = vtx_unit * nx;
      int const idx_stride = idx_unit * nx;
      int num_v = vtx_stride * ny;

      Mesh* mesh = new Mesh;
      int vtx_size = mesh->vtx_size = num_v*sizeof(float);
      float* vtx_data = mesh->vtx_data = new float[num_v];
      int num_i = mesh->num_indx = 2 * idx_stride * ny;
      int idx_size = mesh->idx_size = num_i*sizeof(uint);
      uint* idx_data = mesh->idx_data = new uint[num_i]; //two triangle form a rect face

      for (int i = 0; i < ny; ++i) {
        for (int j = 0; j < nx; ++j) {
          float* p = vtx_data + vtx_stride * i + vtx_unit * j;
          float* n = p + 3;
          float* t = n + 3;
          unsigned int* idx1 = idx_data + idx_stride*i + idx_unit*j;
          unsigned int* idx2 = idx1     + idx_stride*ny;

          // sphere vertex
          int   const ny_1  = ny - 1; // to cover the pole
          int   const nx_1  = nx - 1; // to cover the pole
          t[1] = (float)i/(float)ny_1;
          t[0] = (float)j/(float)nx_1;
          float PI = (float)M_PI;
          float const theta = (2.f*PI)*t[0];
          float const phi   =     (PI)*t[1];
          p[0] = r*sin(phi)*cos(theta);
          p[1] = r*cos(phi);
          p[2] = r*sin(phi)*sin(theta);
          n[0] = p[0];
          n[1] = p[1];
          n[2] = p[2];

          // sphere index
          if (i == ny_1) {
            idx1[0] = idx1[1] = idx1[2] = 0; // neglect last row
          } else {
            int head = i*nx;
            int curr = head + j;
            int next = curr + nx;
            int rght = curr + 1;
            if (rght == next) rght = head;
            idx1[0] = curr;
            idx1[1] = rght;
            idx1[2] = next;

            int next_rght = rght + nx;
            idx2[0] = next;
            idx2[1] = rght;
            idx2[2] = next_rght;
          }
        }
      }
      return mesh;
    }//end GenSphere()


    /**
     */
    Mesh* Quad332(
      float x1, float y1, float z1,
      float x2, float y2, float z2,
      float x3, float y3, float z3,
      float x4, float y4, float z4 ) {

        //TODO
        assert(0);
        Mesh* mesh = new Mesh;
        return mesh;
    }

    /** BillBoards
     */
    Mesh* BillBoards332(int num, float width, float height) {
      float data[] = {
        -0.5f*width, -0.5f*height, 0,  0,0,1, 0,1,
         0.5f*width, -0.5f*height, 0,  0,0,1, 1,1,
         0.5f*width,  0.5f*height, 0,  0,0,1, 1,0,
        -0.5f*width,  0.5f*height, 0,  0,0,1, 0,0,        
      };

      uint index[] = {
        0, 1, 2,
        2, 3, 0
      };

      Mesh* mesh = new Mesh;
      int tree_data_stride = sizeof(data);
      int tree_indx_stride = sizeof(index);
      mesh->vtx_size = num*sizeof(data);
      mesh->vtx_data = new float[num*4*8];
      mesh->num_indx = num*(3 + 3);
      mesh->idx_size = mesh->num_indx*sizeof(uint);
      mesh->idx_data = new uint[mesh->num_indx]; //two triangle form a rect face

      for (int i = 0; i < num; ++i) {
        memcpy(mesh->vtx_data + i*32, data, tree_data_stride);
        memcpy(mesh->idx_data + i*6, index, tree_indx_stride);
        for (int j = 0; j < 6; ++j) {
          index[j] += 4;
        }
      }
      return mesh;
    }

    /** BillBoard, cener is (0,0,0), left-bottom:(-0.5*width, -0.5*height, 0)
     */
    Mesh* BillBoards332(float width, float height) {
      
      int idx_stride = 4;

      float data[] = {
        -0.5f*width, -0.5f*height, 0,  0,0,1, 0,1,
         0.5f*width, -0.5f*height, 0,  0,0,1, 1,1,
         0.5f*width,  0.5f*height, 0,  0,0,1, 1,0,
        -0.5f*width,  0.5f*height, 0,  0,0,1, 0,0,        
      };

      uint index[] = {
        0, 1, 2,
        2, 3, 0
      };
      
      Mesh* mesh = new Mesh;
      mesh->vtx_size = sizeof(data);
      mesh->vtx_data = new float[4*8];
      mesh->num_indx = 3 + 3;
      mesh->idx_size = sizeof(index);
      mesh->idx_data = new uint[mesh->num_indx]; //two triangle form a rect face

      memcpy(mesh->vtx_data, data, mesh->vtx_size);
      memcpy(mesh->idx_data, index, mesh->idx_size);

      return mesh;
    }
  
  
  /** BillBoards
  */
  Mesh* BillBoards3321(int num, float width, float height) {
    float data[] = {
      -0.5f*width, -0.5f*height, 0,     0,0,0,    0,1,    0,
       0.5f*width, -0.5f*height, 0,     0,0,0,    1,1,    0,
       0.5f*width,  0.5f*height, 0,     0,0,0,    1,0,    0,
      -0.5f*width,  0.5f*height, 0,     0,0,0,    0,0,    0    
    };
    int const num_data = 36;

    uint index[] = {
      0, 1, 2,
      2, 3, 0
    };
    int const num_indx = 6;

    Mesh* mesh = new Mesh;
    int tree_data_stride = sizeof(data);
    int tree_indx_stride = sizeof(index);
    mesh->vtx_size = num*sizeof(data);
    mesh->vtx_data = new float[num*num_data];
    mesh->num_indx = num*(num_indx);
    mesh->idx_size = mesh->num_indx*sizeof(uint);
    mesh->idx_data = new uint[mesh->num_indx]; //two triangle form a rect face

    for (int i = 0; i < num; ++i) {
      memcpy(mesh->vtx_data + i*num_data, data,  tree_data_stride);
      memcpy(mesh->idx_data + i*num_indx, index, tree_indx_stride);
      for (int j = 0; j < num_indx; ++j) {
        index[j] += 4;
      }
    }
    return mesh;
  }

  
  /** BillBoards
  */
  Mesh* Bird3321(int num, float width, float height) {
    float s = width;
    float data[] = {
       s, 1.5f*s,  0.2f*s,   0,0,0,  0,0,  0,
       s, 1.5f*s, -0.8f*s,   0,0,0,  0,1,  0,
       0,   0, -0.5f*s,   0,0,0,  1,1,  0,      
       0,   0,  0.5f*s,   0,0,0,  1,0,  0,

      -s, 1.5f*s, -0.8f*s,   0,0,0,  0,1,  0,
      -s, 1.5f*s,  0.2f*s,   0,0,0,  0,0,  0
    };
    
    int const num_data = 54;

    uint index[] = {
      0, 1, 2,
      2, 3, 0,
      3, 2, 4,
      4, 5, 3
    };

    int const num_indx = 12;

    Mesh* mesh = new Mesh;
    int tree_data_stride = sizeof(data);
    int tree_indx_stride = sizeof(index);
    mesh->vtx_size = num*sizeof(data);
    mesh->vtx_data = new float[num*num_data];
    mesh->num_indx = num*(num_indx);
    mesh->idx_size = mesh->num_indx*sizeof(uint);
    mesh->idx_data = new uint[mesh->num_indx]; //two triangle form a rect face

    for (int i = 0; i < num; ++i) {
      memcpy(mesh->vtx_data + i*num_data, data,  tree_data_stride);
      memcpy(mesh->idx_data + i*num_indx, index, tree_indx_stride);
      for (int j = 0; j < num_indx; ++j) {
        index[j] += 4;
      }
    }
    return mesh;
  }

  }// namespace mesh
}// namespace cs559
