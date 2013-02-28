//
//  Layer.hpp
//  G3MiOSSDK
//
//  Created by José Miguel S N on 23/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef G3MiOSSDK_Layer_hpp
#define G3MiOSSDK_Layer_hpp

#include <string>

#include "Sector.hpp"
#include "IFactory.hpp"
#include "Context.hpp"
#include "URL.hpp"
#include "TerrainTouchEventListener.hpp"
#include "TimeInterval.hpp"

class Petition;
class Tile;
class LayerCondition;
class LayerSet;
class Vector2I;
class LayerTilesRenderParameters;

class Layer {
private:
  LayerCondition*                         _condition;
  std::vector<TerrainTouchEventListener*> _listeners;

  LayerSet* _layerSet;

  bool _enable;

  const std::string _name;

protected:
  const LayerTilesRenderParameters* _parameters;

  const TimeInterval& _timeToCache;

  void notifyChanges() const;

public:
  Layer(LayerCondition* condition,
        const std::string& name,
        const TimeInterval& timeToCache,
        const LayerTilesRenderParameters* parameters) :
  _condition(condition),
  _name(name),
  _layerSet(NULL),
  _timeToCache(timeToCache),
  _enable(true),
  _parameters(parameters)
  {

  }

  virtual void setEnable(bool enable) {
    if (enable != _enable) {
      _enable = enable;
      notifyChanges();
    }
  }

  virtual bool isEnable() const {
    return _enable;
  }

  virtual ~Layer();

  virtual std::vector<Petition*> getMapPetitions(const G3MRenderContext* rc,
                                                 const Tile* tile,
                                                 const Vector2I& tileTextureResolution) const = 0;

  virtual bool isAvailable(const G3MRenderContext* rc,
                           const Tile* tile) const;

  virtual bool isAvailable(const G3MEventContext* ec,
                           const Tile* tile) const;

  //  virtual bool isTransparent() const = 0;

  virtual URL getFeatureInfoURL(const Geodetic2D& position,
                                const Sector& sector) const = 0;

  virtual bool isReady() const {
    return true;
  }

  virtual void initialize(const G3MContext* context) {}

  void addTerrainTouchEventListener(TerrainTouchEventListener* listener) {
    _listeners.push_back(listener);
  }

  void onTerrainTouchEventListener(const G3MEventContext* ec,
                                   const TerrainTouchEvent& tte) const {
    for (unsigned int i = 0; i < _listeners.size(); i++) {
      TerrainTouchEventListener* listener = _listeners[i];
      if (listener != NULL) {
        listener->onTerrainTouchEvent(ec, tte);
      }
    }
  }

  void setLayerSet(LayerSet* layerSet);

  const std::string getName();

  const LayerTilesRenderParameters* getLayerTilesRenderParameters() const {
    return _parameters;
  }
  
};

#endif
