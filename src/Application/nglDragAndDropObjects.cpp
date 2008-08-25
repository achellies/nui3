/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglDragAndDropObjects.h"
#include "nglKernel.h"

#include "nglDataObjects.h"

nglDragAndDrop::nglDragAndDrop(nglImage *pFeedbackImage, uint offsetX, uint offsetY)
{
  mpFeedbackImage = pFeedbackImage;
  mOffsetX = offsetX;
  mOffsetY = offsetY;
  mDesiredDropEffect = eDropEffectNone;
  mModifiers = 0;
}

nglDragAndDrop::~nglDragAndDrop()
{
  std::map<nglString,nglDataObject*>::iterator end = mObjects.end();
  std::map<nglString,nglDataObject*>::iterator it = mObjects.begin();

  while (it != end)
  {
    delete it->second;
    it++;
  }
}

void nglDragAndDrop::AddType(nglDataObject* pObject)
{
  NGL_ASSERT(pObject);
  mObjects[pObject->GetMimeType()] = pObject;
}

nglDataObject* nglDragAndDrop::GetType(const nglString& rMimeType)
{
  std::map<nglString, nglDataObject*>::iterator it = mObjects.find(rMimeType);
  if (it != mObjects.end() && it->second)
    return it->second;
  return NULL;
}

bool nglDragAndDrop::IsTypeSupported(const nglString& rMimeType)
{
  std::map<nglString,nglDataObject*>::iterator it = mObjects.find(rMimeType);
  if (it != mObjects.end())
    return true;
  return false;
}

std::list<nglDropEffect>& nglDragAndDrop::GetSupportedDropEffects()
{ 
  return mSupportedDropEffects; 
}
void nglDragAndDrop::AddSupportedDropEffect(nglDropEffect effect)
{ 
  mSupportedDropEffects.push_back(effect);
}

void nglDragAndDrop::SetDesiredDropEffect(nglDropEffect effect)
{ 
  mDesiredDropEffect = effect;
}

nglDropEffect nglDragAndDrop::GetDesiredDropEffect()
{ 
  return mDesiredDropEffect; 
}

std::map<nglString, nglDataObject*>& nglDragAndDrop::GetSupportedTypesMap()
{ 
  return mObjects;
}

const nglImage *nglDragAndDrop::GetFeedbackImage() const 
{
  return mpFeedbackImage;
}

uint nglDragAndDrop::GetFeedbackImageOffsetX() const
{
  return mOffsetX;
}

uint nglDragAndDrop::GetFeedbackImageOffsetY() const
{
  return mOffsetY;
}

int& nglDragAndDrop::GetModifiers()
{
  return mModifiers;
}
