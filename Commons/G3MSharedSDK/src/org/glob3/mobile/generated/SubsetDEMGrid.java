package org.glob3.mobile.generated;
//
//  SubsetDEMGrid.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/17/16.
//
//

//
//  SubsetDEMGrid.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/17/16.
//
//




public class SubsetDEMGrid extends DecoratorDEMGrid
{
  private final int _offsetX;
  private final int _offsetY;

  private SubsetDEMGrid(DEMGrid grid, Sector sector, Vector2I extent, int offsetX, int offsetY)
  {
     super(grid, sector, extent);
     _offsetX = offsetX;
     _offsetY = offsetY;
  
  }


  public void dispose()
  {
    super.dispose();
  }



  ///#include "ILogger.hpp"
  
  
  public static SubsetDEMGrid create(DEMGrid grid, Sector sector)
  {
    final Projection projection = grid.getProjection();
    final Vector2D lowerUV = projection.getUV(sector._lower);
    final Vector2D upperUV = projection.getUV(sector._upper);
  
  //  ILogger::instance()->logInfo("%s -> %s | %s",
  //                               lowerUV.description().c_str(),
  //                               upperUV.description().c_str(),
  //                               upperUV.sub(lowerUV).description().c_str());
  
    final Vector2I gridExtent = grid.getExtent();
  
    final IMathUtils mu = IMathUtils.instance();
  
    final int offsetX = (int) mu.round(lowerUV._x * gridExtent._x);
    final int offsetY = (int) mu.round(upperUV._y * gridExtent._y);
    final int width = (int)(mu.round(upperUV._x * gridExtent._x) - offsetX);
    final int height = (int)(mu.round(lowerUV._y * gridExtent._y) - offsetY);
  
  //  const int offsetX = (int)  mu->round(lowerUV._x * gridExtent._x);
  //  const int offsetY = (int)  mu->round((1.0 - lowerUV._y) * gridExtent._y);
  //  const int width   = (int) (mu->round(upperUV._x * gridExtent._x) - offsetX);
  //  const int height  = (int) (mu->round((1.0 - upperUV._y) * gridExtent._y) - offsetY);
  
  //  const int offsetX = (int)  mu->round(lowerUV._x * gridExtent._x);
  //  const int offsetY = (int)  mu->round(lowerUV._y * gridExtent._y);
  //  const int width   = (int) (mu->round(upperUV._x * gridExtent._x) - offsetX);
  //  const int height  = (int) (mu->round(upperUV._y * gridExtent._y) - offsetY);
  
    return new SubsetDEMGrid(grid, sector, new Vector2I(width, height), offsetX, offsetY);
  }

  public final double getElevationAt(int x, int y)
  {
    return _grid.getElevationAt(_offsetX + x, _offsetY + y);
  }

}