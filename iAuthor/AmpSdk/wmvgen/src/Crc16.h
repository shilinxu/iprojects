// Crc16.h: interface for the CCrc16 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __Crc16__
#define __Crc16__

class CCrc16
{
public:
	WORD m_crc16;

	CCrc16();
	CCrc16(const BYTE* src, int idx, int sz);
	CCrc16(const BYTE* src, int sz);
	~CCrc16();

	static WORD MakeCRC16(const BYTE* src, int idxStart, int Length);
	static bool CheckCRC16(const BYTE* src, int Length, WORD crc);

	static inline BYTE hi(WORD b)
	{
		return(b >> 8);
	}

	static inline BYTE lo(WORD b)
	{
		return(b & 0x00ff);
	}
};

#endif // __Crc16__
