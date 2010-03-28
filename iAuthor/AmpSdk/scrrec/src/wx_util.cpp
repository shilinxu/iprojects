#include "stdafx.h"
#include "wx_util.h"
#include "stdio.h"

#if _DEBUG
FILE* log = NULL;
#endif

// --- CAMEvent -----------------------
WXEvent::WXEvent(BOOL fManualReset) {
    m_hEvent = CreateEvent(NULL, fManualReset, FALSE, NULL);
}

WXEvent::~WXEvent() {
    if(m_hEvent) {
        CloseHandle(m_hEvent);
    }
}


WXStreamBuffer::WXStreamBuffer()
{
	m_bEOF = false;
	m_bDestroyed = false;
	m_bBuffering = true;
	//m_ogmin = false;

	m_pHBuffer = NULL;
	m_FileLength = 0;
	m_HeaderSize = m_HeaderPtr = 0;

	m_pRBuffer = NULL;
	m_FilePtr = 0;
	m_BufferSize = m_ReadPtr = m_WritePtr = 0;

#ifdef _DEBUG
	fopen_s(&log, "C:\\sbufread.log", "w");
	fprintf(log, "== WXStreamBUffer Log Start\n");
	fflush(log);
#endif
}
   
WXStreamBuffer::~WXStreamBuffer()
{
	Destroy();
#ifdef _DEBUG
	fprintf(log, "========= Stop logging\n");
	fclose(log);
#endif
}

void WXStreamBuffer::CheckEvent(ULONG len)
{
	if (m_bEOF || m_bDestroyed)
	{
		m_evRead.Set();
		return;
	}
	// Buffering
	if (GetWrittenSize() < len)
	{
		m_bBuffering = true;
	}
	
	else 
	if (m_bBuffering && GetWrittenSize() > m_BufferSize*.2 )
	{
		m_bBuffering = false;
	}

	if (!m_bBuffering && GetWrittenSize() >= len)
		m_evRead.Set();
	else
		m_evRead.Reset();
/*
	// wait for incomming data
	///
	DWORD tm = 0;
	DWORD tick = 50;
	DWORD tout = 60*1000;
	while (!m_bEOF && GetWrittenSize() < m_BufferSize*.9)
	{
		if (m_bDestroyed)
		{
//			return 0;
		}

		Sleep(tick);
		tm += tick;

		if (tm > tout)
		{
#ifdef _DEBUG
			MessageBox(::GetForegroundWindow(), "StreamBuffer: Read time out", "err", MB_OK);
#endif
//			return 0;
		}
	}
	*/
}

bool WXStreamBuffer::HitTest(LONGLONG startPtr, ULONG len)
{
	return (m_FilePtr == startPtr && (startPtr + len) <= (m_FilePtr + GetWrittenSize()));
}

// create header area and ring buffer area
///
bool WXStreamBuffer::Create(LONGLONG fileLength, ULONG headerSize, ULONG buffSize)
{
	if (m_pHBuffer || m_pRBuffer)
		return false;

	if (headerSize > buffSize)
		buffSize = headerSize * 2;
	
	
	m_pHBuffer = (char *) malloc(headerSize);
	if (!m_pHBuffer)
		return false;

	buffSize = ULONG(double(buffSize) * 1.2);
	m_pRBuffer = (char *) malloc(buffSize);
	if (!m_pRBuffer)
		return false;
		
	m_HeaderSize = headerSize;
	m_BufferSize = buffSize;
	m_FilePtr = 0;
	m_HeaderPtr = m_ReadPtr = m_WritePtr = 0;

	m_bEOF = false;
	m_FileLength = fileLength;

	m_bDestroyed = false;
	m_bBuffering = true;
	
	m_evRead.Reset();

	return true;
}
   
// destroy header area and ring buffer area
///
void WXStreamBuffer::Destroy()
{ 
	m_bDestroyed = true;
	m_HeaderSize = m_HeaderPtr = 0;
	m_FilePtr = 0;
	m_BufferSize = m_ReadPtr = m_WritePtr = 0;

	if( m_pHBuffer )
	{
		free(m_pHBuffer);
		m_pHBuffer = NULL;
	}

	if( m_pRBuffer )
	{
		free(m_pRBuffer);
		m_pRBuffer = NULL;
	}
}

// clear only ring buffer area
///	
void WXStreamBuffer::Clear(LONGLONG position)
{
	//WXAutoLock lock(&m_csBuffer);

	m_evRead.Reset();

	m_bEOF = false;
	m_bBuffering = true;

	//m_HeaderPtr = 0;
	m_HeaderSize = 0;
	m_FilePtr = position;
	m_ReadPtr = m_WritePtr = 0;
}

ULONG WXStreamBuffer::GetHeaderSize()
{
	return m_HeaderSize;
}

ULONG WXStreamBuffer::GetBufferSize()
{
	return m_BufferSize;
}

LONGLONG WXStreamBuffer::GetFilePointer()
{
	return m_FilePtr;
}

// get amount of data (in bytes) available for reading in ring buffer.
///
ULONG WXStreamBuffer::GetWrittenSize()
{
	if( m_ReadPtr == m_WritePtr )
		return 0;

	if( m_ReadPtr < m_WritePtr )
		return m_WritePtr - m_ReadPtr;

	if( m_ReadPtr > m_WritePtr )
		return (m_BufferSize-m_ReadPtr)+m_WritePtr;

	return 0;
}

// get amount of space (in bytes) available for writing into ring buffer.
///
ULONG WXStreamBuffer::GetFreeSize()
{
	if( m_ReadPtr == m_WritePtr )
		return m_BufferSize;

	if( m_WritePtr < m_ReadPtr )
		return m_ReadPtr - m_WritePtr;

	if( m_WritePtr > m_ReadPtr )
		return (m_BufferSize-m_WritePtr)+m_ReadPtr;

	return 0;
}

// Writes binary data into the ring buffer.
// Parameters:
//     [in] pBuff - Pointer to the data to write.
//     [in] len - Size of the data to write (in bytes).
// Return Value: true upon success, otherwise false.
///
bool WXStreamBuffer::Write(char* pBuff, LONGLONG startPtr, ULONG len)
{
	bool result=false;

#ifdef _DEBUG
	fprintf(log, "WRITE:: ");
	fprintf(log, " len(%d) ",  len);
	fprintf(log, " WritePtr(%d)  \n", (int) m_WritePtr);
	fflush(log);
#endif

	if (!m_pHBuffer || !m_pRBuffer || m_bEOF)
	{
		SetEOF(true);
		return false;
	}

	// wait for free space
	///
	while (len >= GetFreeSize())
	{
#ifdef _DEBUG
	fprintf(log, "(len >= GetFreeSize()) strtPtr(%lld)", startPtr );
	fprintf(log, " len(%d) FreeSize(%d)  \n", (int) len, (int) GetFreeSize());
	fflush(log);
#endif
		CheckEvent();

		if (m_bDestroyed)
		{
			return false;
		}
		else
		{
			Sleep(50);
		}
	}

	if (len < GetFreeSize())	// ?? <=
	{
		// easy case, no wrapping
		if( m_WritePtr + len < m_BufferSize )
		{
			CopyMemory(&m_pRBuffer[m_WritePtr], pBuff, len);
			m_WritePtr += len;

			// write header
			if (m_HeaderSize > m_HeaderPtr)
			{
				ULONG freeHeaderSize = m_HeaderSize - m_HeaderPtr;
				ULONG chunkSize = (freeHeaderSize > len) ? len : freeHeaderSize;
				
				CopyMemory(&m_pHBuffer[m_HeaderPtr], pBuff, chunkSize);
				m_HeaderPtr += chunkSize;
			}

		}
		else // harder case, we need to wrap
		{
			ULONG firstChunkSize = m_BufferSize - m_WritePtr;
			ULONG secondChunkSize = len - firstChunkSize;

			CopyMemory(&m_pRBuffer[m_WritePtr], pBuff, firstChunkSize);
			CopyMemory(&m_pRBuffer[0], &pBuff[firstChunkSize], secondChunkSize);

			m_WritePtr = secondChunkSize;
		}
		
		result = true;
	}

	// EOF
	if (m_FilePtr+GetWrittenSize() >= m_FileLength) {
#ifdef _DEBUG
	fprintf(log, "LOG SETEOF???????????????\n" );
	fflush(log);
#endif
		SetEOF(true);
	}

	CheckEvent();

	return result;
}

// Reads (and extracts) data from the ring buffer.
// Parameters:
//     [in/out] pBuff - Pointer to where read data will be stored.
//     [in] startPtr - Pointer of file pointer.
//     [in] len - Size of the data to be read (in bytes).
// Return Value: true upon success, otherwise false.
///
ULONG WXStreamBuffer::Read(char* pBuff, LONGLONG startPtr, ULONG len)
{
	ULONG result=0;
	BOOL bWaitObject=false;

	// check file length
	if (m_FileLength < startPtr+len)
	{
#ifdef _DEBUG
	fprintf(log, "READ - 1 " );
#endif
		len = (ULONG) (m_FileLength - startPtr);
	}

	// read header?
	if (startPtr == 0) {
#ifdef _DEBUG
	fprintf(log, "2 " );
#endif
		m_ReadPtr = 0;
		m_FilePtr =0;
	}
	/*
	if ((startPtr+len) <= m_HeaderPtr)
	{
		CopyMemory(pBuff, &m_pHBuffer[(int) startPtr], len);
		m_FilePtr = startPtr + len;
#ifdef _DEBUG
	fprintf(log, "HeadRead: startPtr(%lld)", startPtr );
	fprintf(log, " len(%d) m_FilePtr(%d)\n", (int) len, (int)m_FilePtr);
	fflush(log);
#endif
		return len;
	}
	*/
	

	// wait until buffering or end of file
	if (!m_bEOF)
	{
		int nWaitSec;
		
		if (m_bBuffering) nWaitSec = 120;
		else nWaitSec = 60;

#ifdef _DEBUG
	fprintf(log, "3 " );
#endif
		for (int i=0; i<nWaitSec; i++)
		{
			CheckEvent(len);

			bWaitObject = m_evRead.Wait(1000);
			if (bWaitObject == TRUE || m_bEOF)
				break;
			else {
#ifdef _DEBUG
				fprintf(log, "OOOOOOOOOOOOOOPS::  bWaitObject=false \n");
				fflush(log);
#endif
			}
		}

		if (!bWaitObject)
		{
#ifdef _DEBUG
	fprintf(log, "4 " );
#endif
			SetEOF(true);
#ifdef _DEBUG
			MessageBox(::GetForegroundWindow(), "StreamBuffer: Read time out", "log", MB_OK);
#endif
		}
	}

	if (m_bDestroyed || !m_pHBuffer || !m_pRBuffer)
		return 0;

	if (!HitTest(startPtr, len))
	{
#ifdef _DEBUG
	fprintf(log, "****** Wooooooooooo    Read HitTest Failed startPtr(%lld)" );
	fprintf(log, " len(%d)", len );
	fflush(log);
#endif
		return 0;
	}

#ifdef _DEBUG
	fprintf(log, " \n");
	fflush(log);
#endif

	// easy case, no wrapping
	if ( m_ReadPtr+len <= m_BufferSize )
	{
#ifdef _DEBUG
	fprintf(log, "CopyRead: startPtr(%lld)", startPtr );
	fprintf(log, " len(%d) m_ReadPtr(%d)\n", (int) len, (int)m_ReadPtr);
	fflush(log);
#endif
		CopyMemory(pBuff, &m_pRBuffer[m_ReadPtr], len );
		m_ReadPtr += len;
	}
	else // harder case, buffer wraps
	{

		ULONG FirstChunkSize = m_BufferSize - m_ReadPtr;
		ULONG SecondChunkSize = len - FirstChunkSize;

		CopyMemory(pBuff, &m_pRBuffer[m_ReadPtr], FirstChunkSize);
		CopyMemory(&pBuff[FirstChunkSize], &m_pRBuffer[0], SecondChunkSize);

#ifdef _DEBUG
	fprintf(log, "RR: startPtr(%lld) len(%d) m_ReadPtr(%d) first(%d) second(%d)\n",
		startPtr, (int)len, (int)m_ReadPtr, (int)FirstChunkSize, (int)SecondChunkSize);
	fflush(log);
#endif
		m_ReadPtr = SecondChunkSize;
	}
	
	m_FilePtr = startPtr + len;

	return len;
}