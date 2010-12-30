/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTTP.h"

#ifdef _UIKIT_
#include <CFNetwork/CFNetwork.h>
#endif//_UIKIT_

#ifdef _COCOA_
#include <CoreServices/CoreServices.h>
#endif//_UIKIT_

nuiHTTPResponse* nuiHTTPRequest::SendRequest()
{
  char* pUrl = mUrl.Export();
	
  CFStringRef originalURLString = CFStringCreateWithCString(kCFAllocatorDefault, pUrl, kCFStringEncodingUTF8);
	
  CFStringRef preprocessedString =
  CFURLCreateStringByReplacingPercentEscapesUsingEncoding(kCFAllocatorDefault, originalURLString, CFSTR(""), kCFStringEncodingUTF8);
  CFStringRef urlString =
  CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault, preprocessedString, NULL, NULL, kCFStringEncodingUTF8);
	
  free(pUrl);
  CFURLRef url = CFURLCreateWithString(kCFAllocatorDefault, urlString, NULL);
  CFRelease(urlString);
  
  char* pMeth = mMethod.Export(); 
  CFStringRef method = CFStringCreateWithCString(kCFAllocatorDefault, pMeth, kCFStringEncodingUTF8);
  free(pMeth);
  
  CFHTTPMessageRef req = CFHTTPMessageCreateRequest(kCFAllocatorDefault, method, url, kCFHTTPVersion1_1);
  
  CFStringRef userAgentField = CFSTR("User-Agent");
  CFStringRef userAgentName = CFSTR("nuiHTTP/2.0");
  CFHTTPMessageSetHeaderFieldValue(req, userAgentField, userAgentName);
  CFRelease(userAgentField);
  CFRelease(userAgentName);
  
  nuiHTTPHeaderMap::const_iterator end = mHeaders.end();
  for (nuiHTTPHeaderMap::const_iterator it = mHeaders.begin(); it != end; ++it)
  {
    char* pName = it->first.Export();
    char* pVal = it->second.Export();
    CFStringRef fieldName = CFStringCreateWithCString(kCFAllocatorDefault, pName, kCFStringEncodingUTF8);
    CFStringRef fieldValue = CFStringCreateWithCString(kCFAllocatorDefault, pVal, kCFStringEncodingUTF8);
    
    CFHTTPMessageSetHeaderFieldValue(req, fieldName, fieldValue);
    
    CFRelease(fieldName);
    CFRelease(fieldValue);
    delete pName;
    delete pVal;
  }
  
  CFDataRef body = NULL;
  if (mBody.size())
  {
    body = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, (UInt8*)&mBody[0], mBody.size(), kCFAllocatorNull);
    CFHTTPMessageSetBody(req, body);
  }
  
  CFReadStreamRef readStream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, req);
  CFReadStreamOpen(readStream);
  
  std::vector<char> buf;
  CFIndex pos = 0;
  CFIndex size = 0;
  bool cont = true;
  do
  {
    buf.resize(pos + 1024);
    size = CFReadStreamRead(readStream, (UInt8*)&buf[pos], 1024);
    if (size == -1)
    {
      return NULL;
    }
    else if (size == 0)
    {
      if (CFReadStreamGetStatus(readStream) == kCFStreamStatusAtEnd)
        cont = false;
      else
        nglThread::MsSleep(10);
    }
    
    pos += size;
  }
  while (cont);
  
  CFHTTPMessageRef responseHeader = (CFHTTPMessageRef)CFReadStreamCopyProperty(readStream, kCFStreamPropertyHTTPResponseHeader);
  
  CFStringRef statusLine = CFHTTPMessageCopyResponseStatusLine(responseHeader);
  CFIndex strSize = CFStringGetLength(statusLine)+1;
  char* pStatus = new char[strSize];
  CFStringGetCString(statusLine, pStatus, strSize, kCFStringEncodingUTF8);
  nglString status(pStatus);
  
  UInt32 statusCode = CFHTTPMessageGetResponseStatusCode(responseHeader);
  
  nuiHTTPResponse* pResponse = new nuiHTTPResponse(statusCode, status);
  pResponse->SetBody(&buf[0], pos);
  
  delete[] pStatus;
  
  CFDictionaryRef dict = CFHTTPMessageCopyAllHeaderFields(responseHeader);
  CFIndex valueCount = CFDictionaryGetCount(dict);
  const CFStringRef* pNames = new CFStringRef[valueCount];
  const CFStringRef* pValues = new CFStringRef[valueCount];
  CFDictionaryGetKeysAndValues(dict, (const void**)pNames, (const void**)pValues);
  for (CFIndex i = 0; i< valueCount; i++)
  {
    strSize = CFStringGetLength(pNames[i])+1;
    char* pName = new char[strSize];
    CFStringGetCString(pNames[i], pName, strSize, kCFStringEncodingUTF8);
    
    strSize = CFStringGetLength(pValues[i])+1;
    char* pVal = new char[strSize];
    CFStringGetCString(pValues[i], pVal, strSize, kCFStringEncodingUTF8);
    
    nglString name(pName);
    nglString val(pVal);
    pResponse->AddHeader(name, val);
    
    delete[] pName;
    delete[] pVal;
  }
  delete[] pNames;
  delete[] pValues;
  
  CFRelease(responseHeader);
  CFRelease(url);
  CFRelease(method);
  CFRelease(req);
  CFRelease(readStream);
  CFRelease(dict);
  CFRelease(statusLine);
  if (body)
    CFRelease(body);
  
  return pResponse;
}
