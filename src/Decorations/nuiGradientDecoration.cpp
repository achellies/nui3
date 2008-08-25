/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiGradientDecoration.h"

nuiGradientDecoration::nuiGradientDecoration(const nglString& rName)
: nuiDecoration(rName),
  mColor1(128, 128, 128),
  mColor2(0, 0, 0),
  mClientRect(0, 0, 0, 0),
  mStrokeSize(1),
  mStrokeColor(64, 64, 64),
  mShapeMode(eFillShape),
  mOrientation(nuiVertical)
{
  if (SetObjectClass(_T("nuiGradientDecoration")))
    InitAttributes();
  mGradientType = nuiGradient2Colors;
  mUserOffsets = false;
  InitOffsets();
}


nuiGradientDecoration::nuiGradientDecoration(const nglString& rName, const nuiRect& rClientRect, const nuiColor& rColor1, const nuiColor& rColor2, nuiOrientation orientation, uint strokeSize, const nuiColor& rStrokeColor, nuiShapeMode shapeMode)
: nuiDecoration(rName),
  mColor1(rColor1),
  mColor2(rColor2),
  mStrokeSize(strokeSize),
  mStrokeColor(rStrokeColor),
  mShapeMode(shapeMode),
  mOrientation(orientation)
{
  if (SetObjectClass(_T("nuiGradientDecoration")))
    InitAttributes();

  mClientRect = rClientRect;
  mGradientType = nuiGradient2Colors;
  mUserOffsets = false;
  InitOffsets();
}


nuiGradientDecoration::nuiGradientDecoration(const nglString& rName, const nuiRect& rClientRect, const nuiColor& rColor1, const nuiColor& rColor2, const nuiColor& rColor3, const nuiColor& rColor4, nuiOrientation orientation, uint strokeSize, const nuiColor& rStrokeColor, nuiShapeMode shapeMode)
: nuiDecoration(rName),
  mColor1(rColor1),
  mColor2(rColor2),
  mColor3(rColor3),
  mColor4(rColor4),
  mStrokeSize(strokeSize),
  mStrokeColor(rStrokeColor),
  mShapeMode(shapeMode),
  mOrientation(orientation)
{
  if (SetObjectClass(_T("nuiGradientDecoration")))
    InitAttributes();

  mClientRect = rClientRect;
  mGradientType = nuiGradient4Colors;
  mUserOffsets = false;
  InitOffsets();
}


nuiGradientDecoration::nuiGradientDecoration(const nglString& rName, const nuiColor& rColor1, const nuiColor& rColor2, uint strokeSize, const nuiColor& rStrokeColor, nuiShapeMode shapeMode, nuiOrientation orientation, const nuiRect& rClientRect)
: nuiDecoration(rName),
  mColor1(rColor1),
  mColor2(rColor2),
  mStrokeSize(strokeSize),
  mStrokeColor(rStrokeColor),
  mShapeMode(shapeMode),
  mOrientation(orientation)
{
  if (SetObjectClass(_T("nuiGradientDecoration")))
    InitAttributes();

  mClientRect = rClientRect;
  mGradientType = nuiGradient2Colors;
  mUserOffsets = false;
  InitOffsets();
}


nuiGradientDecoration::nuiGradientDecoration(const nglString& rName, const nuiColor& rColor1, const nuiColor& rColor2, const nuiColor& rColor3, const nuiColor& rColor4, uint strokeSize, const nuiColor& rStrokeColor, nuiShapeMode shapeMode, nuiOrientation orientation, const nuiRect& rClientRect)
: nuiDecoration(rName),
  mColor1(rColor1),
  mColor2(rColor2),
  mColor3(rColor3),
  mColor4(rColor4),
  mStrokeSize(strokeSize),
  mStrokeColor(rStrokeColor),
  mShapeMode(shapeMode),
  mOrientation(orientation)
{
  if (SetObjectClass(_T("nuiGradientDecoration")))
    InitAttributes();

  mClientRect = rClientRect;
  mGradientType = nuiGradient4Colors;
  mUserOffsets = false;
  InitOffsets();
}




nuiGradientDecoration::~nuiGradientDecoration()
{
}

void nuiGradientDecoration::InitAttributes()
{
  nuiAttribute<const nuiRect&>* AttributeRect = new nuiAttribute<const nuiRect&>
   (nglString(_T("ClientRect")), nuiUnitNone,
    nuiAttribute<const nuiRect&>::GetterDelegate(this, &nuiGradientDecoration::GetSourceClientRect),
    nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiGradientDecoration::SetSourceClientRect));
  AddAttribute(_T("ClientRect"), AttributeRect);

  nuiAttribute<nglString>* AttributeOrientation = new nuiAttribute<nglString>
  (nglString(_T("Orientation")), nuiUnitNone,
    nuiAttribute<nglString>::GetterDelegate(this, &nuiGradientDecoration::GetOrientation),
    nuiAttribute<nglString>::SetterDelegate(this, &nuiGradientDecoration::SetOrientation));
  AddAttribute(_T("Orientation"), AttributeOrientation);

  nuiAttribute<uint32>* StrokeSize= new nuiAttribute<uint32>
  (nglString(_T("StrokeSize")), nuiUnitPixels,
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::GetStrokeSize),
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::SetStrokeSize));
	AddAttribute(_T("StrokeSize"), StrokeSize);
  
  nuiAttribute<const nuiColor&>* Color1 = new nuiAttribute<const nuiColor&>
  (nglString(_T("Color1")), nuiUnitColor,
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::GetColor1), 
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::SetColor1));
	AddAttribute(_T("Color1"), Color1);
  
  nuiAttribute<const nuiColor&>* Color2 = new nuiAttribute<const nuiColor&>
  (nglString(_T("Color2")), nuiUnitColor,
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::GetColor2), 
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::SetColor2));
	AddAttribute(_T("Color2"), Color2);
  
  nuiAttribute<const nuiColor&>* Color3 = new nuiAttribute<const nuiColor&>
  (nglString(_T("Color3")), nuiUnitColor,
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::GetColor3), 
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::SetColor3));
	AddAttribute(_T("Color3"), Color3);
  
  nuiAttribute<const nuiColor&>* Color4 = new nuiAttribute<const nuiColor&>
  (nglString(_T("Color4")), nuiUnitColor,
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::GetColor4), 
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::SetColor4));
	AddAttribute(_T("Color4"), Color4);
  
  nuiAttribute<const nuiColor&>* StrokeColor = new nuiAttribute<const nuiColor&>
  (nglString(_T("StrokeColor")), nuiUnitColor,
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::GetStrokeColor), 
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::SetStrokeColor));
	AddAttribute(_T("StrokeColor"), StrokeColor);
  
  
  AddAttribute(new nuiAttribute<nuiShapeMode>
  (nglString(_T("ShapeMode")), nuiUnitNone,
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::GetShapeMode), 
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::SetShapeMode)));
  
  nuiAttribute<double>* Offset1 = new nuiAttribute<double>
  (nglString(_T("Offset1")), nuiUnitPercent,
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::GetOffset1), 
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::SetOffset1));
	AddAttribute(_T("Offset1"), Offset1);

  nuiAttribute<double>* Offset2 = new nuiAttribute<double>
  (nglString(_T("Offset2")), nuiUnitPercent,
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::GetOffset2), 
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::SetOffset2));
	AddAttribute(_T("Offset2"), Offset2);

  nuiAttribute<double>* Offset3 = new nuiAttribute<double>
  (nglString(_T("Offset3")), nuiUnitPercent,
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::GetOffset3), 
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::SetOffset3));
	AddAttribute(_T("Offset3"), Offset3);

  nuiAttribute<double>* Offset4 = new nuiAttribute<double>
  (nglString(_T("Offset4")), nuiUnitPercent,
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::GetOffset4), 
   nuiFastDelegate::MakeDelegate(this, &nuiGradientDecoration::SetOffset4));
	AddAttribute(_T("Offset4"), Offset4);
}



bool nuiGradientDecoration::Load(const nuiXMLNode* pNode)
{
  if (pNode->GetAttribute(_T("GradientType")).GetCInt())
    mGradientType = nuiGradient4Colors;
  else
    mGradientType = nuiGradient2Colors;
  mColor1.SetValue(nuiGetString(pNode, _T("Color1"), _T("white")));
  mColor2.SetValue(nuiGetString(pNode, _T("Color2"), _T("Black")));
  mColor3.SetValue(nuiGetString(pNode, _T("Color3"), _T("White")));
  mColor4.SetValue(nuiGetString(pNode, _T("Color4"), _T("Black")));
  mClientRect.SetValue(nuiGetString(pNode, _T("ClientRect")));
  return true;
}

nuiXMLNode* nuiGradientDecoration::Serialize(nuiXMLNode* pNode)
{
  pNode->SetName(_T("nuiGradientDecoration"));
  pNode->SetAttribute(_T("GradientType"), (int)mGradientType);
  pNode->SetAttribute(_T("Color1"), mColor1.GetValue());
  pNode->SetAttribute(_T("Color2"), mColor2.GetValue());
  pNode->SetAttribute(_T("Color3"), mColor3.GetValue());
  pNode->SetAttribute(_T("Color4"), mColor4.GetValue());
  
  pNode->SetAttribute(_T("ClientRect"), mClientRect.GetValue());

  return pNode;
}


void nuiGradientDecoration::InitOffsets()
{
  if (mGradientType == nuiGradient4Colors)
  {
    mOffset1 = 0;
    mOffset2 = 0.48f;
    mOffset3 = 0.52f;
    mOffset4 = 1;
  }
  else
  {
    mOffset1 = 0;
    mOffset2 = 1;
  }
}

//virtual
void nuiGradientDecoration::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect)
{
  pContext->EnableAntialiasing(false);
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableTexturing(false);

  pContext->SetFillColor(mColor1);
  pContext->SetStrokeColor(mStrokeColor);
  
  nuiGradient gradient;
  if (!mUserOffsets)
    InitOffsets();
  gradient.AddStop(mColor1, mOffset1);
  gradient.AddStop(mColor2, mOffset2);
  if (mGradientType == nuiGradient4Colors)
  {
    gradient.AddStop(mColor3, mOffset3);
    gradient.AddStop(mColor4, mOffset4);
  }

  if (mOrientation == nuiVertical)
    pContext->DrawGradient(gradient, rDestRect, 0, rDestRect.Top(), 0, rDestRect.Bottom());
  else
  {
    pContext->DrawGradient(gradient, rDestRect, 0, rDestRect.Top(), rDestRect.Right(), rDestRect.Top());
  }
  
  if (((mShapeMode == eStrokeShape) || (mShapeMode == eStrokeAndFillShape)) && mStrokeSize)
  {
    pContext->SetFillColor(mStrokeColor);
    
    nuiRect rect(rDestRect.Left(), rDestRect.Top(), rDestRect.GetWidth(), mStrokeSize);
    pContext->DrawRect(rect, eFillShape);
    
    rect = nuiRect(rDestRect.Left(), rDestRect.Bottom()-mStrokeSize, rDestRect.GetWidth(), mStrokeSize);
    pContext->DrawRect(rect, eFillShape);
    
    rect = nuiRect(rDestRect.Left(), rDestRect.Top(), mStrokeSize, rDestRect.GetHeight());
    pContext->DrawRect(rect, eFillShape);
    
    rect = nuiRect(rDestRect.Right()-mStrokeSize, rDestRect.Top(), mStrokeSize, rDestRect.GetHeight());
    pContext->DrawRect(rect, eFillShape);
    
  }

}


void nuiGradientDecoration::SetOrientation(nglString orientation)
{
  if (!orientation.Compare(_T("Vertical"), false))
    mOrientation = nuiVertical;
  else
    mOrientation = nuiHorizontal;
    
}

nglString nuiGradientDecoration::GetOrientation()
{
  if (mOrientation == nuiVertical)
    return nglString(_T("Vertical"));

  nglString(_T("Horizontal"));
}



nuiRect nuiGradientDecoration::GetIdealClientRect() const
{
  return nuiRect(0,0,0,0);
}



nuiSize nuiGradientDecoration::GetBorder(nuiPosition position) const
{
  switch (position)
  {
  case nuiLeft:
  case nuiRight:
    return mClientRect.Left();
    break;
  case nuiTop:
  case nuiBottom:
    return mClientRect.Top();
    break;
  case nuiFillHorizontal:
    return mClientRect.Left()*2;
    break;
  case nuiFillVertical:
    return mClientRect.Top()*2;
    break;
  case nuiNoPosition: break;
  case nuiTopLeft: break;
  case nuiTopRight: break;
  case nuiBottomLeft: break;
  case nuiBottomRight: break;
  case nuiCenter: break;
  case nuiTile: break;
  case nuiFill: break;
  case nuiFillLeft: break;
  case nuiFillRight: break;
  case nuiFillTop: break;
  case nuiFillBottom: break;
  }
  //we should'nt arrive here
  return NULL;
}



const nuiRect& nuiGradientDecoration::GetSourceClientRect() const
{
  return mClientRect;
}

void nuiGradientDecoration::SetSourceClientRect(const nuiRect& rRect)
{
  mClientRect = rRect;
}


void nuiGradientDecoration::GetColors(nuiColor& color1, nuiColor& color2) const
{
  color1 = mColor1;
  color2 = mColor2;
}

void nuiGradientDecoration::GetColors(nuiColor& color1, nuiColor& color2, nuiColor& color3, nuiColor& color4) const
{
  color1 = mColor1;
  color2 = mColor2;
  color3 = mColor3;
  color4 = mColor4;
}


void nuiGradientDecoration::SetColors(const nuiColor& color1, const nuiColor& color2)
{
  mColor1 = color1;
  mColor2 = color2;
}

void nuiGradientDecoration::SetColors(const nuiColor& color1, const nuiColor& color2, const nuiColor& color3, const nuiColor& color4)
{
  mColor1 = color1;
  mColor2 = color2;
  mColor3 = color3;
  mColor4 = color4;
}

const nuiColor& nuiGradientDecoration::GetColor1() const
{
  return mColor1;
}

void nuiGradientDecoration::SetColor1(const nuiColor& color)
{
  mColor1 = color;
}

const nuiColor& nuiGradientDecoration::GetColor2() const
{
  return mColor2;
}

void nuiGradientDecoration::SetColor2(const nuiColor& color)
{
  mColor2 = color;
}

const nuiColor& nuiGradientDecoration::GetColor3() const
{
  return mColor3;
}

void nuiGradientDecoration::SetColor3(const nuiColor& color)
{
  mColor3 = color;
  mGradientType = nuiGradient4Colors;
}

const nuiColor& nuiGradientDecoration::GetColor4() const
{
  return mColor4;
}

void nuiGradientDecoration::SetColor4(const nuiColor& color)
{
  mColor4 = color;
  mGradientType = nuiGradient4Colors;
}

const nuiColor& nuiGradientDecoration::GetStrokeColor() const
{
  return mStrokeColor;
}

double nuiGradientDecoration::GetOffset1() const
{
  return mOffset1;
}

double nuiGradientDecoration::GetOffset2() const
{
  return mOffset2;
}


double nuiGradientDecoration::GetOffset3() const
{
  return mOffset3;
}


double nuiGradientDecoration::GetOffset4() const
{
  return mOffset4;
}


void nuiGradientDecoration::SetOffset1(double offset)
{
  mOffset1 = offset;
  mUserOffsets = true;
}


void nuiGradientDecoration::SetOffset2(double offset)
{
  mOffset2 = offset;
  mUserOffsets = true;
}


void nuiGradientDecoration::SetOffset3(double offset)
{
  mOffset3 = offset;
  mUserOffsets = true;
}


void nuiGradientDecoration::SetOffset4(double offset)
{
  mOffset4 = offset;
  mUserOffsets = true;
}





void nuiGradientDecoration::SetStrokeColor(const nuiColor& color)
{
  mStrokeColor = color;
}

uint32 nuiGradientDecoration::GetStrokeSize() const
{
  return mStrokeSize;
}

void nuiGradientDecoration::SetStrokeSize(uint32 size)
{
  mStrokeSize = size;
}

void nuiGradientDecoration::SetShapeMode(nuiShapeMode mode)
{
  mShapeMode = mode;
}


nuiShapeMode nuiGradientDecoration::GetShapeMode() const
{
  return mShapeMode;
}



