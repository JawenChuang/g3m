

package org.glob3.mobile.client;

import java.util.ArrayList;

import com.google.gwt.core.client.JavaScriptObject;
import com.google.gwt.core.client.JsArray;
import com.google.gwt.core.client.JsArrayNumber;


public class XMLDocument {
   private final JavaScriptObject _xml;


   public XMLDocument(final String doc) {
      _xml = jsParse(doc);
   }


   public XMLDocument(final JavaScriptObject xml) {
      _xml = xml;
   }


   private native JavaScriptObject jsParse(final String doc) /*-{
		var xml = (new window.DOMParser()).parseFromString(doc, "text/xml");
		return xml;
   }-*/;


   public XPathResult xpath(final String xpath) {
      return new XPathResult(xpathToJSO(xpath));
   }


   public native String getTextContent() /*-{
		return this.@org.glob3.mobile.client.XMLDocument::_xml.textContent;
   }-*/;


   public ArrayList<Double> evaluateXPathAndGetTextContentAsNumberArray(final String xpath,
                                                                        final String separator) {
      final JavaScriptObject res = xpathToJSO(xpath);
      final JsArrayNumber a = jsGetTextContentAsNumberArray(separator, res);
      final ArrayList<Double> ns = new ArrayList<Double>();
      for (int i = 0; i < a.length(); i++) {
         ns.add(a.get(i));
      }
      return ns;
   }


   public ArrayList<Double> getTextContentAsNumberArray(final String separator) {
      final JsArrayNumber a = jsGetTextContentAsNumberArray(separator, _xml);
      final ArrayList<Double> ns = new ArrayList<Double>();
      for (int i = 0; i < a.length(); i++) {
         ns.add(a.get(i));
      }
      return ns;
   }


   public native JsArrayNumber jsGetTextContentAsNumberArray(String separator,
                                                             JavaScriptObject r) /*-{
		var ts = r.firstChild.textContent.split(separator);

		var n = [];
		for (i = 0; i < ts.length; i++) {
			n.push(parseFloat(ts[i]));
		}

		return n;

   }-*/;


   private native String jsGetTextContentAsText(JavaScriptObject xpathResult) /*-{
		var thisNode = xpathResult.iterateNext();
		return thisNode.textContent;
   }-*/;


   public String evaluateXPathAndGetTextContentAsText(final String xpath) {
      final JavaScriptObject res = xpathToJSO(xpath);
      return jsGetTextContentAsText(res);
   }


   public int evaluateXPathAndGetTextContentAsInteger(final String xpath) {
      final JavaScriptObject res = xpathToJSO(xpath);
      return getTextContentAsInteger(res);
   }


   public double evaluateXPathAndGetNumberValueAsDouble(final String xpath) {
      final JavaScriptObject res = xpathToJSO(xpath);
      return getNumberValueAsDouble(res);
   }


   public native double getNumberValueAsDouble(JavaScriptObject xpathResult) /*-{
		return xpathResult.numberValue;
   }-*/;


   public native int getTextContentAsInteger(JavaScriptObject xpathResult) /*-{
		var thisNode = xpathResult.iterateNext();
		return thisNode.textContent;
   }-*/;


   public native JsArray<JavaScriptObject> jsGetAsDocs(JavaScriptObject xpathResult) /*-{
		var thisNode = xpathResult.iterateNext();
		var array = [];
		while (thisNode != null) {
			array.push(thisNode);
			thisNode = xpathResult.iterateNext();
		}

		for (i = 0; i < array.length; i++) {
			var doc = document.implementation.createDocument('', '');
			doc.appendChild(array[i]);
			array[i] = doc;
		}

		return array;
   }-*/;


   public ArrayList<XMLDocument> evaluateXPathAsXMLDocuments(final String xpath) {
      final JavaScriptObject res = xpathToJSO(xpath);
      final JsArray<JavaScriptObject> a = jsGetAsDocs(res);

      final ArrayList<XMLDocument> out = new ArrayList<XMLDocument>();
      for (int i = 0; i < a.length(); i++) {
         out.add(new XMLDocument(a.get(i)));
      }
      return out;
   }


   public native JavaScriptObject xpathToJSO(final String xpath) /*-{

		var xml = this.@org.glob3.mobile.client.XMLDocument::_xml;

		//Resolver for CityGML
		function nsResolver(prefix) {
			switch (prefix) {
			case 'xhtml':
				return 'http://www.w3.org/1999/xhtml';
			case 'mathml':
				return 'http://www.w3.org/1998/Math/MathML';
			case 'gml':
				return 'http://www.opengis.net/gml';
			case 'bldg':
				return 'http://www.opengis.net/citygml/building/1.0';
			default:
				return 'http://www.opengis.net/citygml/1.0';
			}
		}

		//REMEMBER TO ERASE XMLNS= ATTRIBUTE!!!!!!!!!!!!
		try {
			var xpathResult = xml.evaluate(xpath, xml, nsResolver,
					XPathResult.ANY_TYPE, null);
		} catch (e) {
			debugger;
			console.log(e);
		}

		return xpathResult;
   }-*/;

}
