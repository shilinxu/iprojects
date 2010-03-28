//------------------------------------------------------------------------------
// File: WX_Util.h
//
// Desc: defines helper classes and functions for building multimedia filters.
//
//------------------------------------------------------------------------------


#ifndef __WX_UTIL__
#define __WX_UTIL__

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#define	_1KB_	(1024)
#define	_1MB_	(1024*_1KB_)

// eliminate spurious "statement has no effect" warnings.
#pragma warning(disable: 4705)

// eliminate spurious "needs to have dll-interface to be used by clients of class 'type2'" warnings.
#pragma warning(disable: 4251)


#ifdef _DEBUG
#define TTRACE            _Trace
#else // !DEBUG
#define TTRACE            1 ? (void)0 : _Trace
#endif

inline void _cdecl _Trace(LPCSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	char szBuffer[512];

	nBuf = _vsnprintf_s(szBuffer, sizeof(szBuffer), lpszFormat, args);
	assert(nBuf < sizeof(szBuffer)); //Output truncated as it was > sizeof(szBuffer)

	OutputDebugStringA(szBuffer);
	va_end(args);
}

// wrapper for whatever critical section we have
class WXCritSec {

    // make copy constructor and assignment operator inaccessible

    WXCritSec(const WXCritSec &refCritSec);
    WXCritSec &operator=(const WXCritSec &refCritSec);

    CRITICAL_SECTION m_CritSec;

private:
    bool    m_bLocked;        // Trace this one

public:
    WXCritSec() : m_bLocked(false) {
        InitializeCriticalSection(&m_CritSec);
    }

    ~WXCritSec() {
        DeleteCriticalSection(&m_CritSec);
    }

    bool IsLocked() {
        return m_bLocked;
    }

    void Lock() {
        EnterCriticalSection(&m_CritSec);
		m_bLocked = true;
    }

    void Unlock() {
        LeaveCriticalSection(&m_CritSec);
		m_bLocked = false;
    }
};

//
// To make deadlocks easier to track it is useful to insert in the
// code an assertion that says whether we own a critical section or
// not.  We make the routines that do the checking globals to avoid
// having different numbers of member functions in the debug and
// retail class implementations of CCritSec.  In addition we provide
// a routine that allows usage of specific critical sections to be
// traced.  This is NOT on by default - there are far too many.
//

#ifdef DEBUG
    BOOL WINAPI CritCheckIn(WXCritSec * pcCrit);
    BOOL WINAPI CritCheckIn(const WXCritSec * pcCrit);
    BOOL WINAPI CritCheckOut(WXCritSec * pcCrit);
    BOOL WINAPI CritCheckOut(const WXCritSec * pcCrit);
    void WINAPI DbgLockTrace(WXCritSec * pcCrit, BOOL fTrace);
#else
    #define CritCheckIn(x) TRUE
    #define CritCheckOut(x) TRUE
    #define DbgLockTrace(pc, fT)
#endif


// locks a critical section, and unlocks it automatically
// when the lock goes out of scope
class WXAutoLock {

    // make copy constructor and assignment operator inaccessible

    WXAutoLock(const WXAutoLock &refAutoLock);
    WXAutoLock &operator=(const WXAutoLock &refAutoLock);

protected:
    WXCritSec * m_pLock;

public:
    WXAutoLock(WXCritSec * plock)
    {
        m_pLock = plock;
        m_pLock->Lock();
    }

    ~WXAutoLock() {
        m_pLock->Unlock();
    }
};



// wrapper for event objects
class WXEvent
{

    // make copy constructor and assignment operator inaccessible

    WXEvent(const WXEvent &refEvent);
    WXEvent &operator=(const WXEvent &refEvent);

protected:
    HANDLE m_hEvent;
public:
    WXEvent(BOOL fManualReset = FALSE);
    ~WXEvent();

    // Cast to HANDLE - we don't support this as an lvalue
    operator HANDLE () const { return m_hEvent; };

    void Set() {SetEvent(m_hEvent);}
    BOOL Wait(DWORD dwTimeout = INFINITE) {
    return (WaitForSingleObject(m_hEvent, dwTimeout) == WAIT_OBJECT_0);
    }
    void Reset() { ResetEvent(m_hEvent); };
    BOOL Check() { return Wait(0); };
};

// WXQueue
//
// Implements a simple Queue ADT.  The queue contains a finite number of
// objects, access to which is controlled by a semaphore.  The semaphore
// is created with an initial count (N).  Each time an object is added
// a call to WaitForSingleObject is made on the semaphore's handle.  When
// this function returns a slot has been reserved in the queue for the new
// object.  If no slots are available the function blocks until one becomes
// available.  Each time an object is removed from the queue ReleaseSemaphore
// is called on the semaphore's handle, thus freeing a slot in the queue.
// If no objects are present in the queue the function blocks until an
// object has been added.

#define DEFAULT_WXQUEUESIZE   10

template <class T> class WXQueue {
private:
    HANDLE          hSemPut;        // Semaphore controlling queue "putting"
    HANDLE          hSemGet;        // Semaphore controlling queue "getting"
    CRITICAL_SECTION CritSect;      // Thread seriallization
    int             nMax;           // Max objects allowed in queue
    int             iNextPut;       // Array index of next "PutMsg"
    int             iNextGet;       // Array index of next "GetMsg"
    T              *QueueObjects;   // Array of objects (ptr's to void)

    void Initialize(int n) {
        iNextPut = iNextGet = 0;
        nMax = n;
        InitializeCriticalSection(&CritSect);
        hSemPut = CreateSemaphore(NULL, n, n, NULL);
        hSemGet = CreateSemaphore(NULL, 0, n, NULL);
        QueueObjects = new T[n];
    }


public:
    WXQueue(int n) {
        Initialize(n);
    }

    WXQueue() {
        Initialize(DEFAULT_QUEUESIZE);
    }

    ~WXQueue() {
        delete [] QueueObjects;
        DeleteCriticalSection(&CritSect);
        CloseHandle(hSemPut);
        CloseHandle(hSemGet);
    }

    T GetQueueObject() {
        int iSlot;
        T Object;
        LONG lPrevious;

        // Wait for someone to put something on our queue, returns straight
        // away is there is already an object on the queue.
        //
        WaitForSingleObject(hSemGet, INFINITE);

        EnterCriticalSection(&CritSect);
        iSlot = iNextGet++ % nMax;
        Object = QueueObjects[iSlot];
        LeaveCriticalSection(&CritSect);

        // Release anyone waiting to put an object onto our queue as there
        // is now space available in the queue.
        //
        ReleaseSemaphore(hSemPut, 1L, &lPrevious);
        return Object;
    }

    void PutQueueObject(T Object) {
        int iSlot;
        LONG lPrevious;

        // Wait for someone to get something from our queue, returns straight
        // away is there is already an empty slot on the queue.
        //
        WaitForSingleObject(hSemPut, INFINITE);

        EnterCriticalSection(&CritSect);
        iSlot = iNextPut++ % nMax;
        QueueObjects[iSlot] = Object;
        LeaveCriticalSection(&CritSect);

        // Release anyone waiting to remove an object from our queue as there
        // is now an object available to be removed.
        //
        ReleaseSemaphore(hSemGet, 1L, &lPrevious);
    }
};


class WXStreamBuffer
{

private:
	char* m_pHBuffer;
	char* m_pRBuffer;
	bool m_ogmin;

	ULONG m_BufferSize;
	LONGLONG m_FilePtr;
	ULONG m_ReadPtr;
	ULONG m_WritePtr;
	
	ULONG m_HeaderSize;
	ULONG m_HeaderPtr;

	LONGLONG m_FileLength;
	bool  m_bEOF;

	bool m_bDestroyed;
	bool m_bBuffering;

	WXCritSec m_csBuffer;	// Object we use for locking
	WXEvent   m_evRead;		//

private:
	bool GetEOF()			{ return m_bEOF; }

	void CheckEvent(ULONG len=0);

public:
	WXStreamBuffer();
	~WXStreamBuffer();

	bool IsEOF()	{ return m_bEOF; }
	void SetEOF(bool bEOF)	{ m_bEOF = bEOF; if (m_bEOF) m_evRead.Set();}
	bool HitTest(LONGLONG startPtr, ULONG len);

	bool Create(LONGLONG fileLength, ULONG headerSize=(64*_1KB_), ULONG buffSize=(4*_1MB_));
	   
	void Destroy();

	void Clear(LONGLONG position=0);
	
	ULONG GetHeaderSize();
	ULONG GetBufferSize();
	LONGLONG GetFilePointer();
	ULONG GetWrittenSize();
	ULONG GetFreeSize();
	bool Write(char* pBuff, LONGLONG startPtr, ULONG len);
	ULONG Read(char* pBuff, LONGLONG startPtr, ULONG len);

	int  GetBuffering()		{ return long(GetWrittenSize() / (m_BufferSize-(m_BufferSize*.2)) * 100.0); }
	bool IsBuffering()		{ return m_bBuffering; }

};   
#endif
