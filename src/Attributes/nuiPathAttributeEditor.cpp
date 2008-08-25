/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiPathAttributeEditor.h"

 
nuiPathAttributeEditor::nuiPathAttributeEditor(const nuiAttrib<nglPath>& rAttribute)
	: nuiAttributeEditor(),
    mAttribute(rAttribute)
{
	SetObjectClass(_T("nuiPathAttributeEditor"));
	
//	nglString contents;
//	mpAttribute->Format(contents);
//	
//	mpLabel = new nuiLabel(contents);
//	AddChild(mpLabel);
//
//	mSink.Connect(mpAttribute->AttributeChanged, nuiFastDelegate::MakeDelegate(this, &nuiPointAttributeEditor::OnAttributeChanged));
}


nuiPathAttributeEditor::~nuiPathAttributeEditor()
{

}


//void nuiPathAttributeEditor::OnAttributeChanged(nuiPoint point)
//{
//	nglString contents;
//	mpAttribute->Format(contents);
//	mpLabel->SetText(contents);
//}
