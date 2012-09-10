//
//  Factory_iOS.h
//  G3MiOSSDK
//
//  Created by Agustín Trujillo Pino on 31/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef G3MiOSSDK_Factory_iOS_h
#define G3MiOSSDK_Factory_iOS_h

#include "IFactory.hpp"

#include "Timer_iOS.hpp"
#include "Image_iOS.hpp"

#include "ByteBuffer_iOS.hpp"
#include "FloatBuffer_iOS.hpp"
#include "IntBuffer_iOS.hpp"

class Factory_iOS: public IFactory {
public:
  
  ITimer* createTimer() const {
    return new Timer_iOS();
  }
  
  void deleteTimer(const ITimer* timer) const {
    delete timer;
  }
  
  IImage* createImageFromSize(int width, int height) const
  {
    return new Image_iOS(width, height);
  }
  
  IImage* createImageFromFileName(const std::string filename) const {
    NSString *fn = [NSString stringWithCString:filename.c_str()
                                      encoding:[NSString defaultCStringEncoding]];
    
    UIImage* image = [UIImage imageNamed:fn];
    if (!image) {
      printf("Can't read image %s\n", filename.c_str());
      
      return NULL;
    }
    
    return new Image_iOS(image);
  }
  
  virtual IImage* createImageFromData(const ByteArrayWrapper* buffer) const {
    NSData* data = [NSData dataWithBytes: buffer->getData()
                                  length: buffer->getLength()];
    
    UIImage* image = [UIImage imageWithData:data];
    if (!image) {
      printf("Can't read image from ByteArrayWrapper of %d bytes\n", buffer->getLength());
      return NULL;
    }
    
    return new Image_iOS(image);
  }
  
  void deleteImage(const IImage* image) const {
    delete image;
  }
  
  virtual IByteBuffer* createByteBuffer(unsigned char data[], int length) const{
    return new ByteBuffer_iOS(data, length);
  }
  
  IFloatBuffer* createFloatBuffer(int size) const {
    return new FloatBuffer_iOS(size);
  }
  
  IIntBuffer* createIntBuffer(int size) const {
    return new IntBuffer_iOS(size);

  }
  
};

#endif
