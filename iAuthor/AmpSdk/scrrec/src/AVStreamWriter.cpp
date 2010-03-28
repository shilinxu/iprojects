// AVStreamWriter.cpp: implementation of the AVStreamWriter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AVStreamWriter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


AVStreamWriter::AVStreamWriter() : m_bInit(false), m_hWriter(0)
{
}
AVStreamWriter::~AVStreamWriter()
{
	Destroy();
}

//////////////////////////////////////////////////////////////////////
// Methods

void AVStreamWriter::Destroy()
{
	Stop();

	if (m_bInit)
	{
		m_bInit = false;

		int sz = m_AudioQ.size();
		for (int i=0; i<sz; i++)
			delete m_AudioQ[i];

		sz = m_VideoQ.size();
		for (int i=0; i<sz; i++)
			delete m_VideoQ[i];

		m_AudioQ.clear();
		m_VideoQ.clear();

		m_AudioWQ.clear();
		m_VideoWQ.clear();
		m_WriterQ.clear();
	}
}

bool AVStreamWriter::WriteFile()
{
	HRESULT hr;
	bool bWritten = false;
	StreamQ	tmpQ;

	m_csWriter.Lock();
	tmpQ = m_WriterQ;
	m_WriterQ.clear();
	m_csWriter.Unlock();

	int sz = tmpQ.size();
	if (sz == 0) return true;
	for (int i=0; i<sz; i++)
	{
		if (tmpQ[i]->m_type == ST_AUDIO)
		{
			//write audio stream
			//
			hr = ::AVIStreamWrite(
					m_psAudio,
					tmpQ[i]->m_no,	//-1, // m_FrameTime, append at end of stream
					1, tmpQ[i]->m_pData, tmpQ[i]->m_size,
					AVIIF_KEYFRAME, NULL, NULL);
			if (hr == AVIERR_OK)
			{
				bWritten = true;
			}

			// free
			m_csBuffer.Lock();
			m_AudioWQ.push_back(tmpQ[i]);
			m_csBuffer.Unlock();
		}
		else
		{
			//write video stream
			//
			hr = ::AVIStreamWrite(
					m_psVideo,				// stream pointer
					tmpQ[i]->m_no,			// time of this frame
					1,						// number to write
					tmpQ[i]->m_pData,		// actual data of frame
					tmpQ[i]->m_size,		// size of this frame
					0,						// 0: Dependent n previous frame, not key frame, AVIIF_KEYFRAME
					NULL, NULL);
			if (hr == AVIERR_OK)
			{
				bWritten = true;
			}

			// free
			m_csBuffer.Lock();
			m_VideoWQ.push_back(tmpQ[i]);
			m_csBuffer.Unlock();
		}

	}

#ifdef _DEBUG
	if (bWritten == false)
	{
		bWritten = false;
	}
#endif

	return bWritten;
}

bool AVStreamWriter::Init(PAVISTREAM psAudio, int AudBuffSize,
						  PAVISTREAM psVideo, int VidBuffSize,
						  int fps)
{
	if (!m_bInit)
	{
		m_psAudio = psAudio;
		m_psVideo = psVideo;

		int qsize = (fps >= 2) ? fps/2 : 2;
		qsize = (qsize > 5) ? 5 : qsize;
		for (int i=0; i<qsize; i++)
		{
			AVBuffer* pABuff = new AVBuffer(AudBuffSize);
			AVBuffer* pVBuff = new AVBuffer(VidBuffSize);
			if (pABuff == NULL || pVBuff == NULL)
			{
				Destroy();
				return false;
			}

			pABuff->m_type = ST_AUDIO;
			pVBuff->m_type = ST_VIDEO;

			m_AudioQ.push_back(pABuff);
			m_VideoQ.push_back(pVBuff);
		}

		m_AudioWQ = m_AudioQ;
		m_VideoWQ = m_VideoQ;

		m_evQueued.Reset();

		m_bInit = true;
	}

	return m_bInit;
}