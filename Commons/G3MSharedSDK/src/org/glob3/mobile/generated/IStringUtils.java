package org.glob3.mobile.generated; 
//
//  IStringUtils.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 23/08/12.
//
//

//
//  IStringUtils.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 23/08/12.
//
//




public abstract class IStringUtils
{
  private static IStringUtils _instance;

  public static void setInstance(IStringUtils instance)
  {
    if (_instance != null)
    {
      ILogger.instance().logWarning("IStringUtils instance already set!");
      if (_instance != null)
         _instance.dispose();
    }
    _instance = instance;
  }

  public static IStringUtils instance()
  {
    return _instance;
  }

  public void dispose()
  {
  }

  public abstract String createString(byte[] data, int length);

  public abstract java.util.ArrayList<String> splitLines(String String);

  public abstract boolean beginsWith(String String, String prefix);

  public abstract boolean endsWith(String String, String suffix);

  public abstract String toUpperCase(String String);


  public abstract int indexOf(String String, String search);

  /*
   Returns a new string that is a substring of this string. The substring begins at the
   specified beginIndex and extends to the character at index endIndex - 1. Thus the length
   of the substring is endIndex-beginIndex.
   */
  public abstract String substring(String String, int beginIndex, int endIndex);

  public String substring(String String, int beginIndex)
  {
    //    return substring(string, beginIndex, string.size() + 1);
    return substring(String, beginIndex, String.length());
  }

  public String replaceSubstring(String originalString, String toReplace, String replaceWith)
  {
    int startIndex = indexOf(originalString, toReplace);
    //The part to replace was not found. Return original String
    if (startIndex == -1)
    {
      return originalString;
    }
    final int endIndex = startIndex + toReplace.length();
    final String left = substring(originalString, 0, startIndex);
    final String right = substring(originalString, endIndex);
    final String result = left + replaceWith + right;
    startIndex = indexOf(result, toReplace);
    if (startIndex != -1)
    {
      //recursive call to replace other ocurrences
      return replaceSubstring(result, toReplace, replaceWith);
    }
    return result;
  }

  public String left(String String, int endIndex)
  {
    return substring(String, 0, endIndex);
  }

  public abstract String rtrim(String String);

  public abstract String ltrim(String String);

  public String trim(String String)
  {
    return rtrim(ltrim(String));
  }
  public abstract String capitalize(String String);

  public abstract long parseHexInt(String str);

}