#include "mesh/load_terrain.h"

#include <cassert>

#include "fileio/img_png.h"

namespace cs559 {
   namespace mesh {
     
     //
     // Vertex Utility
     //
     struct VertexUt {
       float* vtx;
       int width, height;
       VertexUt(float* _vtx, int _width, int _height) : vtx(_vtx), width(_width), height(_height) {  
         assert(nullptr != vtx);
         stride = width*(3+3+2);
       }
       // postion
       void SetPX(int x, int y, float val) { assert(y < height && x < width);
         vtx[y*(stride) + x*(3+3+2) + 0] = val;
       }
       void SetPY(int x, int y, float val) {
         vtx[y*(stride) + x*(3+3+2) + 1] = val;
       }
       void SetPZ(int x, int y, float val) {
         vtx[y*(stride) + x*(3+3+2) + 2] = val;
       }
       // normal
       void SetNX(int x, int y, float val) { assert(y < height && x < width);
         vtx[y*(stride) + x*(3+3+2) + 3] = val;
       }
       void SetNY(int x, int y, float val) {
         vtx[y*(stride) + x*(3+3+2) + 4] = val;
       }
       void SetNZ(int x, int y, float val) {
         vtx[y*(stride) + x*(3+3+2) + 5] = val;
       }
       // texture
       void SetS(int x, int y, float val) { assert(y < height && x < width);
         vtx[y*(stride) + x*(3+3+2) + 6] = val;
       }
       void SetT(int x, int y, float val) {
         vtx[y*(stride) + x*(3+3+2) + 7] = val;
       }

     private:
       int stride;
       // declare not defined, i.e. don't use it.
       VertexUt(VertexUt const& rhs);
       VertexUt& operator = (VertexUt const& rhs);
     };// struct MeshUt {}


     //
     // Index Utility
     //
     struct IndexUt {
       uint* idx;
       int width, height;
       IndexUt(uint* _idx, int _width, int _height) : idx(_idx), width(_width), height(_height) {  
         assert(nullptr != idx);
         stride = (width-1)*2*3;
       }

       // set triangle index
       void SetTri(int x, int y) {
         assert(x < width-1 && y < height-1);
         int curr = (y+0)*(width) + x;
         int rght = (y+0)*(width) + x + 1; 
         int next = (y+1)*(width) + x;
         int rgnx = (y+1)*(width) + x + 1;

         idx[y*stride + x*6 + 0] = curr;
         idx[y*stride + x*6 + 1] = next;
         idx[y*stride + x*6 + 2] = rght;

         idx[y*stride + x*6 + 3] = next;
         idx[y*stride + x*6 + 4] = rgnx;
         idx[y*stride + x*6 + 5] = rght;
       }

     private:
       int stride;
       // declare not defined, i.e. don't use it.
       IndexUt(IndexUt const& rhs);
       IndexUt& operator = (IndexUt const& rhs);
     }; //struct IndexUt {}


     //
     // RGB color:
     //  R: abs(Height), sign in Blue color
     //  G: unused
     //  B: 255:+, 0:-
     //
     Mesh* LoadTerrainImg332(char const* filename, float r /*xz-scale*/, float vscale) {       
       ImgPng img(filename, 3 /*rgb*/);
       if (nullptr == img.data) {
         return nullptr;
       }
       int width = img.width, height = img.height;

       // init
       int num_vtx = (width)*(height)*(3+3+2);
       int num_idx = (width-1)*(height-1)*2*3 /*each square contains two triangle*/ /*each triangle contains three vertex*/;
       float* vtx_data = new float[num_vtx];
       uint* idx_data = new uint[num_idx];
       
       VertexUt vtx(vtx_data, width, height);
       IndexUt idx(idx_data, width, height);

       // setup vertex data
       for (int y = 0; y < height; ++y) {
         for (int x = 0; x < width; ++x) {           
           float fx = float(x)/float(width-1);
           float fy = float(y)/float(height-1);
           // position           
           float tall = (float)(img.B(x,y) > 128 ? img.R(x,y) : -img.R(x,y));           
           vtx.SetPY(x, y, vscale*tall/255.f);
           vtx.SetPX(x, y, (2.f*fx - 1.f)*r);
           vtx.SetPZ(x, y, (2.f*fy - 1.f)*r);
           // normal
           vtx.SetNY(x, y, 1.f);
           vtx.SetNX(x, y, 0.f);
           vtx.SetNZ(x, y, 0.f);
           // texture s t
           vtx.SetS(x, y, fx);
           vtx.SetT(x, y, fy);
         }
       }

       // setup index data
       for (int y = 0; y < height-1; ++y) {
         for (int x = 0; x < width-1; ++x) {
           idx.SetTri(x,y);
         }
       }

       // store and return
       Mesh* mesh = new Mesh();
       mesh->idx_data = idx_data;
       mesh->idx_size = num_idx*sizeof(uint);
       mesh->vtx_data = vtx_data;
       mesh->vtx_size = num_vtx*sizeof(float);
       mesh->num_indx = num_idx;
       return mesh;
     }// LoadTerrainImg332(char const* filename) {

   }// namespace mesh
}// namespace cs559