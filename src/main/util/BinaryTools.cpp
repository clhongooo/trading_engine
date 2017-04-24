#include "BinaryTools.h"

BinaryRecorder::BinaryRecorder() :
  m_BinRecStartTime(boost::posix_time::microsec_clock::local_time()),
  m_File(""),
  m_OutFile(NULL),
  m_FlushOnEveryWrite(false)
{
  m_WriteDataToFile_CallBackFunc = new WriteDataToFile_CallBackFunc(boost::bind(&BinaryRecorder::_DoNotWriteATUMDIStruct,this,_1));
}

BinaryRecorder::~BinaryRecorder()
{
  if (m_OutFile)
  {
    fflush(m_OutFile);
    fclose(m_OutFile);
    delete m_OutFile;
    m_OutFile = NULL;
  }
  if (m_WriteDataToFile_CallBackFunc)
  {
    delete m_WriteDataToFile_CallBackFunc;
    m_WriteDataToFile_CallBackFunc = NULL;
  }
}

bool BinaryRecorder::SetOutFilePathAndOpen(const string & f, const string & mode)
{
  m_File = f;
  if (!m_OutFile) m_OutFile = fopen(m_File.c_str(), mode.c_str());

  if (m_OutFile) return true;
  else return false;
}

void BinaryRecorder::SetFlushOnEveryWrite(const bool b)
{
  m_FlushOnEveryWrite = b;
}

void BinaryRecorder::SetIfWriteATUMDIStruct(const bool b)
{
  if (m_WriteDataToFile_CallBackFunc)
  {
    delete m_WriteDataToFile_CallBackFunc;
    m_WriteDataToFile_CallBackFunc = NULL;
  }
  if (b) m_WriteDataToFile_CallBackFunc = new WriteDataToFile_CallBackFunc(boost::bind(&BinaryRecorder::_WriteATUMDIStruct,     this,_1));
  else   m_WriteDataToFile_CallBackFunc = new WriteDataToFile_CallBackFunc(boost::bind(&BinaryRecorder::_DoNotWriteATUMDIStruct,this,_1));
}

void BinaryRecorder::WriteHKExSim(const char * buffer)
{
  unsigned short iPktLen = *((uint16_t*)(&buffer[0]));
  fwrite((&buffer[0])+1,1,1,m_OutFile);
  fwrite(buffer,1,1,m_OutFile);
  fwrite(buffer,1,iPktLen,m_OutFile);
  if (m_FlushOnEveryWrite) fflush(m_OutFile);
}

void BinaryRecorder::WriteHKExRelTime(const char * buffer)
{
  //--------------------------------------------------
  // 8 bytes - Relative time stamp in millisec
  // n bytes - The actual packet
  //--------------------------------------------------
  uint16_t uiPktSize = *((uint16_t*)&buffer[0]);
  if (uiPktSize <= BUFFER_SIZE)
  {
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration diff = now - m_BinRecStartTime;
    uint64_t uiRelTime = diff.total_milliseconds();
    fwrite(&uiRelTime,8,1,m_OutFile);

    //--------------------------------------------------
    // The msg itself
    //--------------------------------------------------
    uint16_t uiPktSize = *((uint16_t*)&buffer[0]);
    fwrite(buffer,1,uiPktSize,m_OutFile);
  if (m_FlushOnEveryWrite) fflush(m_OutFile);
  }
}

void BinaryRecorder::WriteHKExUnixTime(const char * buffer)
{
  //--------------------------------------------------
  // 8 bytes - Unix time stamp in millisec
  // n bytes - The actual packet
  //--------------------------------------------------
  uint16_t uiPktSize = *((uint16_t*)&buffer[0]);
  if (uiPktSize <= BUFFER_SIZE)
  {
    unsigned long uiUnixTime = SDateTime::GetCurrentUnixTimeInMillisecGMT();
    fwrite(&uiUnixTime,8,1,m_OutFile);

    //--------------------------------------------------
    // The msg itself
    //--------------------------------------------------
    uint16_t uiPktSize = *((uint16_t*)&buffer[0]);
    fwrite(buffer,1,uiPktSize,m_OutFile);
  if (m_FlushOnEveryWrite) fflush(m_OutFile);
  }
}

void BinaryRecorder::_WriteATUMDIStruct(const ATU_MDI_marketfeed_struct & mfs)
{
  strcpy(m_TmpCharArray,ATU_MDI_marketfeed_struct::ToString(mfs).c_str());
  strcat(m_TmpCharArray,"\n");
  fwrite(m_TmpCharArray,sizeof(char),strlen(m_TmpCharArray),m_OutFile);
  if (m_FlushOnEveryWrite) fflush(m_OutFile);
}
