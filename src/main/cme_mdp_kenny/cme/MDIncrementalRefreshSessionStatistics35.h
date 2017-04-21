/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_MDINCREMENTALREFRESHSESSIONSTATISTICS35_H_
#define _MKTDATA_MDINCREMENTALREFRESHSESSIONSTATISTICS35_H_

#if defined(SBE_HAVE_CMATH)
/* cmath needed for std::numeric_limits<double>::quiet_NaN() */
#  include <cmath>
#  define SBE_FLOAT_NAN std::numeric_limits<float>::quiet_NaN()
#  define SBE_DOUBLE_NAN std::numeric_limits<double>::quiet_NaN()
#else
/* math.h needed for NAN */
#  include <math.h>
#  define SBE_FLOAT_NAN NAN
#  define SBE_DOUBLE_NAN NAN
#endif

#if __cplusplus >= 201103L
#  include <cstdint>
#  include <functional>
#  include <string>
#  include <cstring>
#endif

#include <sbe/sbe.h>

#include "MDEntryTypeBook.h"
#include "OpenCloseSettlFlag.h"
#include "SecurityTradingStatus.h"
#include "LegSide.h"
#include "MatchEventIndicator.h"
#include "MaturityMonthYear.h"
#include "FLOAT.h"
#include "MDEntryTypeDailyStatistics.h"
#include "PRICE.h"
#include "PutOrCall.h"
#include "EventType.h"
#include "SecurityUpdateAction.h"
#include "SecurityTradingEvent.h"
#include "DecimalQty.h"
#include "MDEntryTypeStatistics.h"
#include "MDUpdateAction.h"
#include "InstAttribValue.h"
#include "AggressorSide.h"
#include "GroupSize8Byte.h"
#include "HaltReason.h"
#include "GroupSize.h"
#include "SettlPriceType.h"
#include "MDEntryType.h"
#include "PRICENULL.h"

using namespace sbe;

namespace mktdata {

class MDIncrementalRefreshSessionStatistics35
{
private:
    char *m_buffer;
    std::uint64_t m_bufferLength;
    std::uint64_t *m_positionPtr;
    std::uint64_t m_offset;
    std::uint64_t m_position;
    std::uint64_t m_actingBlockLength;
    std::uint64_t m_actingVersion;

    inline void reset(
        char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength,
        const std::uint64_t actingBlockLength, const std::uint64_t actingVersion)
    {
        m_buffer = buffer;
        m_offset = offset;
        m_bufferLength = bufferLength;
        m_actingBlockLength = actingBlockLength;
        m_actingVersion = actingVersion;
        m_positionPtr = &m_position;
        position(offset + m_actingBlockLength);
    }

public:

    MDIncrementalRefreshSessionStatistics35(void) : m_buffer(nullptr), m_bufferLength(0), m_offset(0) {}

    MDIncrementalRefreshSessionStatistics35(char *buffer, const std::uint64_t bufferLength)
    {
        reset(buffer, 0, bufferLength, sbeBlockLength(), sbeSchemaVersion());
    }

    MDIncrementalRefreshSessionStatistics35(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingBlockLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingBlockLength, actingVersion);
    }

    MDIncrementalRefreshSessionStatistics35(const MDIncrementalRefreshSessionStatistics35& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
    }

#if __cplusplus >= 201103L
    MDIncrementalRefreshSessionStatistics35(MDIncrementalRefreshSessionStatistics35&& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
    }

    MDIncrementalRefreshSessionStatistics35& operator=(MDIncrementalRefreshSessionStatistics35&& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
        return *this;
    }

#endif

    MDIncrementalRefreshSessionStatistics35& operator=(const MDIncrementalRefreshSessionStatistics35& codec)
    {
        reset(codec.m_buffer, codec.m_offset, codec.m_bufferLength, codec.m_actingBlockLength, codec.m_actingVersion);
        return *this;
    }

    static const std::uint16_t sbeBlockLength(void)
    {
        return (std::uint16_t)11;
    }

    static const std::uint16_t sbeTemplateId(void)
    {
        return (std::uint16_t)35;
    }

    static const std::uint16_t sbeSchemaId(void)
    {
        return (std::uint16_t)1;
    }

    static const std::uint16_t sbeSchemaVersion(void)
    {
        return (std::uint16_t)6;
    }

    static const char *sbeSemanticType(void)
    {
        return "X";
    }

    std::uint64_t offset(void) const
    {
        return m_offset;
    }

    MDIncrementalRefreshSessionStatistics35 &wrapForEncode(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
        return *this;
    }

    MDIncrementalRefreshSessionStatistics35 &wrapForDecode(
         char *buffer, const std::uint64_t offset, const std::uint64_t actingBlockLength,
         const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingBlockLength, actingVersion);
        return *this;
    }

    std::uint64_t position(void) const
    {
        return m_position;
    }

    void position(const std::uint64_t position)
    {
        if (SBE_BOUNDS_CHECK_EXPECT((position > m_bufferLength), false))
        {
            throw std::runtime_error("buffer too short [E100]");
        }
        m_position = position;
    }

    std::uint64_t encodedLength(void) const
    {
        return position() - m_offset;
    }

    char *buffer(void)
    {
        return m_buffer;
    }

    std::uint64_t actingVersion(void) const
    {
        return m_actingVersion;
    }

    static const std::uint16_t transactTimeId(void)
    {
        return 60;
    }

    static const std::uint64_t transactTimeSinceVersion(void)
    {
         return 0;
    }

    bool transactTimeInActingVersion(void)
    {
        return (m_actingVersion >= transactTimeSinceVersion()) ? true : false;
    }


    static const char *TransactTimeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "UTCTimestamp";
        }

        return "";
    }

    static const std::uint64_t transactTimeNullValue()
    {
        return SBE_NULLVALUE_UINT64;
    }

    static const std::uint64_t transactTimeMinValue()
    {
        return 0x0L;
    }

    static const std::uint64_t transactTimeMaxValue()
    {
        return 0xfffffffffffffffeL;
    }

    std::uint64_t transactTime(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((std::uint64_t *)(m_buffer + m_offset + 0)));
    }

    MDIncrementalRefreshSessionStatistics35 &transactTime(const std::uint64_t value)
    {
        *((std::uint64_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const std::uint16_t matchEventIndicatorId(void)
    {
        return 5799;
    }

    static const std::uint64_t matchEventIndicatorSinceVersion(void)
    {
         return 0;
    }

    bool matchEventIndicatorInActingVersion(void)
    {
        return (m_actingVersion >= matchEventIndicatorSinceVersion()) ? true : false;
    }


    static const char *MatchEventIndicatorMetaAttribute(const MetaAttribute::Attribute metaAttribute)
    {
        switch (metaAttribute)
        {
            case MetaAttribute::EPOCH: return "unix";
            case MetaAttribute::TIME_UNIT: return "nanosecond";
            case MetaAttribute::SEMANTIC_TYPE: return "MultipleCharValue";
        }

        return "";
    }

private:
    MatchEventIndicator m_matchEventIndicator;

public:

    MatchEventIndicator &matchEventIndicator()
    {
        m_matchEventIndicator.wrap(m_buffer, m_offset + 8, m_actingVersion, m_bufferLength);
        return m_matchEventIndicator;
    }

    class NoMDEntries
    {
    private:
        char *m_buffer;
        std::uint64_t m_bufferLength;
        std::uint64_t *m_positionPtr;
        std::uint64_t m_blockLength;
        std::uint64_t m_count;
        std::uint64_t m_index;
        std::uint64_t m_offset;
        std::uint64_t m_actingVersion;
        GroupSize m_dimensions;

    public:

        inline void wrapForDecode(char *buffer, std::uint64_t *pos, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
        {
            m_buffer = buffer;
            m_bufferLength = bufferLength;
            m_dimensions.wrap(m_buffer, *pos, actingVersion, bufferLength);
            m_blockLength = m_dimensions.blockLength();
            m_count = m_dimensions.numInGroup();
            m_index = -1;
            m_actingVersion = actingVersion;
            m_positionPtr = pos;
            *m_positionPtr = *m_positionPtr + 3;
        }

        inline void wrapForEncode(char *buffer, const std::uint8_t count, std::uint64_t *pos, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
        {
            m_buffer = buffer;
            m_bufferLength = bufferLength;
            m_dimensions.wrap(m_buffer, *pos, actingVersion, bufferLength);
            m_dimensions.blockLength((std::uint16_t)24);
            m_dimensions.numInGroup((std::uint8_t)count);
            m_index = -1;
            m_count = count;
            m_blockLength = 24;
            m_actingVersion = actingVersion;
            m_positionPtr = pos;
            *m_positionPtr = *m_positionPtr + 3;
        }

        static const std::uint64_t sbeHeaderSize()
        {
            return 3;
        }

        static const std::uint64_t sbeBlockLength()
        {
            return 24;
        }

        std::uint64_t position(void) const
        {
            return *m_positionPtr;
        }

        void position(const std::uint64_t position)
        {
            if (SBE_BOUNDS_CHECK_EXPECT((position > m_bufferLength), false))
            {
                 throw std::runtime_error("buffer too short [E100]");
            }
            *m_positionPtr = position;
        }

        inline std::uint64_t count(void) const
        {
            return m_count;
        }

        inline bool hasNext(void) const
        {
            return m_index + 1 < m_count;
        }

        inline NoMDEntries &next(void)
        {
            m_offset = *m_positionPtr;
            if (SBE_BOUNDS_CHECK_EXPECT(( (m_offset + m_blockLength) > m_bufferLength ), false))
            {
                throw std::runtime_error("buffer too short to support next group index [E108]");
            }
            *m_positionPtr = m_offset + m_blockLength;
            ++m_index;

            return *this;
        }

    #if __cplusplus < 201103L
        template<class Func> inline void forEach(Func& func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #else
        inline void forEach(std::function<void(NoMDEntries&)> func)
        {
            while(hasNext())
            {
                next(); func(*this);
            }
        }

    #endif


        static const std::uint16_t mDEntryPxId(void)
        {
            return 270;
        }

        static const std::uint64_t mDEntryPxSinceVersion(void)
        {
             return 0;
        }

        bool mDEntryPxInActingVersion(void)
        {
            return (m_actingVersion >= mDEntryPxSinceVersion()) ? true : false;
        }


        static const char *MDEntryPxMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "Price";
            }

            return "";
        }

private:
        PRICE m_mDEntryPx;

public:

        PRICE &mDEntryPx(void)
        {
            m_mDEntryPx.wrap(m_buffer, m_offset + 0, m_actingVersion, m_bufferLength);
            return m_mDEntryPx;
        }

        static const std::uint16_t securityIDId(void)
        {
            return 48;
        }

        static const std::uint64_t securityIDSinceVersion(void)
        {
             return 0;
        }

        bool securityIDInActingVersion(void)
        {
            return (m_actingVersion >= securityIDSinceVersion()) ? true : false;
        }


        static const char *SecurityIDMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        static const std::int32_t securityIDNullValue()
        {
            return SBE_NULLVALUE_INT32;
        }

        static const std::int32_t securityIDMinValue()
        {
            return -2147483647;
        }

        static const std::int32_t securityIDMaxValue()
        {
            return 2147483647;
        }

        std::int32_t securityID(void) const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::int32_t *)(m_buffer + m_offset + 8)));
        }

        NoMDEntries &securityID(const std::int32_t value)
        {
            *((std::int32_t *)(m_buffer + m_offset + 8)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static const std::uint16_t rptSeqId(void)
        {
            return 83;
        }

        static const std::uint64_t rptSeqSinceVersion(void)
        {
             return 0;
        }

        bool rptSeqInActingVersion(void)
        {
            return (m_actingVersion >= rptSeqSinceVersion()) ? true : false;
        }


        static const char *RptSeqMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        static const std::uint32_t rptSeqNullValue()
        {
            return SBE_NULLVALUE_UINT32;
        }

        static const std::uint32_t rptSeqMinValue()
        {
            return 0;
        }

        static const std::uint32_t rptSeqMaxValue()
        {
            return 4294967293;
        }

        std::uint32_t rptSeq(void) const
        {
            return SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset + 12)));
        }

        NoMDEntries &rptSeq(const std::uint32_t value)
        {
            *((std::uint32_t *)(m_buffer + m_offset + 12)) = SBE_LITTLE_ENDIAN_ENCODE_32(value);
            return *this;
        }

        static const std::uint16_t openCloseSettlFlagId(void)
        {
            return 286;
        }

        static const std::uint64_t openCloseSettlFlagSinceVersion(void)
        {
             return 0;
        }

        bool openCloseSettlFlagInActingVersion(void)
        {
            return (m_actingVersion >= openCloseSettlFlagSinceVersion()) ? true : false;
        }


        static const char *OpenCloseSettlFlagMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        OpenCloseSettlFlag::Value openCloseSettlFlag(void) const
        {
            return OpenCloseSettlFlag::get((*((std::uint8_t *)(m_buffer + m_offset + 16))));
        }

        NoMDEntries &openCloseSettlFlag(const OpenCloseSettlFlag::Value value)
        {
            *((std::uint8_t *)(m_buffer + m_offset + 16)) = (value);
            return *this;
        }

        static const std::uint16_t mDUpdateActionId(void)
        {
            return 279;
        }

        static const std::uint64_t mDUpdateActionSinceVersion(void)
        {
             return 0;
        }

        bool mDUpdateActionInActingVersion(void)
        {
            return (m_actingVersion >= mDUpdateActionSinceVersion()) ? true : false;
        }


        static const char *MDUpdateActionMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "int";
            }

            return "";
        }

        MDUpdateAction::Value mDUpdateAction(void) const
        {
            return MDUpdateAction::get((*((std::uint8_t *)(m_buffer + m_offset + 17))));
        }

        NoMDEntries &mDUpdateAction(const MDUpdateAction::Value value)
        {
            *((std::uint8_t *)(m_buffer + m_offset + 17)) = (value);
            return *this;
        }

        static const std::uint16_t mDEntryTypeId(void)
        {
            return 269;
        }

        static const std::uint64_t mDEntryTypeSinceVersion(void)
        {
             return 0;
        }

        bool mDEntryTypeInActingVersion(void)
        {
            return (m_actingVersion >= mDEntryTypeSinceVersion()) ? true : false;
        }


        static const char *MDEntryTypeMetaAttribute(const MetaAttribute::Attribute metaAttribute)
        {
            switch (metaAttribute)
            {
                case MetaAttribute::EPOCH: return "unix";
                case MetaAttribute::TIME_UNIT: return "nanosecond";
                case MetaAttribute::SEMANTIC_TYPE: return "char";
            }

            return "";
        }

        MDEntryTypeStatistics::Value mDEntryType(void) const
        {
            return MDEntryTypeStatistics::get((*((char *)(m_buffer + m_offset + 18))));
        }

        NoMDEntries &mDEntryType(const MDEntryTypeStatistics::Value value)
        {
            *((char *)(m_buffer + m_offset + 18)) = (value);
            return *this;
        }
    };

private:
    NoMDEntries m_noMDEntries;

public:

    static const std::uint16_t NoMDEntriesId(void)
    {
        return 268;
    }


    inline NoMDEntries &noMDEntries(void)
    {
        m_noMDEntries.wrapForDecode(m_buffer, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_noMDEntries;
    }

    NoMDEntries &noMDEntriesCount(const std::uint8_t count)
    {
        m_noMDEntries.wrapForEncode(m_buffer, count, m_positionPtr, m_actingVersion, m_bufferLength);
        return m_noMDEntries;
    }
};
}
#endif
