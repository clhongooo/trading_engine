//**************************************************
//  Author:      Sunny Yan
//  Description: Tools for binary data manipulation
//**************************************************

#ifndef BINARYTOOLS_H_
#define BINARYTOOLS_H_

#include <boost/cstdint.hpp>
#include "ATU_Abstract_MDI.h"
#include "SDateTime.h"

typedef      uint8_t                               BYTE;
#define      Uint8                                 uint8_t
#define      Uint16                                uint16_t
#define      Uint32                                uint32_t
#define      Uint64                                uint64_t
#define      Int8                                  int8_t
#define      Int16                                 int16_t
#define      Int32                                 int32_t
#define      Int64                                 int64_t

#define      READ_UINT8(BYTE_PTR)                  (*((uint8_t*)(BYTE_PTR)))
#define      READ_UINT16(BYTE_PTR)                 (*((uint16_t*)(BYTE_PTR)))
#define      READ_UINT32(BYTE_PTR)                 (*((uint32_t*)(BYTE_PTR)))
#define      READ_UINT64(BYTE_PTR)                 (*((uint64_t*)(BYTE_PTR)))

#define      READ_INT8(BYTE_PTR)                   (*((int8_t*)(BYTE_PTR)))
#define      READ_INT16(BYTE_PTR)                  (*((int16_t*)(BYTE_PTR)))
#define      READ_INT32(BYTE_PTR)                  (*((int32_t*)(BYTE_PTR)))
#define      READ_INT64(BYTE_PTR)                  (*((int64_t*)(BYTE_PTR)))

#define      WRITE_UINT8(BYTE_PTR, val)            ((*((uint8_t*)(BYTE_PTR)))=(val))
#define      WRITE_UINT16(BYTE_PTR,val)            ((*((uint16_t*)(BYTE_PTR)))=(val))
#define      WRITE_UINT32(BYTE_PTR,val)            ((*((uint32_t*)(BYTE_PTR)))=(val))
#define      WRITE_UINT64(BYTE_PTR,val)            ((*((uint64_t*)(BYTE_PTR)))=(val))

#define      WRITE_INT8(BYTE_PTR, val)             ((*((int8_t*)(BYTE_PTR)))=(val))
#define      WRITE_INT16(BYTE_PTR,val)             ((*((int16_t*)(BYTE_PTR)))=(val))
#define      WRITE_INT32(BYTE_PTR,val)             ((*((int32_t*)(BYTE_PTR)))=(val))
#define      WRITE_INT64(BYTE_PTR,val)             ((*((int64_t*)(BYTE_PTR)))=(val))

#define      MAX_OMD_PACKET_SIZE                   1500
#define      MAX_MDP_PACKET_SIZE                   32768
#define      JSON_BUFFER_SIZE                      65536

class BinaryRecorder
{
  public:

    static const unsigned int BUFFER_SIZE = 1500;
    BinaryRecorder();
    ~BinaryRecorder();
    void SetOutFilePathAndOpen(const string &);
    void SetProgramStartTime(boost::posix_time::ptime);
    void WriteHKExSim(const char *);
    void WriteHKExRelTime(const char *);
    void WriteHKExUnixTime(const char *);

    //--------------------------------------------------
    // to allow late function binding
    //--------------------------------------------------
    typedef boost::function < void(const ATU_MDI_marketfeed_struct &) > WriteDataToFile_CallBackFunc;
    void SetIfWriteATUMDIStruct(const bool);
    inline void WriteATUMDIStruct(const ATU_MDI_marketfeed_struct & mfs) { (*m_WriteDataToFile_CallBackFunc)(mfs); }
    //--------------------------------------------------

  protected:
    boost::posix_time::ptime m_ProgramStartTime;
    string m_File;
    FILE * m_OutFile;
    char   m_TmpCharArray[BUFFER_SIZE];
    //--------------------------------------------------
    inline void _WriteATUMDIStruct(const ATU_MDI_marketfeed_struct &);
    inline void _DoNotWriteATUMDIStruct(const ATU_MDI_marketfeed_struct &) {}
    WriteDataToFile_CallBackFunc * m_WriteDataToFile_CallBackFunc;
};

#endif
