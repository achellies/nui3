/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiApplication.h"
#include "nuiSpiderMonkey.h"

nuiApplication::nuiApplication()
{
  mpScriptEngine = NULL;
}

nuiApplication::~nuiApplication()
{
  if (mpScriptEngine)
    mpScriptEngine->Release();
}

void nuiApplication::SetScriptEngine(nuiScriptEngine* pEngine)
{
  if (mpScriptEngine)
    mpScriptEngine->Release();
  mpScriptEngine = pEngine;
  if (mpScriptEngine)
    mpScriptEngine->Acquire();
}

nuiScriptEngine* nuiApplication::GetScriptEngine() const
{
  return mpScriptEngine;
}

void nuiApplication::EnableDefaultScriptEngine()
{
  //mpScriptEngine = new nuiSpiderMonkey();
}