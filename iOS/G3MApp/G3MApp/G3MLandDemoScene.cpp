//
//  G3MLandDemoScene.cpp
//  G3MApp
//
//  Created by Diego Gomez Deck on 10/4/16.
//

#include "G3MLandDemoScene.hpp"

#include "G3MDemoModel.hpp"

#include <G3MiOSSDK/LayerSet.hpp>
#include <G3MiOSSDK/URLTemplateLayer.hpp>
#include <G3MiOSSDK/PlanetRenderer.hpp>


void G3MLandDemoScene::rawSelectOption(const std::string& option,
                                       int optionIndex) {
  // no options
}

void G3MLandDemoScene::rawActivate(const G3MContext* context) {
#warning Diego at work!

  PlanetRenderer* planetRenderer = getModel()->getPlanetRenderer();
  planetRenderer->setShowStatistics(true);
  planetRenderer->setIncrementalTileQuality(true);


  // https://mapzen.com/blog/elevation/
  URLTemplateLayer* layer = URLTemplateLayer::newMercator("https://terrain-preview.mapzen.com/normal/{z}/{x}/{y}.png",
                                                          Sector::FULL_SPHERE,
                                                          false,                      // isTransparent
                                                          2,                          // firstLevel
                                                          15,                         // maxLevel
                                                          TimeInterval::fromDays(30)  // timeToCache
                                                          );

  getModel()->getLayerSet()->addLayer( layer );
}

