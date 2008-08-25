/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglNativeVolume.h"

nglNativeVolume::nglNativeVolume(const nglString& rVolumeName, const nglPathVolume& rPathVolume)
: nglVolume(rVolumeName, rPathVolume.mPath, rPathVolume.mComment, rPathVolume.mFlags, rPathVolume.mType)
{
}

nglNativeVolume::nglNativeVolume(const nglString& rVolumeName, const nglPath& rSourcePath)
: nglVolume(rVolumeName, rSourcePath, nglString::Empty, nglPathVolume::Removable, nglPathVolume::eTypeUnknown)
{
}

nglNativeVolume::~nglNativeVolume()
{
}


bool nglNativeVolume::GetPathInfo(const nglPath& rPath, nglPathInfo& rInfo)
{
  nglPath p(mInfos.mPath);
  p += rPath.GetVolumeLessPath();
  
  return p.GetInfo(rInfo);
}

bool nglNativeVolume::MakeDirectory(const nglPath& rPath)
{
  nglPath p(mInfos.mPath);
  p += rPath.GetVolumeLessPath();
  
  return p.Create(false);
}

bool nglNativeVolume::Delete(const nglPath& rPathToDelete)
{
  nglPath p(mInfos.mPath);
  p += rPathToDelete.GetVolumeLessPath();
  
  return p.Delete(false);
}

bool nglNativeVolume::Move(const nglPath& rSource, const nglPath& rPathTarget)
{
  nglPath p(mInfos.mPath);
  p += rSource.GetVolumeLessPath();
  
  return p.Move(rPathTarget);
}

nglIStream* nglNativeVolume::OpenRead(const nglPath& rPath)
{
  nglPath p(mInfos.mPath);
  p += rPath.GetVolumeLessPath();
  
  return p.OpenRead();
}

nglIOStream* nglNativeVolume::OpenWrite(const nglPath& rPath, bool OverWrite)
{
  nglPath p(mInfos.mPath);
  p += rPath.GetVolumeLessPath();
  
  return p.OpenWrite(OverWrite);
}

bool nglNativeVolume::GetChildren(const nglPath& rPath, std::list<nglPath>* pChildren)
{
  nglPath p(mInfos.mPath);
  p += rPath.GetVolumeLessPath();
  
  return p.GetChildren(pChildren);
}

