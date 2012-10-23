package org.glob3.mobile.generated; 
public class G3MWidget
{

  public static G3MWidget create(FrameTasksExecutor frameTasksExecutor, IFactory factory, IStringUtils stringUtils, IThreadUtils threadUtils, IStringBuilder stringBuilder, IMathUtils mathUtils, IJSONParser jsonParser, ILogger logger, GL gl, TexturesHandler texturesHandler, TextureBuilder textureBuilder, IDownloader downloader, Planet planet, java.util.ArrayList<ICameraConstrainer> cameraConstrainers, Renderer renderer, Renderer busyRenderer, EffectsScheduler effectsScheduler, int width, int height, Color backgroundColor, boolean logFPS, boolean logDownloaderStatistics, GTask initializationTask, boolean autoDeleteInitializationTask)
  {
	if (logger != null)
	{
	  logger.logInfo("Creating G3MWidget...");
	}
  
	IFactory.setInstance(factory);
	IStringUtils.setInstance(stringUtils);
	ILogger.setInstance(logger);
	IThreadUtils.setInstance(threadUtils);
	IStringBuilder.setInstance(stringBuilder);
	IMathUtils.setInstance(mathUtils);
	IJSONParser.setInstance(jsonParser);
  
	return new G3MWidget(frameTasksExecutor, factory, stringUtils, threadUtils, logger, gl, texturesHandler, textureBuilder, downloader, planet, cameraConstrainers, renderer, busyRenderer, effectsScheduler, width, height, backgroundColor, logFPS, logDownloaderStatistics, initializationTask, autoDeleteInitializationTask);
  }

  public void dispose()
  {
	if (_userData != null)
	{
	  if (_userData != null)
		  _userData.dispose();
	}
  
	if (_factory != null)
		_factory.dispose();
	if (_logger != null)
		_logger.dispose();
	if (_gl != null)
		_gl.dispose();
	if (_renderer != null)
		_renderer.dispose();
	if (_busyRenderer != null)
		_busyRenderer.dispose();
	if (_effectsScheduler != null)
		_effectsScheduler.dispose();
	if (_currentCamera != null)
		_currentCamera.dispose();
	if (_nextCamera != null)
		_nextCamera.dispose();
	if (_texturesHandler != null)
		_texturesHandler.dispose();
	if (_timer != null)
		_timer.dispose();
  
	if (_downloader != null)
	{
	  _downloader.stop();
	}
  
	if (_frameTasksExecutor != null)
		_frameTasksExecutor.dispose();
  
  }

  public final void render()
  {
	_timer.start();
	_renderCounter++;
  
	//Start periodical task
	for (int i = 0; i < _periodicalTasks.size(); i++)
	{
	  PeriodicalTask pt = _periodicalTasks.get(i);
	  pt.executeIfNecessary();
	}
  
	// give to the CameraContrainers the opportunity to change the nextCamera
	for (int i = 0; i< _cameraConstrainers.size(); i++)
	{
	  ICameraConstrainer constrainer = _cameraConstrainers.get(i);
	  constrainer.onCameraChange(_planet, _currentCamera, _nextCamera);
	}
	_currentCamera.copyFrom(_nextCamera);
  
  
	if (_initializationTask != null)
	{
	  _initializationTask.run();
	  if (_autoDeleteInitializationTask)
	  {
		if (_initializationTask != null)
			_initializationTask.dispose();
	  }
	  _initializationTask = null;
	}
  
	RenderContext rc = new RenderContext(_frameTasksExecutor, _factory, _stringUtils, _threadUtils, _logger, _planet, _gl, _currentCamera, _nextCamera, _texturesHandler, _textureBuilder, _downloader, _effectsScheduler, _factory.createTimer());
  
	_effectsScheduler.doOneCyle(rc);
  
	_frameTasksExecutor.doPreRenderCycle(rc);
  
	_rendererReady = _renderer.isReadyToRender(rc);
  
	Renderer selectedRenderer = _rendererReady ? _renderer : _busyRenderer;
	if (selectedRenderer != _selectedRenderer)
	{
	  if (_selectedRenderer != null)
	  {
		_selectedRenderer.stop();
	  }
	  _selectedRenderer = selectedRenderer;
	  _selectedRenderer.start();
	}
  
	_gl.clearScreen(_backgroundColor);
  
	if (_selectedRenderer.isEnable())
	{
	  _selectedRenderer.render(rc);
	}
  
	//  _frameTasksExecutor->doPostRenderCycle(&rc);
  
	final TimeInterval elapsedTime = _timer.elapsedTime();
	if (elapsedTime.milliseconds() > 100)
	{
	  _logger.logWarning("Frame took too much time: %dms", elapsedTime.milliseconds());
	}
  
	if (_logFPS)
	{
	  _totalRenderTime += elapsedTime.milliseconds();
  
	  if ((_renderStatisticsTimer == null) || (_renderStatisticsTimer.elapsedTime().seconds() > 2))
	  {
		final double averageTimePerRender = (double) _totalRenderTime / _renderCounter;
		final double fps = 1000.0 / averageTimePerRender;
		_logger.logInfo("FPS=%f", fps);
  
		_renderCounter = 0;
		_totalRenderTime = 0;
  
		if (_renderStatisticsTimer == null)
		{
		  _renderStatisticsTimer = _factory.createTimer();
		}
		else
		{
		  _renderStatisticsTimer.start();
		}
	  }
	}
  
	if (_logDownloaderStatistics)
	{
	  String cacheStatistics = "";
  
	  if (_downloader != null)
	  {
		cacheStatistics = _downloader.statistics();
	  }
  
	  if (!_lastCacheStatistics.equals(cacheStatistics))
	  {
		_logger.logInfo("%s", cacheStatistics);
		_lastCacheStatistics = cacheStatistics;
	  }
	}
  
  }

  public final void onTouchEvent(TouchEvent myEvent)
  {
	if (_rendererReady)
	{
	  if (_renderer.isEnable())
	  {
		EventContext ec = new EventContext(_factory, _stringUtils, _threadUtils, _logger, _planet, _downloader, _effectsScheduler);
  
		_renderer.onTouchEvent(ec, myEvent);
	  }
	}
  }

  public final void onResizeViewportEvent(int width, int height)
  {
	if (_rendererReady)
	{
	  if (_renderer.isEnable())
	  {
		EventContext ec = new EventContext(_factory, _stringUtils, _threadUtils, _logger, _planet, _downloader, _effectsScheduler);
  
		_renderer.onResizeViewportEvent(ec, width, height);
	  }
	}
  }

  public final void onPause()
  {
	InitializationContext ic = new InitializationContext(_factory, _stringUtils, _threadUtils, _logger, _planet, _downloader, _effectsScheduler);
  
	_renderer.onPause(ic);
	_busyRenderer.onPause(ic);
  
	_effectsScheduler.onPause(ic);
  
	if (_downloader != null)
	{
	  _downloader.onPause(ic);
	}
  }

  public final void onResume()
  {
	InitializationContext ic = new InitializationContext(_factory, _stringUtils, _threadUtils, _logger, _planet, _downloader, _effectsScheduler);
  
	_renderer.onResume(ic);
	_busyRenderer.onResume(ic);
  
	_effectsScheduler.onResume(ic);
  
	if (_downloader != null)
	{
	  _downloader.onResume(ic);
	}
  }

//C++ TO JAVA CONVERTER WARNING: 'const' methods are not available in Java:
//ORIGINAL LINE: GL* getGL() const
  public final GL getGL()
  {
	return _gl;
  }

//  const Camera* getCurrentCamera() const {
//    return _currentCamera;
//  }

//C++ TO JAVA CONVERTER WARNING: 'const' methods are not available in Java:
//ORIGINAL LINE: Camera* getNextCamera() const
  public final Camera getNextCamera()
  {
	return _nextCamera;
  }

  public final void setUserData(UserData userData)
  {
	if (_userData != null)
	{
	  if (_userData != null)
		  _userData.dispose();
	}
	_userData = userData;
	if (_userData != null)
	{
	  _userData.setWidget(this);
	}
  }

//C++ TO JAVA CONVERTER WARNING: 'const' methods are not available in Java:
//ORIGINAL LINE: UserData* getUserData() const
  public final UserData getUserData()
  {
	return _userData;
  }

  //Periodical Tasks
  public final void addPeriodicalTasks(TimeInterval interval, GTask task)
  {
	PeriodicalTask pt = new PeriodicalTask(interval, task);
	_periodicalTasks.add(pt);
  }

  private FrameTasksExecutor _frameTasksExecutor;
  private IFactory _factory;
  private final IStringUtils _stringUtils;
  private IThreadUtils _threadUtils;
  private ILogger _logger;
  private GL _gl;
  private Planet _planet; // REMOVED FINAL WORD BY CONVERSOR RULE
  private Renderer _renderer;
  private Renderer _busyRenderer;
  private EffectsScheduler _effectsScheduler;

  private java.util.ArrayList<ICameraConstrainer> _cameraConstrainers = new java.util.ArrayList<ICameraConstrainer>();

  private Camera _currentCamera;
  private Camera _nextCamera;
  private IDownloader _downloader;
  private TexturesHandler _texturesHandler;
  private TextureBuilder _textureBuilder;
  private final Color _backgroundColor ;

  private ITimer _timer;
  private int _renderCounter;
  private int _totalRenderTime;
  private final boolean _logFPS;
  private final boolean _logDownloaderStatistics;
  private String _lastCacheStatistics;

  private boolean _rendererReady;
  private Renderer _selectedRenderer;

  private ITimer _renderStatisticsTimer;

  private UserData _userData;

  private GTask _initializationTask;
  private boolean _autoDeleteInitializationTask;

  //Storing Scheduled Tasks
  private java.util.ArrayList<PeriodicalTask> _periodicalTasks = new java.util.ArrayList<PeriodicalTask>();

  private void initializeGL()
  {
	_gl.enableDepthTest();
  
	_gl.enableCullFace(GLCullFace.back());
  }


  ///#include "ITimer.hpp"
  
  
  
  private G3MWidget(FrameTasksExecutor frameTasksExecutor, IFactory factory, IStringUtils stringUtils, IThreadUtils threadUtils, ILogger logger, GL gl, TexturesHandler texturesHandler, TextureBuilder textureBuilder, IDownloader downloader, Planet planet, java.util.ArrayList<ICameraConstrainer> cameraConstrainers, Renderer renderer, Renderer busyRenderer, EffectsScheduler effectsScheduler, int width, int height, Color backgroundColor, boolean logFPS, boolean logDownloaderStatistics, GTask initializationTask, boolean autoDeleteInitializationTask)
  {
	  _frameTasksExecutor = frameTasksExecutor;
	  _factory = factory;
	  _stringUtils = stringUtils;
	  _threadUtils = threadUtils;
	  _logger = logger;
	  _gl = gl;
	  _texturesHandler = texturesHandler;
	  _textureBuilder = textureBuilder;
	  _planet = planet;
	  _cameraConstrainers = cameraConstrainers;
	  _renderer = renderer;
	  _busyRenderer = busyRenderer;
	  _effectsScheduler = effectsScheduler;
	  _currentCamera = new Camera(width, height);
	  _nextCamera = new Camera(width, height);
	  _backgroundColor = backgroundColor;
	  _timer = factory.createTimer();
	  _renderCounter = 0;
	  _totalRenderTime = 0;
	  _logFPS = logFPS;
	  _downloader = downloader;
	  _rendererReady = false;
	  _selectedRenderer = null;
	  _renderStatisticsTimer = null;
	  _logDownloaderStatistics = logDownloaderStatistics;
	  _userData = null;
	  _initializationTask = initializationTask;
	  _autoDeleteInitializationTask = autoDeleteInitializationTask;
	initializeGL();
  
	InitializationContext ic = new InitializationContext(_factory, _stringUtils, _threadUtils, _logger, _planet, _downloader, _effectsScheduler);
  
	_effectsScheduler.initialize(ic);
	_renderer.initialize(ic);
	_busyRenderer.initialize(ic);
	_currentCamera.initialize(ic);
	_nextCamera.initialize(ic);
  
	if (_downloader != null)
	{
	  _downloader.start();
	}
  }

}