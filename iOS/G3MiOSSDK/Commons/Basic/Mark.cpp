//
//  Mark.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 06/06/12.
//  Copyright (c) 2012 IGO Software SL. All rights reserved.
//

#include "Mark.hpp"
#include "Camera.hpp"


void Mark::render(const RenderContext* rc,
                  const double minDistanceToCamera) {
  int __dgd_at_work;
  
  const Camera* camera = rc->getCamera();
  const Planet* planet = rc->getPlanet();
  
  const Vector3D cameraPosition = camera->getPos();
  const Vector3D markPosition = planet->toVector3D(_position);
  
  const Vector3D markCameraVector = markPosition.sub(cameraPosition);
  const double distanceToCamera = markCameraVector.length();
  
  if (distanceToCamera <= minDistanceToCamera || true) {
    const Vector3D normalAtMarkPosition = planet->geodeticSurfaceNormal(markPosition);
    
    if (normalAtMarkPosition.angleBetween(markCameraVector).radians() > M_PI / 2) {
      IGL* gl = rc->getGL();
      
      if (_textureId < 1) {
        _textureId = rc->getTexturesHandler()->getTextureIdFromFileName(rc, _textureFilename, 128, 128);
      }
      
      if (_textureId < 1) {
        rc->getLogger()->logError("Can't load file %s", _textureFilename.c_str());
        return;
      }
      
//    rc->getLogger()->logInfo(" Visible   << %f %f", minDist, distanceToCamera);
      gl->drawBillBoard(_textureId,
                        (float) markPosition.x(), (float) markPosition.y(), (float) markPosition.z(),
                        camera->getViewPortRatio());
    }
    
  }
  
}
