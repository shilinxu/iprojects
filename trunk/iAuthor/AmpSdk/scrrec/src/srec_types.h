#ifndef _SREC_TYPES_H_
#define _SREC_TYPES_H_

typedef __int8				sint8;		// -128 to 127
typedef __int16				sint16;		// -32,768 to 32,767
typedef __int32				sint32;		// -2,147,483,648 to 2,147,483,647
typedef __int64				sint64;		// -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
typedef int					sint;		// -2,147,483,648 to 2,147,483,647

typedef unsigned __int8		uint8;		// 255
typedef unsigned __int16	uint16;		// 65,535
typedef unsigned __int32	uint32;		// 4,294,967,295
typedef unsigned __int64	uint64;		// 18,446,744,073,709,551,616
typedef unsigned int		uint;		// 4,294,967,295

typedef CHAR	iCharA;					// generic 8bit char
typedef WCHAR	iCharW;					// 16bit Unicode char
typedef TCHAR	iCharT;					// depended on project defines


#endif //_SREC_TYPES_H_