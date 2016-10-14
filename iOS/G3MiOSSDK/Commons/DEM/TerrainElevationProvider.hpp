//
//  TerrainElevationProvider.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 10/13/16.
//
//

#ifndef TerrainElevationProvider_hpp
#define TerrainElevationProvider_hpp

#include "RCObject.hpp"

class RenderState;
class G3MContext;


class TerrainElevationProvider : public RCObject {
protected:
  virtual ~TerrainElevationProvider() {
#ifdef JAVA_CODE
    super.dispose();
#endif
  }

public:
  virtual RenderState getRenderState() = 0;

  virtual void initialize(const G3MContext* context) = 0;

  virtual void cancel() = 0;

};

#endif