/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_INSTATTRIBVALUE_H_
#define _MKTDATA_INSTATTRIBVALUE_H_

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

using namespace sbe;

namespace mktdata {

class InstAttribValue
{
private:
    char *m_buffer;
    std::uint64_t m_bufferLength;
    std::uint64_t m_offset;
    std::uint64_t m_actingVersion;

    inline void reset(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        if (SBE_BOUNDS_CHECK_EXPECT(((offset + 4) > bufferLength), false))
        {
            throw std::runtime_error("buffer too short for flyweight [E107]");
        }
        m_buffer = buffer;
        m_bufferLength = bufferLength;
        m_offset = offset;
        m_actingVersion = actingVersion;
    }

public:
    InstAttribValue(void) : m_buffer(nullptr), m_offset(0) {}

    InstAttribValue(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    InstAttribValue(const InstAttribValue& codec) :
        m_buffer(codec.m_buffer), m_offset(codec.m_offset), m_actingVersion(codec.m_actingVersion) {}

#if __cplusplus >= 201103L
    InstAttribValue(InstAttribValue&& codec) :
        m_buffer(codec.m_buffer), m_offset(codec.m_offset), m_actingVersion(codec.m_actingVersion) {}

    InstAttribValue& operator=(InstAttribValue&& codec)
    {
        m_buffer = codec.m_buffer;
        m_bufferLength = codec.m_bufferLength;
        m_offset = codec.m_offset;
        m_actingVersion = codec.m_actingVersion;
        return *this;
    }

#endif

    InstAttribValue& operator=(const InstAttribValue& codec)
    {
        m_buffer = codec.m_buffer;
        m_bufferLength = codec.m_bufferLength;
        m_offset = codec.m_offset;
        m_actingVersion = codec.m_actingVersion;
        return *this;
    }

    InstAttribValue &wrap(char *buffer, const std::uint64_t offset, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static const std::uint64_t encodedLength(void)
    {
        return 4;
    }


    InstAttribValue &clear(void)
    {
        *((std::uint32_t *)(m_buffer + m_offset)) = 0;
        return *this;
    }


    bool electronicMatchEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 0)) ? true : false;
    }

    InstAttribValue &electronicMatchEligible(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 0)) : (bits & ~(0x1L << 0));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool orderCrossEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 1)) ? true : false;
    }

    InstAttribValue &orderCrossEligible(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 1)) : (bits & ~(0x1L << 1));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool blockTradeEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 2)) ? true : false;
    }

    InstAttribValue &blockTradeEligible(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 2)) : (bits & ~(0x1L << 2));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool eFPEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 3)) ? true : false;
    }

    InstAttribValue &eFPEligible(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 3)) : (bits & ~(0x1L << 3));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool eBFEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 4)) ? true : false;
    }

    InstAttribValue &eBFEligible(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 4)) : (bits & ~(0x1L << 4));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool eFSEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 5)) ? true : false;
    }

    InstAttribValue &eFSEligible(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 5)) : (bits & ~(0x1L << 5));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool eFREligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 6)) ? true : false;
    }

    InstAttribValue &eFREligible(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 6)) : (bits & ~(0x1L << 6));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool oTCEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 7)) ? true : false;
    }

    InstAttribValue &oTCEligible(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 7)) : (bits & ~(0x1L << 7));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool iLinkIndicativeMassQuotingEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 8)) ? true : false;
    }

    InstAttribValue &iLinkIndicativeMassQuotingEligible(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 8)) : (bits & ~(0x1L << 8));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool negativeStrikeEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 9)) ? true : false;
    }

    InstAttribValue &negativeStrikeEligible(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 9)) : (bits & ~(0x1L << 9));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool negativePriceOutrightEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 10)) ? true : false;
    }

    InstAttribValue &negativePriceOutrightEligible(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 10)) : (bits & ~(0x1L << 10));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool isFractional(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 11)) ? true : false;
    }

    InstAttribValue &isFractional(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 11)) : (bits & ~(0x1L << 11));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool volatilityQuotedOption(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 12)) ? true : false;
    }

    InstAttribValue &volatilityQuotedOption(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 12)) : (bits & ~(0x1L << 12));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool rFQCrossEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 13)) ? true : false;
    }

    InstAttribValue &rFQCrossEligible(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 13)) : (bits & ~(0x1L << 13));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool zeroPriceOutrightEligible(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 14)) ? true : false;
    }

    InstAttribValue &zeroPriceOutrightEligible(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 14)) : (bits & ~(0x1L << 14));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool decayingProductEligibility(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 15)) ? true : false;
    }

    InstAttribValue &decayingProductEligibility(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 15)) : (bits & ~(0x1L << 15));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool variableProductEligibility(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 16)) ? true : false;
    }

    InstAttribValue &variableProductEligibility(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 16)) : (bits & ~(0x1L << 16));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool dailyProductEligibility(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 17)) ? true : false;
    }

    InstAttribValue &dailyProductEligibility(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 17)) : (bits & ~(0x1L << 17));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool gTOrdersEligibility(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 18)) ? true : false;
    }

    InstAttribValue &gTOrdersEligibility(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 18)) : (bits & ~(0x1L << 18));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }

    bool impliedMatchingEligibility(void) const
    {
        return (SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset))) & (0x1L << 19)) ? true : false;
    }

    InstAttribValue &impliedMatchingEligibility(const bool value)
    {
        std::uint32_t bits = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(m_buffer + m_offset)));
        bits = value ? (bits | (0x1L << 19)) : (bits & ~(0x1L << 19));
        *((std::uint32_t *)(m_buffer + m_offset)) = SBE_LITTLE_ENDIAN_ENCODE_32(bits);
        return *this;
    }
};
}
#endif
