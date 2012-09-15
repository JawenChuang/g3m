

package org.glob3.mobile.specific;

import org.glob3.mobile.generated.IFloatBuffer;

import com.google.gwt.core.client.JavaScriptObject;


public class FloatBuffer_WebGL
         extends
            IFloatBuffer {

   private final JavaScriptObject _buffer;
   private final int              _timestamp = 0;


   public FloatBuffer_WebGL(final JavaScriptObject data) {
      _buffer = jsCreateBuffer(data);
   }


   public FloatBuffer_WebGL(final int size) {
      _buffer = jsCreateBuffer(size);
   }


   FloatBuffer_WebGL(final byte[] data) {
      //      TODO needed??
      throw new RuntimeException("ByteBuffer_WebGL(final byte[] data) IS NOT IMPLEMENTED");
   }


   @Override
   public int size() {
      return jsSize();
   }


   @Override
   public int timestamp() {
      return _timestamp;
   }


   @Override
   public float get(final int i) {
      return jsGet(i);
   }


   @Override
   public void put(final int i,
                   final float value) {
      jsPut(i, value);
   }


   public JavaScriptObject getBuffer() {
      return _buffer;
   }


   private native JavaScriptObject jsCreateBuffer(final JavaScriptObject data) /*-{
		return new Float32Array(data);
   }-*/;


   private native JavaScriptObject jsCreateBuffer(final int size) /*-{
		return new Float32Array(size);
   }-*/;


   private native int jsSize() /*-{
		return this.@org.glob3.mobile.specific.FloatBuffer_WebGL::_buffer.length;
   }-*/;


   private native byte jsGet(int i) /*-{
		this.@org.glob3.mobile.specific.FloatBuffer_WebGL::_buffer[i];
   }-*/;


   private native void jsPut(int i,
                             float value) /*-{
		var thisInstance = this;
		if (thisInstance.@org.glob3.mobile.specific.FloatBuffer_WebGL::_buffer[i] != value) {
			thisInstance.@org.glob3.mobile.specific.FloatBuffer_WebGL::_buffer[i] = value;
			thisInstance.@org.glob3.mobile.specific.FloatBuffer_WebGL::_timestamp++;
		}
   }-*/;

}
