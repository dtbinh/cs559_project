//
//  gen_model.h
//  cs559fltk
//
//  Created by Leon Chu on 11/14/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#ifndef __cs559fltk__gen_model__
#define __cs559fltk__gen_model__

#include <stdio.h>
#include "cs559core.h"
#include "mesh/mesh.h"

namespace cs559 {
  
  namespace mesh {
    Mesh* PlaneXZ332(float r);
    Mesh* GenCube332(float r);
    Mesh* GenSphere(float r, int nx, int ny);

    /** first point is bottm left of the texture coordinate, then CCW.
     */
    Mesh* Quad332(
      float x1, float y1, float z1,
      float x2, float y2, float z2,
      float x3, float y3, float z3,
      float x4, float y4, float z4 );

    /** BillBoard, cener is (0,0,0), left-bottom:(-0.5*width, -0.5*height, 0)
     */    
    Mesh* BillBoards332(float width, float height);

    /** Normal is used as updated position
     */
    Mesh* BillBoards332(int num, float width, float height);

    /** the last 1 float is for free use
     */
    Mesh* BillBoards3321(int num, float width, float height);

    /** the last 1 float is for free use
     */
    Mesh* Bird3321(int num, float width, float height);
  }// namespace mesh
  
}// namespace cs559


#endif /* defined(__cs559fltk__gen_model__) */
