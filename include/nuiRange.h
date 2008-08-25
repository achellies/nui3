/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiRange_h__
#define __nuiRange_h__

//#include "nui.h"
#include "nuiEvent.h"
#include "nuiRect.h"
#include "nuiEventRegistry.h"


#include <limits>

/// This class implements the behaviour of a range such as what is needed by a scroll bar for exemple. 
class NUI_API nuiRange
{
public:
  nuiRange(double Value=0, double Min=0, double Max=50, double Increment=1, double PageIncrement=10, double PageSize=10, double Origin = std::numeric_limits<double>::quiet_NaN());
  nuiRange(const nuiRange& rRange);
  virtual ~nuiRange();

  void SetValue(double Value);
  void SetRange(double Minimum,double Maximum);
  void SetIncrement(double Increment);
  void SetPageIncrement(double PageIncrement);
  void SetPageSize(double Size);
  void SetOrigin(double Origin);
  void SetValueAndSize(double Value, double Size);

  void Increment();
  void Decrement();
  void PageIncrement();
  void PageDecrement();
  bool MakeInRange(double Position, double Size = 0.0);

  double GetValue() const;
  double GetMinimum() const;
  double GetMaximum() const;
  double GetIncrement() const;
  double GetPageIncrement() const;
  double GetPageSize() const;
  double GetOrigin() const;
  double GetRange() const;
  double ConvertToUnit(double RangeValue) const; ///< Convert RangeValue from the Range Value to a value that is 0<= x <=1.

  nuiSimpleEventSource<nuiValueChanged> ValueChanged; ///< This event is sent whenever the value is changed.
  nuiSimpleEventSource<nuiChanged> Changed; ///< This event is sent whenever the range is changed (not the value).

  void EnableEvents(bool Set); ///< If set to false the range will not send nuiEvents when changed. 
  bool AreEventsEnabled(); ///< Returns true if the range sends events when changed.

  void SetDiscreetStepSize(bool DiscreetStepSize);

  nuiRange& operator=(const nuiRange& rRange);
  
  void ToString(nglString& str);
  bool FromString(const nglString& Value);
  
protected:
  double mMinimum;
  double mMaximum;
  double mPageSize;
  double mValue;
  double mIncrement;
  double mPageIncrement;
  double mOrigin;

  bool mDiscreetStepSize;

  bool mEvents;

};

#endif // __nuiRange_h__
