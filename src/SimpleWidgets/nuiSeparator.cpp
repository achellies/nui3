/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiSeparator.h"

nuiSeparator::nuiSeparator(nuiOrientation orientation)
: nuiWidget()
{
  SetObjectClass(_T("nuiSeparator"));
  mOrientation = orientation;
}

nuiSeparator::~nuiSeparator()
{
}


nuiRect nuiSeparator::CalcIdealSize()
{
  nuiContainer* pParent = GetParent();
  nuiRect rect(0,0,32, 32);
  if (pParent)
    rect = pParent->GetRect();
  nuiRect res;
  switch (mOrientation)
  {
  case nuiHorizontal:
    res.Set(0.0f, 0.0f, rect.GetWidth(), 2.0f);
    break;
  case nuiVertical:
    res.Set(0.0f, 0.0f, 2.0f, rect.GetHeight());
    break;
  }
  
  return res;
}


bool nuiSeparator::Draw(nuiDrawContext *pContext)
{
  nuiRect r(GetRect());

  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableBlending(true);

  pContext->SetStrokeColor(nuiColor(255, 255, 255, 120));
  pContext->DrawLine(r.Left(), 0, r.Right(), 0);

  pContext->SetStrokeColor(nuiColor(0, 0, 0, 32));
  pContext->DrawLine(r.Left(), 1, r.Right(), 1);
  
  return true;
}


