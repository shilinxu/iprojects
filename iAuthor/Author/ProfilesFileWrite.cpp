// ProfilesFileWrite.cpp: implementation of the CProfilesFileWrite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "author.h"
#include "ProfilesFileWrite.h"
#include <ATLCONV.H>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProfilesFileWrite::CProfilesFileWrite()
{

}

CProfilesFileWrite::~CProfilesFileWrite()
{

}

bool CProfilesFileWrite::WritePDAProfile(CString strPath)
{	
	CFile File;	
	if(File.Open(strPath, CFile::modeCreate|CFile::modeWrite) == 0)
		return false;
	
	WORD wMagic = 0xFEFF;
	File.Write((char *) &wMagic, 2);
	
	CString strTemp;
	strTemp = " <profile version=\"589824\" \r\n storageformat=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " name=\"CD 음질 오디오(CBR) / 포켓 PC 표준 비디오(CBR)\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " description=\"\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73647561-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp =" streamnumber=\"1\" \r\n streamname=\"Audio Stream\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp =" inputname=\"Audio412\" \r\n bitrate=\"16000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp =" bufferwindow=\"-1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp =" reliabletransport=\"0\" \r\n decodercomplexity=\"\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\" >\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{00000161-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bfixedsizesamples=\"1\" \r\n btemporalcompression=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " lsamplesize=\"640\">\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <waveformatex wFormatTag=\"353\" \r\n nChannels=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nSamplesPerSec=\"16000\" \r\n nAvgBytesPerSec=\"2000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nBlockAlign=\"640\" \r\n wBitsPerSample=\"16\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " codecdata=\"002200002E0080070000\"/> \r\n </wmmediatype> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </streamconfig> \r\n <streamconfig majortype=\"{73646976-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamnumber=\"2\" \r\n streamname=\"Video Stream\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " inputname=\"Video412\" \r\n bitrate=\"186000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n reliabletransport=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " decodercomplexity=\"AU\" \r\n rfc1766langid=\"ko\" \r\n > \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videomediaprops maxkeyframespacing=\"20000000\" quality=\"0\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{32564D57-0000-0010-8000-00AA00389B71}\" \r\n bfixedsizesamples=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"1\" \r\n lsamplesize=\"0\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videoinfoheader dwbitrate=\"186000\" \r\n dwbiterrorrate=\"0\" \r\n avgtimeperframe=\"666666\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <rcsource left=\"0\" \r\n top=\"0\" \r\n right=\"208\" \r\n bottom=\"160\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <rctarget left=\"0\" \r\n top=\"0\" \r\n right=\"208\" \r\n bottom=\"160\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <bitmapinfoheader biwidth=\"208\" \r\n biheight=\"160\" \r\n biplanes=\"1\" \r\n bibitcount=\"24\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bicompression=\"WMV2\" \r\n bisizeimage=\"0\" \r\n bixpelspermeter=\"0\" \r\n biypelspermeter=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biclrused=\"0\" \r\n biclrimportant=\"0\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </videoinfoheader> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </wmmediatype>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </streamconfig>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </profile>\r\n";
	WriteUnicode(&File, strTemp);

	File.Close();		

	return true;
}


bool CProfilesFileWrite::WriteVBRProfile(CString strPath, int nWidth, int nHeight)
{
	CFile File;	
	if(File.Open(strPath, CFile::modeCreate|CFile::modeWrite) == 0)
		return false;

	WORD wMagic = 0xFEFF;
	File.Write((char *) &wMagic, 2);

	CString strTemp;
	strTemp = " <profile version=\"589824\" \r\n storageformat=\"1\" \r\n";	
	WriteUnicode(&File, strTemp);
	strTemp = " name=\"화면 비디오/오디오 높음(CBR)\" \r\n description=\"\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73647561-0000-0010-8000-00AA00389B71}\" \r\n streamnumber=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Audio Stream\" \r\n inputname=\"Audio412\" \r\n bitrate=\"128016\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n reliabletransport=\"0\" \r\n decodercomplexity=\"\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\" \r\n vbrenabled=\"1\" \r\n vbrquality=\"10\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bitratemax=\"0\" \r\n bufferwindowmax=\"0\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{00000161-0000-0010-8000-00AA00389B71}\" \r\n bfixedsizesamples=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"0\" \r\n lsamplesize=\"2230\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <waveformatex wFormatTag=\"353\" \r\n nChannels=\"2\" \r\n nSamplesPerSec=\"44100\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nAvgBytesPerSec=\"2147483402\" \r\n nBlockAlign=\"2230\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " wBitsPerSample=\"16\" \r\n codecdata=\"008800001F00D9220000\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </wmmediatype> \r\n </streamconfig> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73646976-0000-0010-8000-00AA00389B71}\" \r\n streamnumber=\"2\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Video Stream\" \r\n inputname=\"Video412\" \r\n bitrate=\"370000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n reliabletransport=\"0\" \r\n decodercomplexity=\"AU\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\" \r\n vbrenabled=\"1\" \r\n vbrquality=\"100\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bitratemax=\"0\" \r\n bufferwindowmax=\"0\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videomediaprops maxkeyframespacing=\"20000000\" \r\n quality=\"100\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{3253534D-0000-0010-8000-00AA00389B71}\" \r\n bfixedsizesamples=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"1\" \r\n lsamplesize=\"0\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videoinfoheader dwbitrate=\"370000\" \r\n dwbiterrorrate=\"0\" \r\n avgtimeperframe=\"666666\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp.Format(" <rcsource left=\"0\" top=\"0\" right=\"%d\" bottom=\"%d\"/> \r\n", nWidth, nHeight);	
	WriteUnicode(&File, strTemp);
	strTemp.Format(" <rctarget left=\"0\" top=\"0\" right=\"%d\" bottom=\"%d\"/> \r\n", nWidth, nHeight);
	WriteUnicode(&File, strTemp);
	strTemp.Format(" <bitmapinfoheader biwidth=\"%d\" biheight=\"%d\" biplanes=\"1\" \r\n", nWidth, nHeight);
	WriteUnicode(&File, strTemp);
	strTemp = " bibitcount=\"24\" \r\n bicompression=\"MSS2\" \r\n bisizeimage=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bixpelspermeter=\"0\" \r\n biypelspermeter=\"0\" \r\n biclrused=\"0\" \r\n biclrimportant=\"0\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </videoinfoheader> \r\n </wmmediatype> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <dataunitextension extensionsystemid=\"{1B1EE554-F9EA-4BC8-821A-376B74E4C4B8}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " extensiondatasize=\"2\" \r\n extensionsysteminfo=\"\"/> \r\n </streamconfig> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamprioritization>\r\n <stream number=\"1\" mandatory=\"0\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <stream number=\"2\" mandatory=\"0\"/> \r\n </streamprioritization> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </profile> \r\n";	
	WriteUnicode(&File, strTemp);

	File.Close();		

	return true;
}


bool CProfilesFileWrite::WriteCBRProfile(CString strPath, int nIndex)
{
	bool	bReturn = false;

	switch(nIndex)
	{
	case	0:
		bReturn = Write100kStreaming(strPath);
		break;
	case	1:
		bReturn = Write200kStreaming(strPath);
		break;		
	case	2:
		bReturn = Write300kStreaming(strPath);
		break;
	case	3:
		bReturn = Write400kStreaming(strPath);
		break;
	case	4:
		bReturn = Write500kStreaming(strPath);
		break;
	case	5:
		bReturn = Write600kStreaming(strPath);
		break;	
	}

	return bReturn;
}


bool CProfilesFileWrite::Write100kStreaming(CString strPath)
{
	CFile File;	
	if(File.Open(strPath, CFile::modeCreate|CFile::modeWrite) == 0)
		return false;

	WORD wMagic = 0xFEFF;
	File.Write((char *) &wMagic, 2);

	CString strTemp;
	strTemp = " <profile version=\"589824\" \r\n storageformat=\"1\" \r\n";	
	WriteUnicode(&File, strTemp);
	strTemp = " name=\"다중 비트 전송률 오디오(CBR) / 다중 비트 전송률 비디오(CBR)\" \r\n description=\"\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73647561-0000-0010-8000-00AA00389B71}\" \r\n streamnumber=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Audio Stream\" \r\n inputname=\"Audio412\" \r\n bitrate=\"8000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n reliabletransport=\"0\" \r\n decodercomplexity=\"\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{00000161-0000-0010-8000-00AA00389B71}\" \r\n bfixedsizesamples=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"0\" \r\n lsamplesize=\"320\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <waveformatex wFormatTag=\"353\" \r\n nChannels=\"1\" \r\n nSamplesPerSec=\"8000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nAvgBytesPerSec=\"1000\" \r\n nBlockAlign=\"320\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " wBitsPerSample=\"16\" \r\n codecdata=\"00220000260080070000\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </wmmediatype> \r\n </streamconfig> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73646976-0000-0010-8000-00AA00389B71}\" \r\n streamnumber=\"2\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Video Stream\" \r\n inputname=\"Video412\" \r\n bitrate=\"86000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n reliabletransport=\"0\" \r\n decodercomplexity=\"AU\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videomediaprops maxkeyframespacing=\"30000000\" \r\n quality=\"55\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{33564D57-0000-0010-8000-00AA00389B71}\" \r\n bfixedsizesamples=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"1\" \r\n lsamplesize=\"0\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videoinfoheader dwbitrate=\"86000\" \r\n dwbiterrorrate=\"0\" \r\n avgtimeperframe=\"666666\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp.Format(" <rcsource left=\"0\" top=\"0\" right=\"%d\" bottom=\"%d\"/> \r\n", 240, 180);	
	WriteUnicode(&File, strTemp);
	strTemp.Format(" <rctarget left=\"0\" top=\"0\" right=\"%d\" bottom=\"%d\"/> \r\n", 240, 180);
	WriteUnicode(&File, strTemp);
	strTemp.Format(" <bitmapinfoheader biwidth=\"%d\" biheight=\"%d\" biplanes=\"1\" \r\n", 240, 180);
	WriteUnicode(&File, strTemp);
	strTemp = " bibitcount=\"24\" \r\n bicompression=\"WMV3\" \r\n bisizeimage=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bixpelspermeter=\"0\" \r\n biypelspermeter=\"0\" \r\n biclrused=\"0\" \r\n biclrimportant=\"0\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </videoinfoheader> \r\n </wmmediatype> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </streamconfig> \r\n </profile> \r\n";	
	WriteUnicode(&File, strTemp);

	File.Close();		

	return true;
}


bool CProfilesFileWrite::Write200kStreaming(CString strPath)
{
	CFile File;	
	if(File.Open(strPath, CFile::modeCreate|CFile::modeWrite) == 0)
		return false;

	WORD wMagic = 0xFEFF;
	File.Write((char *) &wMagic, 2);

	CString strTemp;
	strTemp = " <profile version=\"589824\" \r\n storageformat=\"1\" \r\n";	
	WriteUnicode(&File, strTemp);
	strTemp = " name=\"다중 비트 전송률 오디오(CBR) / 다중 비트 전송률 비디오(CBR)\" \r\n description=\"\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73647561-0000-0010-8000-00AA00389B71}\" \r\n streamnumber=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Audio Stream\" \r\n inputname=\"Audio412\" \r\n bitrate=\"16000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n reliabletransport=\"0\" \r\n decodercomplexity=\"\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{00000161-0000-0010-8000-00AA00389B71}\" \r\n bfixedsizesamples=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"0\" \r\n lsamplesize=\"640\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <waveformatex wFormatTag=\"353\" \r\n nChannels=\"1\" \r\n nSamplesPerSec=\"16000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nAvgBytesPerSec=\"2000\" \r\n nBlockAlign=\"640\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " wBitsPerSample=\"16\" \r\n codecdata=\"002200002E0080070000\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </wmmediatype> \r\n </streamconfig> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73646976-0000-0010-8000-00AA00389B71}\" \r\n streamnumber=\"2\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Video Stream\" \r\n inputname=\"Video412\" \r\n bitrate=\"180000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n reliabletransport=\"0\" \r\n decodercomplexity=\"AU\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videomediaprops maxkeyframespacing=\"30000000\" \r\n quality=\"60\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{33564D57-0000-0010-8000-00AA00389B71}\" \r\n bfixedsizesamples=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"1\" \r\n lsamplesize=\"0\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videoinfoheader dwbitrate=\"180000\" \r\n dwbiterrorrate=\"0\" \r\n avgtimeperframe=\"666666\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp.Format(" <rcsource left=\"0\" top=\"0\" right=\"%d\" bottom=\"%d\"/> \r\n", 320, 240);	
	WriteUnicode(&File, strTemp);
	strTemp.Format(" <rctarget left=\"0\" top=\"0\" right=\"%d\" bottom=\"%d\"/> \r\n", 320, 240);
	WriteUnicode(&File, strTemp);
	strTemp.Format(" <bitmapinfoheader biwidth=\"%d\" biheight=\"%d\" biplanes=\"1\" \r\n", 320, 240);
	WriteUnicode(&File, strTemp);
	strTemp = " bibitcount=\"24\" \r\n bicompression=\"WMV3\" \r\n bisizeimage=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bixpelspermeter=\"0\" \r\n biypelspermeter=\"0\" \r\n biclrused=\"0\" \r\n biclrimportant=\"0\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </videoinfoheader> \r\n </wmmediatype> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </streamconfig> \r\n </profile> \r\n";	
	WriteUnicode(&File, strTemp);

	File.Close();		

	return true;
}



bool CProfilesFileWrite::Write300kStreaming(CString strPath)
{
	CFile File;	
	if(File.Open(strPath, CFile::modeCreate|CFile::modeWrite) == 0)
		return false;

	WORD wMagic = 0xFEFF;
	File.Write((char *) &wMagic, 2);

	CString strTemp;
	strTemp = " <profile version=\"589824\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " storageformat=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " name=\"다중 비트 전송률 오디오(CBR) / 다중 비트 전송률 비디오(CBR)\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " description=\"\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73647561-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamnumber=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Audio Stream\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " inputname=\"Audio412\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bitrate=\"16000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " reliabletransport=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " decodercomplexity=\"\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " > \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{00000161-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bfixedsizesamples=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " lsamplesize=\"640\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <waveformatex wFormatTag=\"353\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nChannels=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nSamplesPerSec=\"16000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nAvgBytesPerSec=\"2000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nBlockAlign=\"640\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " wBitsPerSample=\"16\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " codecdata=\"002200002E0080070000\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </wmmediatype> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </streamconfig> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73646976-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamnumber=\"2\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Video Stream\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " inputname=\"Video412\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bitrate=\"280000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " reliabletransport=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " decodercomplexity=\"AU\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " > \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videomediaprops maxkeyframespacing=\"30000000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " quality=\"60\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{33564D57-0000-0010-8000-00AA00389B71}\"  \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bfixedsizesamples=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " lsamplesize=\"0\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videoinfoheader dwbitrate=\"280000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " dwbiterrorrate=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " avgtimeperframe=\"666666\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <rcsource left=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " top=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " right=\"640\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bottom=\"480\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <rctarget left=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " top=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " right=\"640\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bottom=\"480\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <bitmapinfoheader biwidth=\"640\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biheight=\"480\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biplanes=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bibitcount=\"24\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bicompression=\"WMV3\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bisizeimage=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bixpelspermeter=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biypelspermeter=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biclrused=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biclrimportant=\"0\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </videoinfoheader>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </wmmediatype>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </streamconfig>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </profile> \r\n";
	WriteUnicode(&File, strTemp);

	File.Close();		

	return true;
}



bool CProfilesFileWrite::Write400kStreaming(CString strPath)
{	
	CFile File;	
	if(File.Open(strPath, CFile::modeCreate|CFile::modeWrite) == 0)
		return false;

	WORD wMagic = 0xFEFF;
	File.Write((char *) &wMagic, 2);

	CString strTemp;
	strTemp = " <profile version=\"589824\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " storageformat=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " name=\"다중 비트 전송률 오디오(CBR) / 다중 비트 전송률 비디오(CBR)\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " description=\"\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73647561-0000-0010-8000-00AA00389B71}\"  \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamnumber=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Audio Stream\"  \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " inputname=\"Audio412\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bitrate=\"16000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " reliabletransport=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " decodercomplexity=\"\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " > \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{00000161-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bfixedsizesamples=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " lsamplesize=\"640\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <waveformatex wFormatTag=\"353\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nChannels=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nSamplesPerSec=\"16000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nAvgBytesPerSec=\"2000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nBlockAlign=\"640\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " wBitsPerSample=\"16\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " codecdata=\"002200002E0080070000\"/>  \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </wmmediatype> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </streamconfig> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73646976-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamnumber=\"2\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Video Stream\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " inputname=\"Video412\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bitrate=\"380000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " reliabletransport=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " decodercomplexity=\"AU\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " > \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videomediaprops maxkeyframespacing=\"30000000\"  \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " quality=\"70\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{33564D57-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bfixedsizesamples=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " lsamplesize=\"0\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videoinfoheader dwbitrate=\"380000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " dwbiterrorrate=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " avgtimeperframe=\"666666\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <rcsource left=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " top=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " right=\"800\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bottom=\"600\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <rctarget left=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " top=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " right=\"800\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bottom=\"600\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <bitmapinfoheader biwidth=\"800\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biheight=\"600\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biplanes=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bibitcount=\"24\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bicompression=\"WMV3\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bisizeimage=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bixpelspermeter=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biypelspermeter=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biclrused=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biclrimportant=\"0\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </videoinfoheader>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </wmmediatype>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </streamconfig>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </profile> \r\n";
	WriteUnicode(&File, strTemp);

	File.Close();	   
 
	return true;
}



bool CProfilesFileWrite::Write500kStreaming(CString strPath)
{
	CFile File;	
	if(File.Open(strPath, CFile::modeCreate|CFile::modeWrite) == 0)
		return false;

	WORD wMagic = 0xFEFF;
	File.Write((char *) &wMagic, 2);

	CString strTemp;
	strTemp = " <profile version=\"589824\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " storageformat=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " name=\"다중 비트 전송률 오디오(CBR) / 다중 비트 전송률 비디오(CBR)\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " description=\"\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73647561-0000-0010-8000-00AA00389B71}\"  \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamnumber=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Audio Stream\"  \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " inputname=\"Audio412\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bitrate=\"16000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " reliabletransport=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " decodercomplexity=\"\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " > \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{00000161-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bfixedsizesamples=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " lsamplesize=\"640\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <waveformatex wFormatTag=\"353\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nChannels=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nSamplesPerSec=\"16000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nAvgBytesPerSec=\"2000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nBlockAlign=\"640\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " wBitsPerSample=\"16\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " codecdata=\"002200002E0080070000\"/>  \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </wmmediatype> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </streamconfig> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73646976-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamnumber=\"2\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Video Stream\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " inputname=\"Video412\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bitrate=\"491000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " reliabletransport=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " decodercomplexity=\"AU\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " > \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videomediaprops maxkeyframespacing=\"30000000\"  \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " quality=\"75\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{33564D57-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bfixedsizesamples=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " lsamplesize=\"0\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videoinfoheader dwbitrate=\"491000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " dwbiterrorrate=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " avgtimeperframe=\"666666\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <rcsource left=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " top=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " right=\"1024\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bottom=\"768\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <rctarget left=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " top=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " right=\"1024\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bottom=\"768\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <bitmapinfoheader biwidth=\"1024\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biheight=\"768\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biplanes=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bibitcount=\"24\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bicompression=\"WMV3\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bisizeimage=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bixpelspermeter=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biypelspermeter=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biclrused=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biclrimportant=\"0\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </videoinfoheader>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </wmmediatype>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </streamconfig>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </profile> \r\n";
	WriteUnicode(&File, strTemp);

	File.Close();	   

	return true;
}



bool CProfilesFileWrite::Write600kStreaming(CString strPath)
{
	CFile File;	
	if(File.Open(strPath, CFile::modeCreate|CFile::modeWrite) == 0)
		return false;

	WORD wMagic = 0xFEFF;
	File.Write((char *) &wMagic, 2);

	CString strTemp;
	strTemp = " <profile version=\"589824\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " storageformat=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " name=\"다중 비트 전송률 오디오(CBR) / 다중 비트 전송률 비디오(CBR)\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " description=\"\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73647561-0000-0010-8000-00AA00389B71}\"  \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamnumber=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Audio Stream\"  \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " inputname=\"Audio412\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bitrate=\"16000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " reliabletransport=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " decodercomplexity=\"\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " > \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{00000161-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bfixedsizesamples=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " lsamplesize=\"640\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <waveformatex wFormatTag=\"353\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nChannels=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nSamplesPerSec=\"16000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nAvgBytesPerSec=\"2000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " nBlockAlign=\"640\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " wBitsPerSample=\"16\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " codecdata=\"002200002E0080070000\"/>  \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </wmmediatype> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </streamconfig> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <streamconfig majortype=\"{73646976-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamnumber=\"2\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " streamname=\"Video Stream\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " inputname=\"Video412\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bitrate=\"580000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bufferwindow=\"-1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " reliabletransport=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " decodercomplexity=\"AU\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " rfc1766langid=\"ko\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " > \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videomediaprops maxkeyframespacing=\"30000000\"  \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " quality=\"90\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <wmmediatype subtype=\"{33564D57-0000-0010-8000-00AA00389B71}\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bfixedsizesamples=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " btemporalcompression=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " lsamplesize=\"0\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <videoinfoheader dwbitrate=\"580000\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " dwbiterrorrate=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " avgtimeperframe=\"666666\"> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <rcsource left=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " top=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " right=\"1280\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bottom=\"1024\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <rctarget left=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " top=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " right=\"1280\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bottom=\"1024\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " <bitmapinfoheader biwidth=\"1280\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biheight=\"1024\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biplanes=\"1\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bibitcount=\"24\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bicompression=\"WMV3\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bisizeimage=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " bixpelspermeter=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biypelspermeter=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biclrused=\"0\" \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " biclrimportant=\"0\"/> \r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </videoinfoheader>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </wmmediatype>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </streamconfig>\r\n";
	WriteUnicode(&File, strTemp);
	strTemp = " </profile> \r\n";
	WriteUnicode(&File, strTemp);

	File.Close();

	return true;
}



void CProfilesFileWrite::WriteUnicode(CFile* pFile, CString strTemp)
{
	USES_CONVERSION;
	
	WCHAR * pwszLine = A2W(strTemp);
	int nBytelen = wcslen(pwszLine) * 2;
	pFile->Write(pwszLine, nBytelen);	
}
