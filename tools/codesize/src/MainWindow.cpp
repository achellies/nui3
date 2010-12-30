/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSS.h"
#include "nuiVBox.h"
#include "nuiTreeView.h"

#include "nuiScrollView.h"

class Node: public nuiTreeNode
{
public:
  Node(const nglString& rName)
  : nuiTreeNode(new nuiLabel(rName)), mName(rName), mSize(0)
  {
  }
  
  uint64 GetSize() const
  {
    return mSize;
  }
  
  void SetSize(uint64 s)
  {
    mSize = s;
    nuiLabel* pLabel = (nuiLabel*)GetElement();
    nglString txt;
    txt.Add(mSize).Add(_T(" - ")).Add(mName);
    pLabel->SetText(txt);
  }
  
private:
  nglString mName;
  uint64 mSize;
};

/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
: nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
  SetDebugMode(true);
  
#ifdef NUI_IPHONE
  LoadCSS(_T("rsrc:/css/style-iPhone.css"));
#else
  LoadCSS(_T("rsrc:/css/style.css"));
#endif

}

MainWindow::~MainWindow()
{
}

bool cmpnodes(const std::pair<nglString, Node*>& rLeft, const std::pair<nglString, Node*>& rRight)
{
  uint64 l = rLeft.second->GetSize();
  uint64 r = rRight.second->GetSize();
  return l < r;
}

void MainWindow::OnCreation()
{
  //Load(_T("/Users/meeloo/Desktop/nm.txt"));
  mpScrollView = new nuiScrollView();
  AddChild(mpScrollView);
}

nglDropEffect MainWindow::OnCanDrop(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y)
{
  if (!pDragObject->IsTypeSupported(_T("ngl/Files")) && !pDragObject->IsTypeSupported(_T("ngl/PromiseFiles")))
    return eDropEffectNone;
  
  nglDataFilesObject* pFiles = NULL;
  if (pDragObject->IsTypeSupported(_T("ngl/Files")))
    pFiles =  (nglDataFilesObject*)(pDragObject->GetType(_T("ngl/Files")));
  else if (pDragObject->IsTypeSupported(_T("ngl/PromiseFiles")))
    pFiles =  (nglDataFilesObject*)(pDragObject->GetType(_T("ngl/PromiseFiles")));
  
  nglPath path = *(pFiles->GetFiles().begin());
  
  return eDropEffectLink;
}


// virtual 
void MainWindow::OnDropped(nglDragAndDrop* pDragObject, nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  nglDataFilesObject* pFiles = NULL;
  if (pDragObject->IsTypeSupported(_T("ngl/Files")))
    pFiles =  (nglDataFilesObject*)(pDragObject->GetType(_T("ngl/Files")));
  else if (pDragObject->IsTypeSupported(_T("ngl/PromiseFiles")))
    pFiles =  (nglDataFilesObject*)(pDragObject->GetType(_T("ngl/PromiseFiles")));
  
  nglPath path = *(pFiles->GetFiles().begin());
  
  NGL_OUT(_T("dropped file : %ls\n"), path.GetChars());
  if (!path.Exists())
    return;

  // Try iPhone flat executable
  if (!path.IsLeaf())
  {
    nglPath p = path.GetRemovedExtension();
    nglString name(p.GetNodeName());
    p = path;
    p += name;
    
    if (!p.Exists() || !p.IsLeaf())
    {
      p = path;
      p += "Contents";
      p += "MacOS";
      p += name;
      
      if (!p.Exists() || !p.IsLeaf())
      {
        printf("no iOS or MacOS executable found...\n");
        return;
      }
      
      printf("MacOS executable found:\n%ls\n\n", p.GetChars());
    }
    else
    {
      printf("iOS executable found:\n%ls\n\n", p.GetChars());
    }

    path = p;
  }
  
  Load(path);
}

void MainWindow::OnDropLeave()
{
}

bool IsMethodName(const nglString& rSymbol, nglString& rClassName)
{
  int32 pos = rSymbol.Find(':');
  if (pos > 0 && rSymbol[pos+1] == ':')
  {
    rClassName = rSymbol.GetLeft(pos);
    return true;
  }
  return false;
}
                  
void MainWindow::Load(const nglPath& p)
{    
  mpScrollView->Clear();
  ///////////////////////////////

  nglString cmdline;
  //cmdline.Add("/usr/bin/nm -n -U -arch i386 ").Add(p.GetPathName()).Add(" | c++filt | c++filt -n");
  nglString ppp(p.GetPathName());
  ppp.Replace(_T(" "), _T("\\ ")); 
  cmdline.Add("/usr/bin/nm -n -U ").Add(ppp).Add(" | c++filt | c++filt -n");
  printf("Launching\n%ls\n", cmdline.GetChars());
  FILE * file = popen(cmdline.GetStdString().c_str(), "r");
  nglOMemory omem;
  uint32 res = 0;
  do
  {
    char buf[1025];
    memset(buf, 0, 1025);
    res = fread(buf, 1024, 1, file);
    //printf("%s", buf);
    omem.Write(buf, 1024, 1);
  } while (!feof(file));
  pclose(file);
  
  printf("redirection done\n");
  nglIStream* pStream = new nglIMemory(omem.GetBufferData(), omem.GetBufferSize());;
  
  nuiTreeNode* pTree = new nuiTreeNode(new nuiLabel(_T("Classes")));
  
  nglString line;
  
  uint64 lastaddr = 0;
  nglString lastsymbol;
  
  std::map<nglString, Node*> methods;
  std::map<nglString, Node*> classes;
  
  printf("read result\n");
  while (pStream->ReadLine(line))
  {
    // Read address
    int32 c = 0;
    while (line[c] && line[c] != ' ')
      c++;
    nglString a(line.Extract(0, c));
    uint64 address = a.GetCUInt(16);

    c++;
    // Read type char
    nglChar type = line[c];
    
    // Read Symbol if we are on a method / function decl
    if (type == 't')
    {
      c++;

      if (!lastsymbol.IsEmpty())
      {
        uint64 lastsize = address - lastaddr;
        
        std::map<nglString, Node*>::iterator it = methods.find(lastsymbol);
        
        bool skip = false;
        Node* pMethod = NULL;
        if (it != methods.end())
        {
          pMethod = it->second;
          pMethod->SetSize(pMethod->GetSize() + lastsize);
          //it->second += lastsize;
          skip = true;
        }
        else
        {
          //NGL_OUT(_T("new method \t %ls\n"), lastsymbol.GetChars());
          pMethod = new Node(lastsymbol);
          pMethod->SetSize(lastsize);
          methods[lastsymbol] = pMethod;
        }
        
        NGL_ASSERT(pMethod != NULL);
        
        if (!skip) // The method already exist so no need to add it to its class
        {
          nglString classname;
          if (IsMethodName(lastsymbol, classname))
          {
            //NGL_OUT(_T("new class %ls\n"), classname.GetChars());
            
            std::map<nglString, Node*>::iterator it = classes.find(classname);
            Node* pNode = NULL;
            if (it != classes.end())
            {
              pNode = it->second;
              pNode->SetSize(it->second->GetSize() + lastsize);
            }
            else
            {
              pNode = new Node(classname);
              pNode->SetSize(lastsize);
              classes[classname] = pNode;
            }
            
            pNode->AddChild(pMethod);
          }
          else
          {
            // Standalone function
            pTree->AddChild(pMethod);
          }

        }
      }
      
      lastaddr = address;
      lastsymbol = line.GetRight(line.GetLength() - c);
    }
  }
  printf("done\n");
  
  printf("build tree\n");
  delete pStream;
  
  std::list<std::pair<nglString, Node*> > sorted;
  {
    std::map<nglString, Node*>::const_iterator it = classes.begin();
    std::map<nglString, Node*>::const_iterator end = classes.end();
    
    while (it != end)
    {
      //NGL_OUT(_T("add unsorted %ls\n"), it->first.GetChars());
      sorted.push_back(std::pair<nglString, Node*>(it->first, it->second) );
      ++it;
    }
  }
  
  sorted.sort(cmpnodes);
  
  {
    std::list<std::pair<nglString, Node*> >::const_iterator it = sorted.begin();
    std::list<std::pair<nglString, Node*> >::const_iterator end = sorted.end();
    
    while (it != end)
    {
      //NGL_OUT(_T("%lld\t\t%ls\n"), it->second->GetSize(), it->first.GetChars());
      pTree->AddChild(it->second);
      ++it;
    }
  }
  
  pTree->Open(true);
  nuiTreeView* pTreeView = new nuiTreeView(pTree);
  mpScrollView->AddChild(pTreeView);
  printf("done\n");
}




void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}


bool MainWindow::LoadCSS(const nglPath& rPath)
{
  nglIStream* pF = rPath.OpenRead();
  if (!pF)
  {
    NGL_OUT(_T("Unable to open CSS source file '%ls'\n"), rPath.GetChars());
    return false;
  }
  
  nuiCSS* pCSS = new nuiCSS();
  bool res = pCSS->Load(*pF, rPath);
  delete pF;
  
  if (res)
  {
    nuiMainWindow::SetCSS(pCSS);
    return true;
  }
  
  NGL_OUT(_T("%ls\n"), pCSS->GetErrorString().GetChars());
  
  delete pCSS;
  return false;
}
