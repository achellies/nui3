

// Include
#include "nui.h"
#include "nglThread.h"
#include "nglThreadChecker.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	nglThread class.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class nglThreadPrivate 
{
  friend class	nglThread;
public:
  
  // Start
  static	inline	void	Start(nglThread * pThread) 
  {
    if(pThread)
    {
      // register the thread by the nglThreadChecker
      nglThreadChecker::RegisterThread(pThread->GetID(), pThread->GetName());  


      // Call main thread method (which often contains a loop inside)
      pThread->OnStart();

      // warn the nglThreadChecker that the thread stoped
      nglThreadChecker::UnregisterThread(pThread->GetID());

      
      // Now, the thread is closed
      pThread->mState = nglThread::Closed;
    }

    //
    // IMPORTANT:	no need to call 'ExitThread' which is "C" coding style.
    // in C++, you just have to return, that's all.
    // See MSDN for details.
    //ExitThread(0);
  }

  // Get
  inline nglThread::ID GetThreadID() const
  {
    return mThreadID;
  }

private:

  // Data
  HANDLE			mThread;
  DWORD			mThreadID;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Static function
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static DWORD WINAPI start_thread(void *arg) 
{
  nglThreadPrivate::Start( (nglThread*)arg );
  return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Constructor of nglThread class 
//
nglThread::nglThread(Priority priority)
{
  mpData = new nglThreadPrivate();
  
  mpData->mThread		= NULL;
  mpData->mThreadID	= 0;
  mState				= Stopped;
  mPriority			= priority;
}

nglThread::nglThread(const nglString& rName, Priority priority)
{
  mName = rName;
  mpData = new nglThreadPrivate();
  
  mpData->mThread		= NULL;
  mpData->mThreadID	= 0;
  mState				= Stopped;
  mPriority			= priority;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Destructor of nglThread class 
//
nglThread::~nglThread()
{
  CloseHandle(mpData->mThread);
  delete mpData;
  mpData = NULL;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Start
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Start a paused thread 
//
bool nglThread::Start()
{
  DWORD	res = 0;
  
  // Maybe release the previous thread instance ?
  if (mpData->mThread && mState==Closed)
  {
    CloseHandle(mpData->mThread);
    mpData->mThread	= NULL;
    mpData->mThreadID	= 0;
  }
  
  // If the thread is not created, do it
  if(mpData->mThread==NULL)
  {
    // Create the thread
    mpData->mThread = CreateThread(NULL, 0, start_thread, this, 0, & mpData->mThreadID);
    if(!mpData->mThread)
      return false;
    
    // Add it in global list
    nglAddThreadInGlobalList(this);
    
    // Set its priority
    SetPriority(mPriority);
  }
  
  // Status
  mState = Running;
  
  return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Join
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Wait until the thread is stopped 
//
bool nglThread::Join()
{
  if (mState == Closed)
    return true;
  
  // If the thread is not running, return false
  if (!mpData->mThread || mState == Stopped)
    return false;
  
  // We wait the end of the thread
  if (WAIT_FAILED == WaitForSingleObject(mpData->mThread, INFINITE))
    return false;
  
  // Status
  mState = Closed;
  
  // Remove thread from list
  nglDelThreadFromGlobalList(this);
  
  return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		OnStart
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	 
//
void nglThread::OnStart()
{
  // Do nothing by default
  mpData->mThread   = NULL;
  mpData->mThreadID	= NULL;
  ExitThread(0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		IsCurrent
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	 
//
bool nglThread::IsCurrent()
{
  return GetCurrentThread() == mpData->mThread;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		GetState
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	 
//
nglThread::State	nglThread::GetState()
{
  return mState;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		GetPriority
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	 
//
nglThread::Priority nglThread::GetPriority()
{
  return mPriority;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		SetPriority
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	 Set thread priority
//
bool nglThread::SetPriority(Priority priority)
{
  static const int priorityTable[] =  
  {
    THREAD_PRIORITY_LOWEST,
    THREAD_PRIORITY_BELOW_NORMAL,
    THREAD_PRIORITY_NORMAL,
    THREAD_PRIORITY_ABOVE_NORMAL,
    THREAD_PRIORITY_HIGHEST,
    THREAD_PRIORITY_TIME_CRITICAL
  };

  return SetThreadPriority(mpData->mThread, priorityTable[priority]) == TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		GetCurThread
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	 Return current active thread
//
nglThread* nglThread::GetCurThread()
{
  return nglGetThreadFromGlobalList( ::GetCurrentThreadId() );
}

nglThread::ID nglThread::GetCurThreadID()
{
  return ::GetCurrentThreadId();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Sleep
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	 Sleep (seconds)
//
void nglThread::Sleep(uint32 secs)
{
  ::Sleep(secs * 1000);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		MsSleep
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	 Sleep (milliseconds)
//
void nglThread::MsSleep(uint32 msecs)
{
  ::Sleep(msecs);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		USleep
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	 Sleep (microseconds)
//
void nglThread::USleep(uint32 usecs)
{
  ::Sleep(usecs/1000);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		GetID
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	 Return thread ID
//
nglThread::ID	nglThread::GetID()
{
  return mpData->GetThreadID();
}
