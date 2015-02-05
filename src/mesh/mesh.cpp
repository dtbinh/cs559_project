#include "mesh/mesh.h"

namespace cs559 {

  /**
   *
   */
  void Mesh::ScaleY(float s) {
    for (int i = 0; i < num_indx; ++i) {
      vtx_data[idx_data[i]*8 + 1]*=s;        
    }
  } // ScaleY()

  /**
   *
   */
  bool Mesh::RemoveGEQ_Y (float thld) {
    uint* buf = new uint[num_indx]; //index buffer
    if (nullptr == buf) {
      return false;
    }
    int n = 0;
    int num_tri = num_indx / 3;
    for (int i = 0; i < num_tri; ++i) {
      float y1 = vtx_data[idx_data[3*i+0]*8+1];
      float y2 = vtx_data[idx_data[3*i+1]*8+1];
      float y3 = vtx_data[idx_data[3*i+2]*8+1];
      if (y1 < thld && y2 < thld && y3 < thld){
        buf[n++] = idx_data[3*i+0];
        buf[n++] = idx_data[3*i+1];
        buf[n++] = idx_data[3*i+2];
      }
    }
    // reset
    num_indx = n;
    memcpy(idx_data, buf, n*sizeof(uint));     
    delete [] buf;
    return true;
  } // RemoveGEQ_Y (float thld)

  /**
   *
   */
  bool Mesh::RemoveLEQ_Y (float thld) {
    uint* buf = new uint[num_indx]; //index buffer
    if (nullptr == buf) {
      return false;
    }
    int n = 0;
    int num_tri = num_indx / 3;
    for (int i = 0; i < num_tri; ++i) {
      float y1 = vtx_data[idx_data[3*i+0]*8+1];
      float y2 = vtx_data[idx_data[3*i+1]*8+1];
      float y3 = vtx_data[idx_data[3*i+2]*8+1];
      if (y1 > thld && y2 > thld && y3 > thld){
        buf[n++] = idx_data[3*i+0];
        buf[n++] = idx_data[3*i+1];
        buf[n++] = idx_data[3*i+2];
      }
    }
    // reset
    num_indx = n;
    memcpy(idx_data, buf, n*sizeof(uint));     
    delete [] buf;
    return true;
  } // RemoveGEQ_Y (float thld)
} // namespace cs559