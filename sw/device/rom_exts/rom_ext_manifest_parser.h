// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef OPENTITAN_SW_DEVICE_ROM_EXTS_ROM_EXT_MANIFEST_PARSER_H_
#define OPENTITAN_SW_DEVICE_ROM_EXTS_ROM_EXT_MANIFEST_PARSER_H_

#include <stdbool.h>
#include <stdint.h>

#include "sw/device/lib/base/mmio.h"
#include "sw/device/rom_exts/manifest.h"

// Header Extern Guard (so header can be used from C and C++)
#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * ROM Extension manifest slot type.
 */
typedef uintptr_t rom_ext_manifest_slot_t;

/**
 * ROM Extension manifest slots base addresses.
 *
 * These are intended to be used as an input parameter to
 * `rom_ext_parameters_get`.
 */
extern const rom_ext_manifest_slot_t kRomExtManifestSlotA;
extern const rom_ext_manifest_slot_t kRomExtManifestSlotB;

/**
 * ROM Extension parameters required for the manifest parsing.
 */
typedef struct rom_ext_manifest {
  /**
   * Base address of the manifest in memory.
   */
  mmio_region_t base_addr;
  rom_ext_manifest_slot_t slot;
} rom_ext_manifest_t;

/**
 * ROM Extension image signature.
 */
typedef struct rom_ext_signature {
  uint32_t data[ROM_EXT_IMAGE_SIGNATURE_SIZE_WORDS];
} rom_ext_signature_t;

/**
 * ROM Extension lockdown information.
 *
 * TODO - probably would eventually become an internal type, and there will
 *        be another public type with more finely parsed information.
 */
typedef struct rom_ext_lockdown_info {
  uint32_t data[ROM_EXT_PERIPHERAL_LOCKDOWN_INFO_SIZE_WORDS];
} rom_ext_lockdown_info_t;

/**
 * ROM Extension public key.
 */
typedef struct rom_ext_public_key {
  uint32_t data[ROM_EXT_SIGNATURE_PUBLIC_KEY_SIZE_WORDS];
} rom_ext_public_key_t;

/**
 * ROM Extension image extension IDs.
 */
typedef enum rom_ext_extension_id {
  /**
   * Image extension 0.
   */
  kRomExtExtensionId0 = 0,
  /**
   * Image extension 1.
   */
  kRomExtExtensionId1,
  /**
   * Image extension 2.
   */
  kRomExtExtensionId2,
  /**
   * Image extension 3.
   */
  kRomExtExtensionId3,
} rom_ext_extension_id_t;

/**
 * ROM Extension image extension.
 */
typedef struct rom_ext_extension {
  /**
   * Image extension address in memory.
   */
  void *address;
  /**
   * Image extension checksum.
   */
  uint32_t checksum;
} rom_ext_extension_t;

/**
 * Creates the ROM extension manifest parameters.
 *
 * Required for all ROM Extension manifest parser API.
 *
 * @param slot ROM Extension manifest slot base address.
 * @return `rom_ext_manifest_t`.
 */
rom_ext_manifest_t rom_ext_get_parameters(rom_ext_manifest_slot_t slot);

/**
 * Retrieves the ROM_EXT identifier.
 *
 * The memory address where ROM_EXT identifier field resides, is relative.
 *
 * @param params Parameters required for manifest parsing.
 * @return ROM_EXT identifier.
 */
uint32_t rom_ext_get_identifier(rom_ext_manifest_t params);

/**
 * Retrieves the ROM_EXT signature.
 *
 * The memory address where ROM_EXT identifier field resides, is relative.
 *
 * @param params Parameters required for manifest parsing.
 * @param dst The destination address where the signature is coppied to.
 * @return `true` on success, `false` on failure.
 */
bool rom_ext_get_signature(rom_ext_manifest_t params, rom_ext_signature_t *dst);

/**
 * Retrieves the ROM_EXT image length.
 *
 * The memory address where ROM_EXT image length field resides, is relative.
 *
 * @param params Parameters required for manifest parsing.
 * @return ROM_EXT image length.
 */
uint32_t rom_ext_get_image_len(rom_ext_manifest_t params);

/**
 * Retrieves the ROM_EXT image version.
 *
 * The memory address where ROM_EXT image version field resides, is relative.
 *
 * @param params Parameters required for manifest parsing.
 * @return ROM_EXT image version.
 */
uint32_t rom_ext_get_version(rom_ext_manifest_t params);

/**
 * Retrieves the ROM_EXT image timestamp.
 *
 * The memory address where ROM_EXT image timestamp field resides, is relative.
 *
 * @param params Parameters required for manifest parsing.
 * @return ROM_EXT image timestamp.
 */
uint64_t rom_ext_get_timestamp(rom_ext_manifest_t params);

/**
 * Retrieves the ROM_EXT signature algorithm identifier.
 *
 * The memory address where ROM_EXT signature algorithm identifier field
 * resides, is relative.
 *
 * @param params Parameters required for manifest parsing.
 * @return ROM_EXT signature algorithm identifier.
 */
uint32_t rom_ext_get_algorithm_id(rom_ext_manifest_t params);

/**
 * Retrieves the ROM_EXT exponent.
 *
 * The memory address where ROM_EXT exponent field resides, is relative.
 *
 * @param params Parameters required for manifest parsing.
 * @return ROM_EXT exponent.
 */
uint32_t rom_ext_get_exponent(rom_ext_manifest_t params);

/**
 * Retrieves the ROM_EXT usage constraints.
 *
 * The memory address where ROM_EXT usage constraints field resides, is
 * relative.
 *
 * @param params Parameters required for manifest parsing.
 * @return ROM_EXT usage constraints.
 */
uint32_t rom_ext_get_usage_constraints(rom_ext_manifest_t params);

/**
 * Retrieves the ROM_EXT lockdown info.
 *
 * The memory address where ROM_EXT lockdown info field resides, is relative.
 *
 * @param params Parameters required for manifest parsing.
 * @param dst The destination address where the lockdown info is coppied to.
 * @return `true` on success, `false` on failure.
 */
bool rom_ext_get_peripheral_lockdown_info(rom_ext_manifest_t params,
                                          rom_ext_lockdown_info_t *dst);

/**
 * Retrieves the ROM_EXT public key.
 *
 * The memory address where ROM_EXT public key field resides, is relative.
 *
 * @param params Parameters required for manifest parsing.
 * @param dst The destination address where the public key is coppied to.
 * @return `true` on success, `false` on failure.
 */
bool rom_ext_get_public_key(rom_ext_manifest_t params,
                            rom_ext_public_key_t *dst);

/**
 * Retrieves the ROM_EXT image extension specified in `id`.
 *
 * The memory address where ROM_EXT image extension information fields reside,
 * is relative.
 *
 * @param params Parameters required for manifest parsing.
 * @param id Extension identifier.
 * @param extension Parsed `rom_ext_extension_t` output.
 * @return `true` on success, `false` on failure.
 */
bool rom_ext_get_extension(rom_ext_manifest_t params, rom_ext_extension_id_t id,
                           rom_ext_extension_t *extension);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // OPENTITAN_SW_DEVICE_ROM_EXTS_ROM_EXT_MANIFEST_PARSER_H_
