// AVStreamWriter.h: interface for the AVStreamWriter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVSTREAMWRITER_H__72710046_D820_496E_AF3B_81174F4549D6__INCLUDED_)
#define AFX_AVSTREAMWRITER_H__72710046_D820_496E_AF3B_81174F4549D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vfw.h>
#include <tchar.h>
#include <assert.h>

#include <vector>

#include "wx_util.h"

#define SAFE_DELETE(p)			if(p) { delete p; p = NULL; }
#define SAFE_RELEASE(p)			if(p) { p->Release(); p = NULL; }
#define SAFE_RELEASE_GLOBAL(p)	if(p) { GlobalFreePtr(p); p = NULL; }

#define ST_AUDIO	1
#define ST_VIDEO	2
 
class AVStreamWriter;

class AVBuffer
{
public:
	friend AVStreamWriter;

	AVBuffer(int size, int no=-1) : m_type(-1)
	{
		m_no    = no;
		m_size  = size;
		m_pData = new char[size];
		//m_pData = ::GlobalAlloc(GMEM_FIXED, size);
	}

	int			m_type;
	DWORD		m_no;
	void*		m_pData;
	int			m_size;

private:
	~AVBuffer()
	{
		SAFE_DELETE(m_pData);
	}

};

typedef std::vector< AVBuffer* > StreamQ;

class AVStreamWriter
{
private:
	bool		m_bInit;
	StreamQ		m_AudioQ;
	StreamQ		m_VideoQ;

	StreamQ		m_AudioWQ;
	StreamQ		m_VideoWQ;
	StreamQ		m_WriterQ;

	WXCritSec	m_csBuffer;
	WXCritSec	m_csWriter;
	WXEvent		m_evQueued;

	PAVISTREAM	m_psAudio;
	PAVISTREAM	m_psVideo;

	HANDLE		m_hWriter;
	bool		m_bAliveWriterThread;

	bool WriteFile();
	
public:
	AVStreamWriter();
	~AVStreamWriter();

	bool Init(PAVISTREAM psAudio, int AudBuffSize,
			  PAVISTREAM psVideo, int VidBuffSize,
			  int fps=20);
	void Destroy();


	// return NULL or AVBuffer pointer of desired stream type
	//
	inline AVBuffer* LockBuffer(int StreamType=ST_AUDIO)
	{
		AVBuffer* pBuffer = NULL;

		if (StreamType == ST_AUDIO)
		{
			//int sz = m_AudioWQ.size();
			if (!m_AudioWQ.empty())
			{
				m_csBuffer.Lock();

				pBuffer = m_AudioWQ.back();
				m_AudioWQ.erase(m_AudioWQ.end() - 1);
				
				m_csBuffer.Unlock();
			}
		}
		else
		{
			int sz = m_VideoWQ.size();
			if (!m_VideoWQ.empty())
			{
				m_csBuffer.Lock();

				pBuffer = m_VideoWQ.back();
				m_VideoWQ.erase(m_VideoWQ.end() - 1);
				
				m_csBuffer.Unlock();
			}
		}

		return pBuffer;
	}

	// add new AVBuffer to queue of the AVI file-writer
	// with audio or screen's video data.
	//
	inline void UnlockBuffer(AVBuffer* pBuffer)
	{
		m_csWriter.Lock();
		m_WriterQ.push_back(pBuffer);
		if (m_WriterQ.size() > 0) m_evQueued.Set();
		m_csWriter.Unlock();
	}

	bool Run()
	{
		if (!m_bInit)
			return false;

		DWORD dwThreadID;
		m_hWriter = ::CreateThread(NULL, 0, WriterProc, this,
									CREATE_SUSPENDED, &dwThreadID);
		if (!m_hWriter)
		{
			return false;
		}

		m_bAliveWriterThread = true;
		::SetThreadPriority(m_hWriter, THREAD_PRIORITY_LOWEST);	//THREAD_PRIORITY_TIME_CRITICAL
		::ResumeThread(m_hWriter);
#ifdef _DEBUG
		TTRACE(_T("## The AVStreamWriter::WriterProc thread(0x%x) has started.\n"), dwThreadID);
#endif

		return true;
	}

	inline void Stop()
	{
		if (m_bAliveWriterThread)
		{
			m_bAliveWriterThread = false;

			m_csBuffer.Unlock();
			m_csWriter.Unlock();
			
			m_evQueued.Set();

			// sometimes infinite blocking, so wait 3 seconds
			WaitForSingleObject(m_hWriter, 1000*3);//INFINITE
		}
	}

	inline bool IsAliveWriter()
	{
		return m_bAliveWriterThread;
	}

	static DWORD WINAPI WriterProc(void* pParam)
	{
		AVStreamWriter* pWriter = (AVStreamWriter *) pParam;

		while (1)
		{
			// wait event?
			//
			pWriter->m_evQueued.Wait();
			if (!pWriter->m_bAliveWriterThread)
				break;

			if (!pWriter->WriteFile())
			{
				pWriter->m_bAliveWriterThread = false;
				break;
			}
		}

		return 0;
	}

};

#endif // !defined(AFX_AVSTREAMWRITER_H__72710046_D820_496E_AF3B_81174F4549D6__INCLUDED_)
