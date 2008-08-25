/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiBooleanAttributeEditor.h"
#include "nuiHBox.h"

 
nuiBooleanAttributeEditor::nuiBooleanAttributeEditor(const nuiAttrib<bool>& rAttribute)
	: nuiAttributeEditor(),
    mEventSink(this),
    mAttribute(rAttribute)
{
	SetObjectClass(_T("nuiBooleanAttributeEditor"));
	
	mpBtn = new nuiToggleButton();
	mpBtn->SetDisplayAsCheckBox(true);
	mpBtn->SetPressed(mAttribute.Get());
	mEventSink.Connect(mpBtn->ButtonPressed, &nuiBooleanAttributeEditor::OnButtonPressed);
	mEventSink.Connect(mpBtn->ButtonDePressed, &nuiBooleanAttributeEditor::OnButtonDePressed);
  mpBtn->SetEnabled(!rAttribute.IsReadOnly());
	AddChild(mpBtn);
	
	mSink.Connect(rAttribute.GetChangedSignal(), nuiFastDelegate::MakeDelegate(this, &nuiBooleanAttributeEditor::OnAttributeChanged));
}




nuiBooleanAttributeEditor::~nuiBooleanAttributeEditor()
{

}




void nuiBooleanAttributeEditor::OnAttributeChanged(bool value)
{
	mpBtn->ButtonPressed.Disable();
	mpBtn->ButtonDePressed.Disable();
	mpBtn->SetPressed(value);
	mpBtn->ButtonPressed.Enable();
	mpBtn->ButtonDePressed.Enable();
}


bool nuiBooleanAttributeEditor::OnButtonPressed(const nuiEvent& rEvent)
{
	mAttribute.Set(true);
	return true;
}

bool nuiBooleanAttributeEditor::OnButtonDePressed(const nuiEvent& rEvent)
{
	mAttribute.Set(false);
	return true;
}
