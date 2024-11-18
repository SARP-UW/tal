/**
 * This file is part of the Titan Avionics Library
 * Copyright (c) 2024 UW SARP
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * @file src/arch/armv7m/internal/mpu.h
 * @authors Aaron McBride
 * @brief Memory Protection Unit (MPU) management facilities.
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
  extern "C" {
#endif

  /**************************************************************************************************
   * @section Constants
   **************************************************************************************************/

  #define mpu_min_region_size 32
  #define mpu_max_region_size 0x20000000


  #define mpu_subregion_cnt 8
  #define mpu_min_subregion_size 8

  /**************************************************************************************************
   * @section Type Declarations
   **************************************************************************************************/


  /** @brief Denotes access permissions for a memory region. */
  typedef enum {
    mpu_access_null = 0,   /** @brief Undefined. */
    mpu_access_disabled,   /** @brief Read and writes are disabled. */
    mpu_access_read_only,  /** @brief Reads are enabled, writes are disabled. */
    mpu_access_read_write  /** @brief Both read and writes are enabled. */
  }mpu_access_t;


  /** @brief Denotes the type of memory a MPU region is mapped to. */
  typedef enum {
    mpu_mem_null = 0,          /** @brief Undefined. */
    mpu_mem_normal,           /** @brief Default memory type, may be shared/non-shared and can have any cache policy. */
    mpu_mem_device,           /** @brief Memory accessed by external sources. May be shared/non-shared and must be non-cacheable. */
    mpu_mem_strongly_ordered  /** @brief "Pure" memory, cannot be cached and is implicitly shared. The order of all accesses are stricly defined. */
  }mpu_mem_type_t;


  /** @brief Denotes the cache policy for a memory region. */
  typedef enum {
    mpu_cache_null = 0,         /** @brief Undefined. */
    mpu_cache_disabled,         /** @brief Caching of memory is disabled. All values written/read directly to/from memory. */
    mpu_cache_write_through,    /** @brief Value loaded into cache on read. Value updated in cache and memory on write. */
    mpu_cache_write_back,       /** @brief Value loaded into cache on read. Value updated in cache on write and memory when evicted. */
    mpu_cache_write_back_alloc  /** @brief Value loaded into cache on on read or write. Value updated in cache on write and memory when evicted. */
  }mpu_cache_policy_t;


  /** @brief Denotes the cause of a memmanage fault. */
  typedef enum {
    memmanage_fault_stacking,    /** @brief MPU access violation on exception entry. */
    memmanage_fault_unstacking,  /** @brief MPU access violation on exception return. */
    memmanage_fault_fp_state,    /** @brief MPU access violation when saving FP state on exception entry. */
    memmanage_fault_data,        /** @brief MPU access violation on explicit memory access. */
    memmanage_fault_instruction, /** @brief MPU access violation on instruction fetch (XN/protected memory). */
    memmanage_fault_unknown,     /** @brief Unknown cause. */
    memmanage_fault_none         /** @brief No fault is active. */
  }memmanage_fault_id_t;


  /** @brief Contains information about the location of a memory region. */
  typedef struct {
    uint64_t addr; /** @brief The address of the start of a memory region. */
    int32_t size;  /** @brief The size of a memory region in bytes. */
  }mpu_loc_t;


  /** @brief Contains information about the access policy of a memory region. */
  typedef struct {
    mpu_access_t priv_access;  /** @brief The privileged access policy. */
    mpu_access_t npriv_access; /** @brief The unprivileged access policy. */
    bool exec_enabled;         /** @brief True if code can be executed from this region, false otherwise. */
  }mpu_region_policy_t;


  /** @brief Contains information about the type of a memory region. */
  typedef struct {
    mpu_mem_type_t mem_type;         /** @brief The type of memory mapped to the region. */
    mpu_cache_policy_t cache_policy; /** @brief The cache policy assigned to the region. */
    bool shared;                     /** @brief True if the memory is shared, false otherwise (shared = accessible by external sources). */
  }mpu_region_type_t;


  /** @brief Contains information about a memmanage fault. */
  typedef struct {
    bool addr_valid;         /** @brief True if 'fault_addr' is valid, or false otherwise. */
    uint64_t fault_addr;     /** @brief Contains the invalid address that was accessed. */
    memmanage_fault_id_t id; /** @brief The ID/cause of the memmanage fault. */
  }memmanage_fault_info_t;

  /**************************************************************************************************
   * @section MPU Core Management Facilities
   **************************************************************************************************/

  void init_mpu(void);

  int32_t get_mpu_region_count(void);

  void set_mpu_enabled(const bool enabled);

  bool get_mpu_enabled(void);

  mpu_access_t query_npriv_policy(const uint64_t addr);

  mpu_access_t query_priv_policy(const uint64_t addr);

  bool query_xn_policy(const uint64_t addr);

  /**************************************************************************************************
   * @section MPU Region Management Facilities
   **************************************************************************************************/

  void set_mpu_region_enabled(const int32_t index, const bool enabled, bool* const err);

  bool get_mpu_region_enabled(const int32_t index, bool* const err);

  void set_mpu_region_location(const int32_t index, const mpu_loc_t loc, bool* const err);

  mpu_loc_t get_mpu_region_location(const int32_t index, bool* const err);

  int32_t find_mpu_region(const uint64_t addr);

  void set_mpu_region_type(const int32_t index, const mpu_region_type_t type, bool* const err);

  mpu_region_type_t get_mpu_region_type(const int32_t index, bool* const err);

  void set_mpu_region_policy(const int32_t index, const mpu_region_policy_t policy, bool* const err);

  mpu_region_policy_t get_mpu_region_policy(const int32_t index, bool* const err);

  /**************************************************************************************************
   * @section MPU Subregion Management Facilities
   **************************************************************************************************/

  bool set_mpu_subregion_enabled(const int32_t index, const int32_t sub_index, const bool enabled);

  bool get_mpu_subregion_enabled(const int32_t index, const int32_t sub_index);

  mpu_loc_t get_mpu_subregion_loc(const int32_t index, const int32_t sub_index);

  int32_t find_mpu_subregion(const int32_t index, const uint64_t addr);

  /**************************************************************************************************
   * @section Fault Management Facilities
   **************************************************************************************************/

  void set_memmange_fault_enabled(const bool enabled);

  bool get_memmanage_fault_enabled(void);

  bool set_memmanage_fault_priority(const int32_t priority);

  int32_t get_memmanage_fault_priority(void);

  void set_memmanage_fault_pending(const bool pending);

  bool get_memmanage_fault_pending(void);

  bool get_memmanage_fault_active(void);

  memmanage_fault_info_t get_memmanage_fault_info(void);

  /**************************************************************************************************
   * @section Miscellaneous Facilities
   **************************************************************************************************/

  void set_unaligned_fault_enabled(const bool enabled);

  bool get_unaligned_fault_enabled(void);

#if defined(__cplusplus)
  }
#endif