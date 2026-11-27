// tekken6-nx-installer (2026-11-27): Tekken 6's shader containers are all the standard 0x102A11xx ones; the version
// 0x0E form this front end also reads (Kameo's, kameo-nx's container_0e.h) never occurs, so this stands in for it.
#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

namespace container_0e {
inline bool IsContainer0E(const uint8_t *, size_t) { return false; }
inline uint32_t Rd(const uint8_t *, size_t) { return 0; }
inline std::vector<uint8_t> Convert(const uint8_t *) { return {}; }
}  // namespace container_0e
