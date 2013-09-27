// base32.cpp - written and placed in the public domain by Frank Palazzolo, based on hex.cpp by Wei Dai

#include "pch.h"
#include "base32.h"
#include <atomic>
#include <thread>
#include <mutex>

NAMESPACE_BEGIN(CryptoPP)

static const byte s_vecUpper[] = "ABCDEFGHIJKMNPQRSTUVWXYZ23456789";
static const byte s_vecLower[] = "abcdefghijkmnpqrstuvwxyz23456789";

void Base32Encoder::IsolatedInitialize(const NameValuePairs &parameters)
{
  bool uppercase = parameters.GetValueWithDefault(Name::Uppercase(), true);
  m_filter->Initialize(CombinedNameValuePairs(
    parameters,
    MakeParameters(Name::EncodingLookupArray(), uppercase ? &s_vecUpper[0] : &s_vecLower[0], false)(Name::Log2Base(), 5, true)));
}

void Base32Decoder::IsolatedInitialize(const NameValuePairs &parameters)
{
  BaseN_Decoder::Initialize(CombinedNameValuePairs(
    parameters,
    MakeParameters(Name::DecodingLookupArray(), GetDefaultDecodingLookupArray(), false)(Name::Log2Base(), 5, true)));
}

#ifdef __GNUC__
const int *Base32Decoder::GetDefaultDecodingLookupArray()
{
  static volatile bool s_initialized = false;
  static int s_array[256];

  if (!s_initialized)
  {
    InitializeDecodingLookupArray(s_array, s_vecUpper, 32, true);
    s_initialized = true;
  }
  return s_array;
}
#else
const int *Base32Decoder::GetDefaultDecodingLookupArray()
{
  static std::once_flag s_initialized_flag;
  static std::atomic<bool> s_initialized(false);
  static int s_array[256];

  std::call_once(s_initialized_flag, [] {
    InitializeDecodingLookupArray(s_array, s_vecUpper, 32, true);
    s_initialized = true;
  });

  while (!s_initialized)
    std::this_thread::yield();

  return s_array;
}
#endif


NAMESPACE_END
