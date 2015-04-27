//
//  SGGeometryNode.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/8/12.
//
//

#include "SGGeometryNode.hpp"

#include "Context.hpp"
#include "GL.hpp"

#include "IFloatBuffer.hpp"
#include "IShortBuffer.hpp"

#include "GLState.hpp"
#include "Vector3D.hpp"
#include "Vector2F.hpp"

#include "FloatBufferBuilder.hpp"

SGGeometryNode::~SGGeometryNode() {
  delete _vertices;
  delete _colors;
  delete _uv;
  delete _normals;
  delete _indices;

  _glState->_release();

#ifdef JAVA_CODE
  super.dispose();
#endif

}

void SGGeometryNode::createGLState() {

  _glState->addGLFeature(new GeometryGLFeature(_vertices,    //The attribute is a float vector of 4 elements
                                               3,            //Our buffer contains elements of 3
                                               0,            //Index 0
                                               false,        //Not normalized
                                               0,            //Stride 0
                                               true,         //Depth test
                                               false, 0,
                                               false, (float)0.0, (float)0.0,
                                               (float)1.0,
                                               true, (float)1.0),
                         false);

  if (_normals != NULL) {

    //    _glState->addGLFeature(new DirectionLightGLFeature(Vector3D(1, 0,0),  Color::yellow(),
    //                                                      (float)0.0), false);

    _glState->addGLFeature(new VertexNormalGLFeature(_normals,3,0,false,0),
                           false);


  }

  if (_uv != NULL) {
    _glState->addGLFeature(new TextureCoordsGLFeature(_uv,
                                                      2,
                                                      0,
                                                      false,
                                                      0,
                                                      false,
                                                      Vector2F::zero(),
                                                      Vector2F::zero()) ,
                           false);
  }
}

void SGGeometryNode::rawRender(const G3MRenderContext* rc, const GLState* glState) {
  GL* gl = rc->getGL();
  gl->drawElements(_primitive, _indices, glState, *rc->getGPUProgramManager());
}

Vector3D SGGeometryNode::mostDistantVertexFromCenter(const MutableMatrix44D& transformation) const{
  double max = 0;
  MutableVector3D res(0, 0, 0);
  for (int i = 0; i < _vertices->size(); i+=3){
    Vector3D v(_vertices->get(i), _vertices->get(i+1), _vertices->get(i+2));
    
    Vector3D v2 = v.transformedBy(transformation, 1.0);
    double d = v2.squaredLength();
    if (max < d){
      max = d;
      res.copyFrom(v2);
    }
  }
  
  Vector3D res2 = SGNode::mostDistantVertexFromCenter(transformation);
  
  return (max > res2.squaredLength())? res.asVector3D() : res2;
}

Vector3D SGGeometryNode::getMax(const MutableMatrix44D& transformation){
  
  double maxX = 9e99, maxY= 9e99, maxZ= 9e99;
  
  for (int i = 0; i < _vertices->size(); i+=3){
    Vector3D v(_vertices->get(i), _vertices->get(i+1), _vertices->get(i+2));
    
    Vector3D v2 = v.transformedBy(transformation, 1.0);
    if (v2._x > maxX){
      maxX = v2._x;
    }
    
    if (v2._y > maxY){
      maxY = v2._y;
    }
    
    if (v2._z > maxZ){
      maxZ = v2._z;
    }
  }
  
  Vector3D max(maxX, maxY, maxZ);
  
  return Vector3D::maxOnAllAxis(max, SGNode::getMax(transformation));
}

Vector3D SGGeometryNode::getMin(const MutableMatrix44D& transformation){
  
  double minX= -9e99, minY= -9e99, minZ= -9e99;
  
  for (int i = 0; i < _vertices->size(); i+=3){
    Vector3D v(_vertices->get(i), _vertices->get(i+1), _vertices->get(i+2));
    
    Vector3D v2 = v.transformedBy(transformation, 1.0);
    if (v2._x < minX){
      minX = v2._x;
    }
    
    if (v2._y < minY){
      minY = v2._y;
    }
    if (v2._z < minZ){
      minZ = v2._z;
    }
  }
  
  Vector3D min(minX, minY, minZ);
  
  return Vector3D::minOnAllAxis(min, SGNode::getMin(transformation));
}
