#include "BinaryTools.h"

BinaryRecorder::BinaryRecorder() :
  m_BinRecStartTime(boost::posix_time::microsec_clock::local_time()),
  m_File(""),
  m_FileMode(""),
  m_FlushOnEveryWrite(false)
{
  m_BinaryWriter.reset(new BinaryRecorder::BinaryWriterNotDo(m_BinRecStartTime, m_File, m_FileMode, m_FlushOnEveryWrite));
}

BinaryRecorder::BinaryWriter::BinaryWriter(const boost::posix_time::ptime pt, const string & f, const string & mode, const bool w) :
  m_BinRecStartTime(pt),
  m_File(f),
  m_FileMode(mode),
  m_FlushOnEveryWrite(w),
  m_OutFile(NULL)
{
  m_File = f;
  if (!m_OutFile) m_OutFile = fopen(m_File.c_str(), mode.c_str());
}

BinaryRecorder::BinaryWriterDo::BinaryWriterDo(const boost::posix_time::ptime pt, const string & f, const string & mode, const bool w) :
  BinaryRecorder::BinaryWriter(pt, f, mode, w) {}
  BinaryRecorder::BinaryWriterNotDo::BinaryWriterNotDo(const boost::posix_time::ptime pt, const string & f, const string & mode, const bool w) :
    BinaryRecorder::BinaryWriter(pt, f, mode, w) {}

bool BinaryRecorder::IsOutFileReady() const
{
  return m_OutFile;
}

BinaryRecorder::BinaryWriter::~BinaryWriter()
{
  if (m_OutFile)
  {
    fflush(m_OutFile);
    fclose(m_OutFile);
    delete m_OutFile;
    m_OutFile = NULL;
  }
}
void BinaryRecorder::SetOutFilePath(const string & f, const string & mode)
{
  m_File = f;
  m_FileMode = mode;
}

void BinaryRecorder::SetFlushOnEveryWrite(const bool b)
{
  m_FlushOnEveryWrite = b;
}

bool BinaryRecorder::EnableWriter()
{
  m_BinaryWriter.reset(new BinaryRecorder::BinaryWriterDo(m_BinRecStartTime, m_File, m_FileMode, m_FlushOnEveryWrite));
  return IsOutFileReady();
}

void BinaryRecorder::BinaryWriterDo::WriteHKExSim(const char * buffer)
{
  unsigned short iPktLen = *((uint16_t*)(&buffer[0]));
  fwrite((&buffer[0])+1,1,1,m_OutFile);
  fwrite(buffer,1,1,m_OutFile);
  fwrite(buffer,1,iPktLen,m_OutFile);
  if (m_FlushOnEveryWrite) fflush(m_OutFile);
}

void BinaryRecorder::BinaryWriterDo::WriteHKExRelTime(const char * buffer)
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

void BinaryRecorder::BinaryWriterDo::WriteHKExUnixTime(const char * buffer)
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

void BinaryRecorder::BinaryWriterDo::WriteATUMDIStruct(const ATU_MDI_marketfeed_struct & mfs)
{
  strcpy(m_TmpCharArray,ATU_MDI_marketfeed_struct::ToString(mfs).c_str());
  strcat(m_TmpCharArray,"\n");
  fwrite(m_TmpCharArray,sizeof(char),strlen(m_TmpCharArray),m_OutFile);
  if (m_FlushOnEveryWrite) fflush(m_OutFile);
}

void BinaryRecorder::WriteHKExSim(const char * buffer)
{
  m_BinaryWriter->WriteHKExSim(buffer);
}

void BinaryRecorder::WriteHKExRelTime(const char * buffer)
{
  m_BinaryWriter->WriteHKExRelTime(buffer);
}

void BinaryRecorder::WriteHKExUnixTime(const char * buffer)
{
  m_BinaryWriter->WriteHKExUnixTime(buffer);
}

void BinaryRecorder::WriteATUMDIStruct(const ATU_MDI_marketfeed_struct & mfs)
{
  m_BinaryWriter->WriteATUMDIStruct(mfs);
}

