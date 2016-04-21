//
//  CityGMLBuildingTessellator.hpp
//  G3MiOSSDK
//
//  Created by Jose Miguel SN on 13/4/16.
//
//

#ifndef CityGMLBuildingTessellator_hpp
#define CityGMLBuildingTessellator_hpp

#include "CityGMLBuilding.hpp"
#include "IndexedMesh.hpp"
#include "CompositeMesh.hpp"
#include "CityGMLBuildingColorProvider.hpp"
#include "Mark.hpp"

class DefaultCityGMLBuildingTessellatorData: public CityGMLBuildingTessellatorData{
public:
  Mesh* _containerMesh;
  short _firstVertexIndexWithinContainerMesh;
  short _lastVertexIndexWithinContainerMesh;
  
  DefaultCityGMLBuildingTessellatorData(Mesh* containerMesh,
                                        short firstVertexIndexWithinContainerMesh,
                                        short lastVertexIndexWithinContainerMesh):
  _containerMesh(containerMesh),
  _firstVertexIndexWithinContainerMesh(firstVertexIndexWithinContainerMesh),
  _lastVertexIndexWithinContainerMesh(lastVertexIndexWithinContainerMesh)
  {
    
    if (_firstVertexIndexWithinContainerMesh < 0 ||
        _lastVertexIndexWithinContainerMesh < 0 ||
        _lastVertexIndexWithinContainerMesh < _firstVertexIndexWithinContainerMesh){
      THROW_EXCEPTION("DefaultCityGMLBuildingTessellatorData constructor. Wrong parameters.");
    }
    
  }
  
                                        
};

class CityGMLBuildingTessellator{
  
  static short addTrianglesCuttingEarsForAllWalls(const CityGMLBuilding* building,
                                                  FloatBufferBuilderFromCartesian3D& fbb,
                                                  FloatBufferBuilderFromCartesian3D& normals,
                                                  ShortBufferBuilder& indexes,
                                                  FloatBufferBuilderFromColor& colors,
                                                  const double baseHeight,
                                                  const Planet& planet,
                                                  const short firstIndex,
                                                  const Color& color,
                                                  const bool includeGround,
                                                  const ElevationData* elevationData);
  
  
  static Mesh* createIndexedMeshWithColorPerVertex(const CityGMLBuilding* building,
                                                   const Planet planet,
                                                   const bool fixOnGround,
                                                   const Color color,
                                                   const bool includeGround,
                                                   const ElevationData* elevationData);
  
public:
 
  
  static Mesh* createMesh(const std::vector<CityGMLBuilding*> buildings,
                          const Planet& planet,
                          const bool fixOnGround,
                          const bool includeGround,
                          CityGMLBuildingColorProvider* colorProvider,
                          const ElevationData* elevationData);
  
  static void changeColorOfBuildingInBoundedMesh(const CityGMLBuilding* building, const Color& color);
  
  static Mark* createMark(const CityGMLBuilding* building, const bool fixOnGround);
  
};

#endif /* CityGMLBuildingTessellator_hpp */