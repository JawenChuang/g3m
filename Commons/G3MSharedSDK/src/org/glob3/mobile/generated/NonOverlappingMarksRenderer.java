package org.glob3.mobile.generated; 
public class NonOverlappingMarksRenderer extends DefaultRenderer
{

  private final int _maxVisibleMarks;
  private final int _maxConvergenceSteps;

  private java.util.ArrayList<NonOverlappingMark> _visibleMarks = new java.util.ArrayList<NonOverlappingMark>();
  private java.util.ArrayList<NonOverlappingMark> _marks = new java.util.ArrayList<NonOverlappingMark>();

  private void computeMarksToBeRendered(Camera cam, Planet planet)
  {
  
    _visibleMarks.clear();
  
    final Frustum frustrum = cam.getFrustumInModelCoordinates();
  
    for (int i = 0; i < _marks.size(); i++)
    {
      NonOverlappingMark m = _marks.get(i);
  
      if (_visibleMarks.size() < _maxVisibleMarks && frustrum.contains(m.getCartesianPosition(planet)))
      {
        _visibleMarks.add(m);
      }
      else
      {
        //Resetting marks location of invisible anchors
        m.resetWidgetPositionVelocityAndForce();
      }
    }
  }

  private long _lastPositionsUpdatedTime;

  private GLState _connectorsGLState;
  private void renderConnectorLines(G3MRenderContext rc)
  {
    if (_connectorsGLState == null)
    {
      _connectorsGLState = new GLState();
  
      _connectorsGLState.addGLFeature(new FlatColorGLFeature(Color.black()), false);
    }
  
    _connectorsGLState.clearGLFeatureGroup(GLFeatureGroupName.NO_GROUP);
  
    FloatBufferBuilderFromCartesian2D pos2D = new FloatBufferBuilderFromCartesian2D();
  
    for (int i = 0; i < _visibleMarks.size(); i++)
    {
      Vector2F sp = _visibleMarks.get(i).getScreenPos();
      Vector2F asp = _visibleMarks.get(i).getAnchorScreenPos();
  
      pos2D.add(sp._x, -sp._y);
      pos2D.add(asp._x, -asp._y);
  
    }
  
    _connectorsGLState.addGLFeature(new Geometry2DGLFeature(pos2D.create(), 2, 0, true, 0, 3.0f, true, 10.0f, new Vector2F(0.0f,0.0f)), false);
  
    _connectorsGLState.addGLFeature(new ViewportExtentGLFeature((int)rc.getCurrentCamera().getViewPortWidth(), (int)rc.getCurrentCamera().getViewPortHeight()), false);
  
    rc.getGL().drawArrays(GLPrimitive.lines(), 0, pos2D.size()/2, _connectorsGLState, rc.getGPUProgramManager());
  }

  private void computeForces(Camera cam, Planet planet)
  {
  
    //Compute Mark Anchor Screen Positions
    for (int i = 0; i < _visibleMarks.size(); i++)
    {
      _visibleMarks.get(i).computeAnchorScreenPos(cam, planet);
    }
  
    //Compute Mark Forces
    for (int i = 0; i < _visibleMarks.size(); i++)
    {
      NonOverlappingMark mark = _visibleMarks.get(i);
      mark.applyHookesLaw();
  
      for (int j = i+1; j < _visibleMarks.size(); j++)
      {
        mark.applyCoulombsLaw(_visibleMarks.get(j));
      }
  
      for (int j = 0; j < _visibleMarks.size(); j++)
      {
        if (i != j)
        {
          mark.applyCoulombsLawFromAnchor(_visibleMarks.get(j));
        }
      }
    }
  }
  private void renderMarks(G3MRenderContext rc, GLState glState)
  {
    //Draw Lines
    renderConnectorLines(rc);
  
    //Draw Anchors and Marks
    for (int i = 0; i < _visibleMarks.size(); i++)
    {
      _visibleMarks.get(i).render(rc, glState);
    }
  }
  private void applyForces(long now, Camera cam)
  {
  
    if (_lastPositionsUpdatedTime != 0) //If not First frame
    {
  
      //Update Position based on last Forces
      for (int i = 0; i < _visibleMarks.size(); i++)
      {
        _visibleMarks.get(i).updatePositionWithCurrentForce(now - _lastPositionsUpdatedTime, cam.getViewPortWidth(), cam.getViewPortHeight());
      }
    }
  
    _lastPositionsUpdatedTime = now;
  }


  public NonOverlappingMarksRenderer(int maxVisibleMarks)
  {
     this(maxVisibleMarks, -1);
  }
  public NonOverlappingMarksRenderer()
  {
     this(30, -1);
  }
  public NonOverlappingMarksRenderer(int maxVisibleMarks, int maxConvergenceSteps)
  {
     _maxVisibleMarks = maxVisibleMarks;
     _maxConvergenceSteps = maxConvergenceSteps;
     _lastPositionsUpdatedTime = 0;
     _connectorsGLState = null;
  
  }

  public void dispose()
  {
    _connectorsGLState._release();
  
    for (int i = 0; i < _marks.size(); i++)
    {
      if (_marks.get(i) != null)
         _marks.get(i).dispose();
    }
  }

  public final void addMark(NonOverlappingMark mark)
  {
    _marks.add(mark);
  }

  public final void removeAllMarks()
  {
    final int marksSize = _marks.size();
    for (int i = 0; i < marksSize; i++)
    {
      if (_marks.get(i) != null)
         _marks.get(i).dispose();
    }
    _marks.clear();
  }

  public RenderState getRenderState(G3MRenderContext rc)
  {
    return RenderState.ready();
  }

  public void render(G3MRenderContext rc, GLState glState)
  {
  
    final Camera cam = rc.getCurrentCamera();
    final Planet planet = rc.getPlanet();
  
    if (_maxConvergenceSteps > 0)
    {
  
      //Looking for convergence on _maxConvergenceSteps
  
      long timeStep = 40;
  
      computeMarksToBeRendered(cam, planet);
  
      computeForces(cam, planet);
  
      applyForces(_lastPositionsUpdatedTime + timeStep, cam);
  
      int iteration = 0;
      while (marksAreMoving() && iteration < _maxConvergenceSteps)
      {
        computeForces(cam, planet);
        applyForces(_lastPositionsUpdatedTime + timeStep, cam);
        iteration++;
      }
  
    }
    else
    {
      //Real Time
      computeMarksToBeRendered(cam, planet);
      computeForces(cam, planet);
      applyForces(rc.getFrameStartTimer().nowInMilliseconds(), cam);
    }
  
    renderMarks(rc, glState);
  }

  public boolean onTouchEvent(G3MEventContext ec, TouchEvent touchEvent)
  {
  
    if (touchEvent.getTapCount() == 1)
    {
      final float x = touchEvent.getTouch(0).getPos()._x;
      final float y = touchEvent.getTouch(0).getPos()._y;
      for (int i = 0; i < _visibleMarks.size(); i++)
      {
        if (_visibleMarks.get(i).onTouchEvent(x, y))
        {
          return true;
        }
      }
    }
    return false;
  }

  public void onResizeViewportEvent(G3MEventContext ec, int width, int height)
  {
    for (int i = 0; i < _marks.size(); i++)
    {
      _marks.get(i).onResizeViewportEvent(width, height);
    }
  
  }

  public void start(G3MRenderContext rc)
  {

  }

  public void stop(G3MRenderContext rc)
  {

  }

  public SurfaceElevationProvider getSurfaceElevationProvider()
  {
    return null;
  }

  public PlanetRenderer getPlanetRenderer()
  {
    return null;
  }

  public boolean isPlanetRenderer()
  {
    return false;
  }

  public final boolean marksAreMoving()
  {
    for (int i = 0; i < _visibleMarks.size(); i++)
    {
      if (_visibleMarks.get(i).isMoving())
      {
        //      printf("Mark %d is moving", i);
        return true;
      }
    }
    return false;
  }


}
//C++ TO JAVA CONVERTER TODO TASK: There is no preprocessor in Java:
//#pragma mark MarkWidget

//C++ TO JAVA CONVERTER TODO TASK: There is no preprocessor in Java:
//#pragma mark NonOverlappingMark

//C++ TO JAVA CONVERTER TODO TASK: There is no preprocessor in Java:
//#pragma-mark Renderer