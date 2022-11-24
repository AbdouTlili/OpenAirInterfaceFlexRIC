/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*!
 * \file   slicing.h
 * \brief  General slice definition and helper parameters
 * \author Robert Schmidt
 * \date   2020
 * \email  robert.schmidt@eurecom.fr
 */

#ifndef __SLICING_H__
#define __SLICING_H__

#include "openair2/LAYER2/MAC/mac.h"

typedef struct slice_s {
  /// Arbitrary ID
  slice_id_t id;
  /// Arbitrary label
  char *label;

  union {
    default_sched_dl_algo_t dl_algo;
    default_sched_ul_algo_t ul_algo;
  };

  /// A specific algorithm's implementation parameters
  void *algo_data;
  /// Internal data that might be kept alongside a slice's params
  void *int_data;

  // list of users in this slice
  UE_list_t UEs;
} slice_t;

typedef struct slice_info_s {
  uint8_t num;
  slice_t **s;
  uint8_t UE_assoc_slice[MAX_MOBILES_PER_ENB];
} slice_info_t;

int slicing_get_UE_slice_idx(slice_info_t *si, int UE_id);

#define STATIC_SLICING 10
/* only four static slices for UL, DL resp. (not enough DCIs) */
#define MAX_STATIC_SLICES 4
typedef struct {
  uint16_t posLow;
  uint16_t posHigh;
} static_slice_param_t;
pp_impl_param_t static_dl_init(module_id_t mod_id, int CC_id);
pp_impl_param_t static_ul_init(module_id_t mod_id, int CC_id);


#define NVS_SLICING 20
/* arbitrary upper limit, increase if you want to instantiate more slices */
#define MAX_NVS_SLICES 10
/* window for slice weight averaging -> 1s for fine granularity */
#define BETA 0.001f
typedef struct {
  enum nvs_type {NVS_RATE, NVS_RES} type;
  union {
    struct { float Mbps_reserved; float Mbps_reference; };
    struct { float pct_reserved; };
  };
} nvs_slice_param_t;
pp_impl_param_t nvs_dl_init(module_id_t mod_id, int CC_id);


#define EDF_SLICING 40
/* arbitrary upper limit, increase if you want to instantiate more slices */
#define MAX_EDF_SLICES 10
typedef struct {
  uint32_t deadline;
  uint32_t guaranteed_prbs;
  uint16_t max_replenish;
  // not defined as slice_id_t to allow efficient protobuf exatraction
  uint32_t loverride[MAX_EDF_SLICES];
  uint8_t noverride;
} edf_slice_param_t;
pp_impl_param_t edf_dl_init(module_id_t mod_id, int CC_id);

#endif /* __SLICING_H__ */
