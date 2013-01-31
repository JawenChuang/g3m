//
//  SGLayerNode.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/9/12.
//
//

#include "SGLayerNode.hpp"

#include "IGLTextureId.hpp"
#include "GL.hpp"
#include "Context.hpp"
#include "IDownloader.hpp"
#include "SGShape.hpp"
#include "IImageDownloadListener.hpp"
#include "TexturesHandler.hpp"
#include "IStringBuilder.hpp"

#define TEXTURES_DOWNLOAD_PRIORITY 1000000


class ImageDownloadListener : public IImageDownloadListener {
private:
  SGLayerNode* _layerNode;

public:
  ImageDownloadListener(SGLayerNode* layerNode) :
  _layerNode(layerNode)
  {

  }

  void onDownload(const URL& url,
                  const IImage* image) {
    _layerNode->onImageDownload(image);
  }

  void onError(const URL& url) {
    ILogger::instance()->logWarning("Can't download texture \"%s\"",
                                    url.getPath().c_str());
  }

  void onCancel(const URL& url) {

  }

  void onCanceledDownload(const URL& url,
                          const IImage* image) {

  }
};


void SGLayerNode::onImageDownload(const IImage* image) {
  if (_downloadedImage != NULL) {
    IFactory::instance()->deleteImage(_downloadedImage);
  }
  _downloadedImage = image->shallowCopy();
}

URL SGLayerNode::getURL() const {
  IStringBuilder *isb = IStringBuilder::newStringBuilder();
  isb->addString(_shape->getURIPrefix());
  isb->addString(_uri);
  const std::string path = isb->getString();
  delete isb;

  return URL(path, false);
}

void SGLayerNode::requestImage() {
  if (_uri.compare("") == 0) {
    return;
  }

  if (_context == NULL) {
    return;
  }

  _context->getDownloader()->requestImage(getURL(),
                                          TEXTURES_DOWNLOAD_PRIORITY,
                                          TimeInterval::fromDays(30),
                                          new ImageDownloadListener(this),
                                          true);
}

void SGLayerNode::initialize(const G3MContext* context,
                             SGShape *shape) {
  SGNode::initialize(context, shape);

  if (!_initialized) {
    _initialized = true;
    requestImage();
  }
}

const IGLTextureId* SGLayerNode::getTextureId(const G3MRenderContext* rc) {
  if (_textureId == NULL) {
    if (_downloadedImage != NULL) {
      const bool hasMipMap = false;
      _textureId = rc->getTexturesHandler()->getGLTextureId(_downloadedImage,
                                                            GLFormat::rgba(),
                                                            getURL().getPath(),
                                                            hasMipMap);

      IFactory::instance()->deleteImage(_downloadedImage);
      _downloadedImage = NULL;
    }
  }
  return _textureId;
}

const GLState* SGLayerNode::createState(const G3MRenderContext* rc,
                                        const GLState& parentState) {
  const IGLTextureId* texId = getTextureId(rc);
  if (texId == NULL) {
    return NULL;
  }

  GLState* state = new GLState(parentState);
  state->enableTextures();
  state->enableTexture2D();

  GL* gl = rc->getGL();
  gl->bindTexture(texId);

  return state;
}
