/*
 * Copyright 2013-2020 Software Radio Systems Limited
 *
 * This file is part of srsLTE.
 *
 * srsLTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsLTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#include "srsenb/hdr/stack/mac/scheduler_grid.h"
#include "srsenb/hdr/stack/mac/scheduler.h"
#include "srslte/common/log_helper.h"
#include "srslte/common/logmap.h"
#include <srslte/interfaces/sched_interface.h>

using srslte::tti_point;

namespace srsenb {

const char* alloc_outcome_t::to_string() const
{
  switch (result) {
    case SUCCESS:
      return "success";
    case DCI_COLLISION:
      return "dci_collision";
    case RB_COLLISION:
      return "rb_collision";
    case ERROR:
      return "error";
    case NOF_RB_INVALID:
      return "invalid nof prbs";
    case PUCCH_COLLISION:
      return "pucch_collision";
  }
  return "unknown error";
}

tti_params_t::tti_params_t(uint32_t tti_rx_) :
  tti_rx(tti_rx_),
  sf_idx_tx_dl(TTI_ADD(tti_rx, FDD_HARQ_DELAY_UL_MS) % 10),
  tti_tx_dl(TTI_ADD(tti_rx, FDD_HARQ_DELAY_UL_MS)),
  tti_tx_ul(TTI_ADD(tti_rx, (FDD_HARQ_DELAY_UL_MS + FDD_HARQ_DELAY_DL_MS))),
  sfn_tx_dl(TTI_ADD(tti_rx, FDD_HARQ_DELAY_UL_MS) / 10)
{}

cc_sched_result* sf_sched_result::new_cc(uint32_t enb_cc_idx)
{
  if (enb_cc_idx >= enb_cc_list.size()) {
    enb_cc_list.resize(enb_cc_idx + 1);
  }
  return &enb_cc_list[enb_cc_idx];
}

bool sf_sched_result::is_ul_alloc(uint16_t rnti) const
{
  for (uint32_t i = 0; i < enb_cc_list.size(); ++i) {
    for (uint32_t j = 0; j < enb_cc_list[i].ul_sched_result.nof_dci_elems; ++j) {
      if (enb_cc_list[i].ul_sched_result.pusch[j].dci.rnti == rnti) {
        return true;
      }
    }
  }
  return false;
}
bool sf_sched_result::is_dl_alloc(uint16_t rnti) const
{
  for (uint32_t i = 0; i < enb_cc_list.size(); ++i) {
    for (uint32_t j = 0; j < enb_cc_list[i].dl_sched_result.nof_data_elems; ++j) {
      if (enb_cc_list[i].dl_sched_result.data[j].dci.rnti == rnti) {
        return true;
      }
    }
  }
  return false;
}

sf_sched_result* sched_result_list::new_tti(srslte::tti_point tti_rx)
{
  sf_sched_result* res = &results[tti_rx.to_uint() % results.size()];
  res->tti_rx          = tti_rx;
  res->enb_cc_list.clear();
  return res;
}

sf_sched_result* sched_result_list::get_sf(srslte::tti_point tti_rx)
{
  sf_sched_result* res = &results[tti_rx.to_uint() % results.size()];
  return (res->tti_rx != tti_rx) ? nullptr : res;
}

const sf_sched_result* sched_result_list::get_sf(srslte::tti_point tti_rx) const
{
  const sf_sched_result* res = &results[tti_rx.to_uint() % results.size()];
  return (res->tti_rx != tti_rx) ? nullptr : res;
}

const cc_sched_result* sched_result_list::get_cc(srslte::tti_point tti_rx, uint32_t enb_cc_idx) const
{
  const sf_sched_result* res = get_sf(tti_rx);
  return res != nullptr ? res->get_cc(enb_cc_idx) : nullptr;
}

cc_sched_result* sched_result_list::get_cc(srslte::tti_point tti_rx, uint32_t enb_cc_idx)
{
  sf_sched_result* res = get_sf(tti_rx);
  return res != nullptr ? res->get_cc(enb_cc_idx) : nullptr;
}

/*******************************************************
 *             PDCCH Allocation Methods
 *******************************************************/

void pdcch_grid_t::alloc_tree_t::reset()
{
  prev_start = 0;
  prev_end   = 0;
  dci_alloc_tree.clear();
}

void pdcch_grid_t::init(const sched_cell_params_t& cell_params_)
{
  cc_cfg = &cell_params_;
  log_h  = srslte::logmap::get("MAC ");

  // init alloc trees
  alloc_trees.reserve(cc_cfg->sched_cfg->max_nof_ctrl_symbols);
  for (uint32_t i = 0; i < cc_cfg->sched_cfg->max_nof_ctrl_symbols; ++i) {
    alloc_trees.emplace_back(cc_cfg->nof_cce_table[i]);
  }
}

void pdcch_grid_t::new_tti(const tti_params_t& tti_params_)
{
  tti_params = &tti_params_;

  // Reset back all CFIs
  for (auto& t : alloc_trees) {
    t.reset();
  }
  dci_record_list.clear();
  current_cfix = cc_cfg->sched_cfg->min_nof_ctrl_symbols - 1;
}

const sched_dci_cce_t* pdcch_grid_t::get_cce_loc_table(alloc_type_t alloc_type, sched_ue* user, uint32_t cfix) const
{
  switch (alloc_type) {
    case alloc_type_t::DL_BC:
      return &cc_cfg->common_locations[cfix];
    case alloc_type_t::DL_PCCH:
      return &cc_cfg->common_locations[cfix];
    case alloc_type_t::DL_RAR:
      return &cc_cfg->rar_locations[cfix][tti_params->sf_idx_tx_dl];
    case alloc_type_t::DL_DATA:
      return user->get_locations(cc_cfg->enb_cc_idx, cfix + 1, tti_params->sf_idx_tx_dl);
    case alloc_type_t::UL_DATA:
      return user->get_locations(cc_cfg->enb_cc_idx, cfix + 1, tti_params->sf_idx_tx_dl);
    default:
      break;
  }
  return nullptr;
}

bool pdcch_grid_t::alloc_dci(alloc_type_t alloc_type, uint32_t aggr_idx, sched_ue* user)
{
  // TODO: Make the alloc tree update lazy
  alloc_record_t record{.user = user, .aggr_idx = aggr_idx, .alloc_type = alloc_type};

  // Try to allocate user in PDCCH for given CFI. If it fails, increment CFI.
  uint32_t first_cfi = get_cfi();
  bool     success;
  do {
    success = alloc_dci_record(record, get_cfi() - 1);
  } while (not success and get_cfi() < cc_cfg->sched_cfg->max_nof_ctrl_symbols and set_cfi(get_cfi() + 1));

  if (not success) {
    // DCI allocation failed. go back to original CFI
    if (get_cfi() != first_cfi and not set_cfi(first_cfi)) {
      log_h->error("SCHED: Failed to return back to original PDCCH state\n");
    }
    return false;
  }

  // DCI record allocation successful
  dci_record_list.push_back(record);
  return true;
}

bool pdcch_grid_t::alloc_dci_record(const alloc_record_t& record, uint32_t cfix)
{
  bool  ret  = false;
  auto& tree = alloc_trees[cfix];

  // Get DCI Location Table
  const sched_dci_cce_t* dci_locs = get_cce_loc_table(record.alloc_type, record.user, cfix);
  if (dci_locs == nullptr or dci_locs->nof_loc[record.aggr_idx] == 0) {
    return ret;
  }

  if (tree.prev_end > 0) {
    for (size_t j = tree.prev_start; j < tree.prev_end; ++j) {
      ret |= add_tree_node_leaves(tree, (int)j, record, *dci_locs, tti_params->tti_tx_dl);
    }
  } else {
    ret = add_tree_node_leaves(tree, -1, record, *dci_locs, tti_params->tti_tx_dl);
  }

  if (ret) {
    tree.prev_start = tree.prev_end;
    tree.prev_end   = tree.dci_alloc_tree.size();
  }

  return ret;
}

//! Algorithm to compute a valid PDCCH allocation
bool pdcch_grid_t::add_tree_node_leaves(alloc_tree_t&          tree,
                                        int                    parent_node_idx,
                                        const alloc_record_t&  dci_record,
                                        const sched_dci_cce_t& dci_locs,
                                        uint32_t               tti_tx_dl)
{
  bool ret = false;

  alloc_t alloc;
  alloc.rnti      = (dci_record.user != nullptr) ? dci_record.user->get_rnti() : (uint16_t)0u;
  alloc.dci_pos.L = dci_record.aggr_idx;

  // get cumulative pdcch mask
  pdcch_mask_t cum_mask;
  if (parent_node_idx >= 0) {
    cum_mask = tree.dci_alloc_tree[parent_node_idx].node.total_mask;
  } else {
    cum_mask.resize(tree.nof_cces);
  }

  uint32_t nof_locs = dci_locs.nof_loc[dci_record.aggr_idx];
  for (uint32_t i = 0; i < nof_locs; ++i) {
    uint32_t startpos = dci_locs.cce_start[dci_record.aggr_idx][i];

    if (dci_record.alloc_type == alloc_type_t::DL_DATA and dci_record.user->pucch_sr_collision(tti_tx_dl, startpos)) {
      // will cause a collision in the PUCCH
      continue;
    }

    pdcch_mask_t alloc_mask(tree.nof_cces);
    alloc_mask.fill(startpos, startpos + (1u << dci_record.aggr_idx));
    if ((cum_mask & alloc_mask).any()) {
      // there is collision. Try another mask
      continue;
    }

    // Allocation successful
    alloc.current_mask = alloc_mask;
    alloc.total_mask   = cum_mask | alloc_mask;
    alloc.dci_pos.ncce = startpos;

    // Prune if repetition
    uint32_t j = tree.prev_end;
    for (; j < tree.dci_alloc_tree.size(); ++j) {
      if (tree.dci_alloc_tree[j].node.total_mask == alloc.total_mask) {
        break;
      }
    }
    if (j < tree.dci_alloc_tree.size()) {
      continue;
    }

    // Register allocation
    tree.dci_alloc_tree.emplace_back(parent_node_idx, alloc);
    ret = true;
  }

  return ret;
}

bool pdcch_grid_t::set_cfi(uint32_t cfi)
{
  if (cfi < cc_cfg->sched_cfg->min_nof_ctrl_symbols or cfi > cc_cfg->sched_cfg->max_nof_ctrl_symbols) {
    srslte::logmap::get("MAC")->error("Invalid CFI value. Defaulting to current CFI.\n");
    return false;
  }

  uint32_t new_cfix = cfi - 1;
  if (new_cfix == current_cfix) {
    return true;
  }

  // setup new PDCCH alloc tree
  auto& new_tree = alloc_trees[new_cfix];
  new_tree.reset();

  if (not dci_record_list.empty()) {
    // there are already PDCCH allocations

    // Rebuild Allocation Tree
    bool ret = true;
    for (const auto& old_record : dci_record_list) {
      ret &= alloc_dci_record(old_record, new_cfix);
    }

    if (not ret) {
      // Fail to rebuild allocation tree. Go back to previous CFI
      return false;
    }
  }

  current_cfix = new_cfix;
  // TODO: The estimation of the number of required prbs in metric depends on CFI. Analyse the consequences
  return true;
}

void pdcch_grid_t::get_allocs(alloc_result_t* vec, pdcch_mask_t* tot_mask, size_t idx) const
{
  auto& tree = alloc_trees[current_cfix];
  // if alloc tree is empty
  if (tree.prev_start == tree.prev_end) {
    if (vec != nullptr) {
      vec->clear();
    }
    if (tot_mask != nullptr) {
      tot_mask->resize(nof_cces());
      tot_mask->reset();
    }
    return;
  }

  // set vector of allocations
  if (vec != nullptr) {
    vec->clear();
    size_t i = tree.prev_start + idx;
    while (tree.dci_alloc_tree[i].parent_idx >= 0) {
      vec->push_back(&tree.dci_alloc_tree[i].node);
      i = (size_t)tree.dci_alloc_tree[i].parent_idx;
    }
    vec->push_back(&tree.dci_alloc_tree[i].node);
    std::reverse(vec->begin(), vec->end());
  }

  // set final cce mask
  if (tot_mask != nullptr) {
    *tot_mask = tree.dci_alloc_tree[tree.prev_start + idx].node.total_mask;
  }
}

std::string pdcch_grid_t::result_to_string(bool verbose) const
{
  auto&             tree = alloc_trees[current_cfix];
  std::stringstream ss;
  ss << "cfi=" << get_cfi() << ", mask_size=" << nof_cces() << ", " << tree.prev_end - tree.prev_start
     << " DCI allocation combinations:\n";
  // get all the possible combinations of DCI allocations
  uint32_t count = 0;
  for (size_t i = tree.prev_start; i < tree.prev_end; ++i) {
    alloc_result_t vec;
    pdcch_mask_t   tot_mask;
    get_allocs(&vec, &tot_mask, i - tree.prev_start);

    ss << "                          combination " << count << ": mask=0x" << tot_mask.to_hex().c_str();
    if (verbose) {
      ss << ", DCI allocs:\n";
      for (const auto& dci_alloc : vec) {
        char hex[5];
        sprintf(hex, "%x", dci_alloc->rnti);
        ss << "                          > rnti=0x" << hex << ": " << dci_alloc->current_mask.to_hex().c_str() << " / "
           << dci_alloc->total_mask.to_hex().c_str() << "\n";
      }
    } else {
      ss << "\n";
    }
    count++;
  }

  return ss.str();
}

/*******************************************************
 *          TTI resource Scheduling Methods
 *******************************************************/

void sf_grid_t::init(const sched_cell_params_t& cell_params_)
{
  cc_cfg    = &cell_params_;
  log_h     = srslte::logmap::get("MAC ");
  nof_rbgs  = cc_cfg->nof_rbgs;
  si_n_rbg  = srslte::ceil_div(4, cc_cfg->P);
  rar_n_rbg = srslte::ceil_div(3, cc_cfg->P);

  dl_mask.resize(nof_rbgs);
  ul_mask.resize(cc_cfg->nof_prb());

  pdcch_alloc.init(*cc_cfg);
}

void sf_grid_t::new_tti(const tti_params_t& tti_params_)
{
  tti_params = &tti_params_;

  dl_mask.reset();
  ul_mask.reset();
  avail_rbg = nof_rbgs;

  // internal state
  pdcch_alloc.new_tti(*tti_params);
}

//! Allocates CCEs and RBs for the given mask and allocation type (e.g. data, BC, RAR, paging)
alloc_outcome_t sf_grid_t::alloc_dl(uint32_t aggr_idx, alloc_type_t alloc_type, rbgmask_t alloc_mask, sched_ue* user)
{
  // Check RBG collision
  if ((dl_mask & alloc_mask).any()) {
    return alloc_outcome_t::RB_COLLISION;
  }

  // Allocate DCI in PDCCH
  if (not pdcch_alloc.alloc_dci(alloc_type, aggr_idx, user)) {
    if (user != nullptr) {
      if (log_h->get_level() == srslte::LOG_LEVEL_DEBUG) {
        log_h->debug("No space in PDCCH for rnti=0x%x DL tx. Current PDCCH allocation: %s\n",
                     user->get_rnti(),
                     pdcch_alloc.result_to_string(true).c_str());
      }
    }
    return alloc_outcome_t::DCI_COLLISION;
  }

  // Allocate RBGs
  dl_mask |= alloc_mask;
  avail_rbg -= alloc_mask.count();

  return alloc_outcome_t::SUCCESS;
}

//! Allocates CCEs and RBs for control allocs. It allocates RBs in a contiguous manner.
sf_grid_t::dl_ctrl_alloc_t sf_grid_t::alloc_dl_ctrl(uint32_t aggr_idx, alloc_type_t alloc_type)
{
  rbg_interval range{nof_rbgs - avail_rbg,
                     nof_rbgs - avail_rbg + ((alloc_type == alloc_type_t::DL_RAR) ? rar_n_rbg : si_n_rbg)};

  if (alloc_type != alloc_type_t::DL_RAR and alloc_type != alloc_type_t::DL_BC and
      alloc_type != alloc_type_t::DL_PCCH) {
    log_h->error("SCHED: DL control allocations must be RAR/BC/PDCCH\n");
    return {alloc_outcome_t::ERROR, range};
  }
  // Setup range starting from left
  if (range.stop() > nof_rbgs) {
    return {alloc_outcome_t::RB_COLLISION, range};
  }

  // allocate DCI and RBGs
  rbgmask_t new_mask(dl_mask.size());
  new_mask.fill(range.start(), range.stop());
  return {alloc_dl(aggr_idx, alloc_type, new_mask), range};
}

//! Allocates CCEs and RBs for a user DL data alloc.
alloc_outcome_t sf_grid_t::alloc_dl_data(sched_ue* user, const rbgmask_t& user_mask)
{
  srslte_dci_format_t dci_format = user->get_dci_format();
  uint32_t            nof_bits   = srslte_dci_format_sizeof(&cc_cfg->cfg.cell, nullptr, nullptr, dci_format);
  uint32_t            aggr_idx   = user->find_ue_carrier(cc_cfg->enb_cc_idx)->get_aggr_level(nof_bits);
  alloc_outcome_t     ret        = alloc_dl(aggr_idx, alloc_type_t::DL_DATA, user_mask, user);

  return ret;
}

alloc_outcome_t sf_grid_t::alloc_ul_data(sched_ue* user, prb_interval alloc, bool needs_pdcch)
{
  if (alloc.stop() > ul_mask.size()) {
    return alloc_outcome_t::ERROR;
  }

  prbmask_t newmask(ul_mask.size());
  newmask.fill(alloc.start(), alloc.stop());
  if ((ul_mask & newmask).any()) {
    return alloc_outcome_t::RB_COLLISION;
  }

  // Generate PDCCH except for RAR and non-adaptive retx
  if (needs_pdcch) {
    uint32_t nof_bits = srslte_dci_format_sizeof(&cc_cfg->cfg.cell, nullptr, nullptr, SRSLTE_DCI_FORMAT0);
    uint32_t aggr_idx = user->find_ue_carrier(cc_cfg->enb_cc_idx)->get_aggr_level(nof_bits);
    if (not pdcch_alloc.alloc_dci(alloc_type_t::UL_DATA, aggr_idx, user)) {
      if (log_h->get_level() == srslte::LOG_LEVEL_DEBUG) {
        log_h->debug("No space in PDCCH for rnti=0x%x UL tx. Current PDCCH allocation: %s\n",
                     user->get_rnti(),
                     pdcch_alloc.result_to_string(true).c_str());
      }
      return alloc_outcome_t::DCI_COLLISION;
    }
  }

  ul_mask |= newmask;

  return alloc_outcome_t::SUCCESS;
}

bool sf_grid_t::reserve_dl_rbgs(uint32_t start_rbg, uint32_t end_rbg)
{
  dl_mask.fill(start_rbg, end_rbg);
  return true;
}

bool sf_grid_t::reserve_ul_prbs(const prbmask_t& prbmask, bool strict)
{
  bool ret = true;
  if (strict and (ul_mask & prbmask).any()) {
    log_h->error("There was a collision in UL channel. current mask=0x%s, new alloc mask=0x%s\n",
                 ul_mask.to_hex().c_str(),
                 prbmask.to_hex().c_str());
    ret = false;
  }
  ul_mask |= prbmask;
  return ret;
}

/**
 * Finds a range of L contiguous PRBs that are empty
 * @param L Size of the requested UL allocation in PRBs
 * @param alloc Found allocation. It is guaranteed that 0 <= alloc->L <= L
 * @return true if the requested allocation of size L was strictly met
 */
bool sf_grid_t::find_ul_alloc(uint32_t L, prb_interval* alloc) const
{
  *alloc = {};
  for (uint32_t n = 0; n < ul_mask.size() && alloc->length() < L; n++) {
    if (not ul_mask.test(n) && alloc->length() == 0) {
      alloc->displace_to(n);
    }
    if (not ul_mask.test(n)) {
      alloc->resize_by(1);
    } else if (alloc->length() > 0) {
      // avoid edges
      if (n < 3) {
        *alloc = {};
      } else {
        break;
      }
    }
  }
  if (alloc->length() == 0) {
    return false;
  }

  // Make sure L is allowed by SC-FDMA modulation
  while (!srslte_dft_precoding_valid_prb(alloc->length())) {
    alloc->resize_by(-1);
  }
  return alloc->length() == L;
}

/*******************************************************
 *          TTI resource Scheduling Methods
 *******************************************************/

sf_sched::sf_sched() : log_h(srslte::logmap::get("MAC")) {}

void sf_sched::init(const sched_cell_params_t& cell_params_)
{
  cc_cfg = &cell_params_;
  tti_alloc.init(*cc_cfg);
  max_msg3_prb = std::max(6u, cc_cfg->cfg.cell.nof_prb - (uint32_t)cc_cfg->cfg.nrb_pucch);

  pucch_mask.resize(cc_cfg->nof_prb());
  if (cc_cfg->cfg.nrb_pucch > 0) {
    pucch_mask.fill(0, (uint32_t)cc_cfg->cfg.nrb_pucch);
    pucch_mask.fill(cc_cfg->nof_prb() - cc_cfg->cfg.nrb_pucch, cc_cfg->nof_prb());
  }
}

void sf_sched::new_tti(tti_point tti_rx_, sf_sched_result* cc_results_)
{
  // reset internal state
  bc_allocs.clear();
  rar_allocs.clear();
  data_allocs.clear();
  ul_data_allocs.clear();

  tti_params = tti_params_t{tti_rx_.to_uint()};
  tti_alloc.new_tti(tti_params);
  cc_results = cc_results_;

  // Reserve PRBs for PUCCH
  reserve_ul_prbs(pucch_mask, true);

  // Reserve PRBs for PRACH
  if (srslte_prach_tti_opportunity_config_fdd(cc_cfg->cfg.prach_config, tti_params.tti_tx_ul, -1)) {
    prbmask_t prach_mask{cc_cfg->nof_prb()};
    prach_mask.fill(cc_cfg->cfg.prach_freq_offset, cc_cfg->cfg.prach_freq_offset + 6);
    reserve_ul_prbs(prach_mask, cc_cfg->nof_prb() != 6);
    log_h->debug("SCHED: Allocated PRACH RBs. Mask: 0x%s\n", prach_mask.to_hex().c_str());
  }

  // setup first prb to be used for msg3 alloc. Account for potential PRACH alloc
  last_msg3_prb           = cc_cfg->cfg.nrb_pucch;
  uint32_t tti_msg3_alloc = TTI_ADD(tti_params.tti_tx_ul, MSG3_DELAY_MS);
  if (srslte_prach_tti_opportunity_config_fdd(cc_cfg->cfg.prach_config, tti_msg3_alloc, -1)) {
    last_msg3_prb = std::max(last_msg3_prb, cc_cfg->cfg.prach_freq_offset + 6);
  }
}

bool sf_sched::is_dl_alloc(uint16_t rnti) const
{
  for (const auto& a : data_allocs) {
    if (a.rnti == rnti) {
      return true;
    }
  }
  return false;
}

bool sf_sched::is_ul_alloc(uint16_t rnti) const
{
  for (const auto& a : ul_data_allocs) {
    if (a.rnti == rnti) {
      return true;
    }
  }
  return false;
}

sf_sched::ctrl_code_t sf_sched::alloc_dl_ctrl(uint32_t aggr_lvl, uint32_t tbs_bytes, uint16_t rnti)
{
  ctrl_alloc_t ctrl_alloc{};

  // based on rnti, check which type of alloc
  alloc_type_t alloc_type = alloc_type_t::DL_RAR;
  if (rnti == SRSLTE_SIRNTI) {
    alloc_type = alloc_type_t::DL_BC;
  } else if (rnti == SRSLTE_PRNTI) {
    alloc_type = alloc_type_t::DL_PCCH;
  }

  /* Allocate space in the DL RBG and PDCCH grids */
  sf_grid_t::dl_ctrl_alloc_t ret = tti_alloc.alloc_dl_ctrl(aggr_lvl, alloc_type);
  if (not ret.outcome) {
    return {ret.outcome, ctrl_alloc};
  }

  // Allocation Successful
  ctrl_alloc.dci_idx    = tti_alloc.get_pdcch_grid().nof_allocs() - 1;
  ctrl_alloc.rbg_range  = ret.rbg_range;
  ctrl_alloc.rnti       = rnti;
  ctrl_alloc.req_bytes  = tbs_bytes;
  ctrl_alloc.alloc_type = alloc_type;

  return {ret.outcome, ctrl_alloc};
}

alloc_outcome_t sf_sched::alloc_bc(uint32_t aggr_lvl, uint32_t sib_idx, uint32_t sib_ntx)
{
  uint32_t    sib_len = cc_cfg->cfg.sibs[sib_idx].len;
  uint32_t    rv      = sched_utils::get_rvidx(sib_ntx);
  ctrl_code_t ret     = alloc_dl_ctrl(aggr_lvl, sib_len, SRSLTE_SIRNTI);
  if (not ret.first) {
    Warning("SCHED: Could not allocate SIB=%d, L=%d, len=%d, cause=%s\n",
            sib_idx + 1,
            aggr_lvl,
            sib_len,
            ret.first.to_string());
    return ret.first;
  }

  // BC allocation successful
  bc_alloc_t bc_alloc(ret.second);
  bc_alloc.rv      = rv;
  bc_alloc.sib_idx = sib_idx;
  bc_allocs.push_back(bc_alloc);

  return ret.first;
}

alloc_outcome_t sf_sched::alloc_paging(uint32_t aggr_lvl, uint32_t paging_payload)
{
  ctrl_code_t ret = alloc_dl_ctrl(aggr_lvl, paging_payload, SRSLTE_PRNTI);
  if (not ret.first) {
    Warning(
        "SCHED: Could not allocate Paging with payload length=%d, cause=%s\n", paging_payload, ret.first.to_string());
    return ret.first;
  }

  // Paging allocation successful
  bc_alloc_t bc_alloc(ret.second);
  bc_allocs.push_back(bc_alloc);

  return ret.first;
}

std::pair<alloc_outcome_t, uint32_t> sf_sched::alloc_rar(uint32_t aggr_lvl, const pending_rar_t& rar)
{
  const uint32_t                       msg3_grant_size = 3;
  std::pair<alloc_outcome_t, uint32_t> ret             = {alloc_outcome_t::ERROR, 0};

  for (uint32_t nof_grants = rar.nof_grants; nof_grants > 0; nof_grants--) {
    uint32_t buf_rar         = 7 * nof_grants + 1; // 1+6 bytes per RAR subheader+body and 1 byte for Backoff
    uint32_t total_msg3_size = msg3_grant_size * nof_grants;

    // check if there is enough space for Msg3, try again with a lower number of grants
    if (last_msg3_prb + total_msg3_size > max_msg3_prb) {
      ret.first = alloc_outcome_t::RB_COLLISION;
      continue;
    }

    // allocate RBs and PDCCH
    sf_sched::ctrl_code_t ret2 = alloc_dl_ctrl(aggr_lvl, buf_rar, rar.ra_rnti);
    ret.first                  = ret2.first.result;
    ret.second                 = nof_grants;

    // if there was no space for the RAR, try again
    if (ret.first == alloc_outcome_t::RB_COLLISION) {
      continue;
    }
    // if any other error, return
    if (ret.first != alloc_outcome_t::SUCCESS) {
      log_h->warning("SCHED: Could not allocate RAR for L=%d, cause=%s\n", aggr_lvl, ret.first.to_string());
      return ret;
    }

    // RAR allocation successful
    sched_interface::dl_sched_rar_t rar_grant = {};
    rar_grant.nof_grants                      = nof_grants;
    for (uint32_t i = 0; i < nof_grants; ++i) {
      rar_grant.msg3_grant[i].data            = rar.msg3_grant[i];
      rar_grant.msg3_grant[i].grant.tpc_pusch = 3;
      rar_grant.msg3_grant[i].grant.trunc_mcs = 0;
      uint32_t rba = srslte_ra_type2_to_riv(msg3_grant_size, last_msg3_prb, cc_cfg->cfg.cell.nof_prb);
      rar_grant.msg3_grant[i].grant.rba = rba;

      last_msg3_prb += msg3_grant_size;
    }
    rar_allocs.emplace_back(ret2.second, rar_grant);

    break;
  }
  if (ret.first != alloc_outcome_t::SUCCESS) {
    log_h->warning("SCHED: Failed to allocate RAR due to lack of RBs\n");
  }
  return ret;
}

bool is_periodic_cqi_expected(const sched_interface::ue_cfg_t& ue_cfg, uint32_t tti_tx_ul)
{
  for (const sched_interface::ue_cfg_t::cc_cfg_t& cc : ue_cfg.supported_cc_list) {
    if (cc.dl_cfg.cqi_report.periodic_configured) {
      if (srslte_cqi_periodic_send(&cc.dl_cfg.cqi_report, tti_tx_ul, SRSLTE_FDD)) {
        return true;
      }
    }
  }
  return false;
}

alloc_outcome_t sf_sched::alloc_dl_user(sched_ue* user, const rbgmask_t& user_mask, uint32_t pid)
{
  if (is_dl_alloc(user->get_rnti())) {
    log_h->warning("SCHED: Attempt to assign multiple harq pids to the same user rnti=0x%x\n", user->get_rnti());
    return alloc_outcome_t::ERROR;
  }

  // Check if allocation would cause segmentation
  uint32_t            ue_cc_idx = user->get_active_cell_index(cc_cfg->enb_cc_idx).second;
  const dl_harq_proc& h         = user->get_dl_harq(pid, ue_cc_idx);
  if (h.is_empty()) {
    // It is newTx
    rbg_interval r = user->get_required_dl_rbgs(ue_cc_idx);
    if (r.start() > user_mask.count()) {
      log_h->warning("The number of RBGs allocated to rnti=0x%x will force segmentation\n", user->get_rnti());
      return alloc_outcome_t::NOF_RB_INVALID;
    }
  }

  // Check if there is space in the PUCCH for HARQ ACKs
  const sched_interface::ue_cfg_t& ue_cfg     = user->get_ue_cfg();
  bool                             has_scells = ue_cfg.supported_cc_list.size() > 1;
  if (has_scells and is_periodic_cqi_expected(ue_cfg, get_tti_tx_ul())) {
    bool has_pusch_grant = is_ul_alloc(user->get_rnti()) or cc_results->is_ul_alloc(user->get_rnti());
    if (not has_pusch_grant) {
      // Try to allocate small PUSCH grant, if there are no allocated PUSCH grants for this TTI yet
      prb_interval alloc = {};
      uint32_t     L     = user->get_required_prb_ul(ue_cc_idx, srslte::ceil_div(SRSLTE_UCI_CQI_CODED_PUCCH_B + 2, 8));
      tti_alloc.find_ul_alloc(L, &alloc);
      if (ue_cc_idx != 0 and (alloc.length() == 0 or not alloc_ul_user(user, alloc))) {
        // For SCells, if we can't allocate small PUSCH grant, abort DL allocation
        return alloc_outcome_t::PUCCH_COLLISION;
      }
    }
  }

  // Try to allocate RBGs and DCI
  alloc_outcome_t ret = tti_alloc.alloc_dl_data(user, user_mask);
  if (ret != alloc_outcome_t::SUCCESS) {
    return ret;
  }

  // Allocation Successful
  dl_alloc_t alloc;
  alloc.dci_idx   = tti_alloc.get_pdcch_grid().nof_allocs() - 1;
  alloc.rnti      = user->get_rnti();
  alloc.user_mask = user_mask;
  alloc.pid       = pid;
  data_allocs.push_back(alloc);

  return alloc_outcome_t::SUCCESS;
}

alloc_outcome_t sf_sched::alloc_ul(sched_ue* user, prb_interval alloc, ul_alloc_t::type_t alloc_type, int msg3_mcs)
{
  // Check whether user was already allocated
  if (is_ul_alloc(user->get_rnti())) {
    log_h->warning("SCHED: Attempt to assign multiple ul_harq_proc to the same user rnti=0x%x\n", user->get_rnti());
    return alloc_outcome_t::ERROR;
  }

  // Allocate RBGs and DCI space
  bool            needs_pdcch = alloc_type == ul_alloc_t::ADAPT_RETX or alloc_type == ul_alloc_t::NEWTX;
  alloc_outcome_t ret         = tti_alloc.alloc_ul_data(user, alloc, needs_pdcch);
  if (ret != alloc_outcome_t::SUCCESS) {
    return ret;
  }

  ul_alloc_t ul_alloc = {};
  ul_alloc.type       = alloc_type;
  ul_alloc.dci_idx    = tti_alloc.get_pdcch_grid().nof_allocs() - 1;
  ul_alloc.rnti       = user->get_rnti();
  ul_alloc.alloc      = alloc;
  ul_alloc.msg3_mcs   = msg3_mcs;
  ul_data_allocs.push_back(ul_alloc);

  return alloc_outcome_t::SUCCESS;
}

alloc_outcome_t sf_sched::alloc_ul_user(sched_ue* user, prb_interval alloc)
{
  // check whether adaptive/non-adaptive retx/newtx
  ul_alloc_t::type_t alloc_type;
  ul_harq_proc*      h = user->get_ul_harq(get_tti_tx_ul(), user->get_active_cell_index(cc_cfg->enb_cc_idx).second);
  bool               has_retx = h->has_pending_retx();
  if (has_retx) {
    prb_interval prev_alloc = h->get_alloc();
    if (prev_alloc == alloc) {
      alloc_type = ul_alloc_t::NOADAPT_RETX;
    } else {
      alloc_type = ul_alloc_t::ADAPT_RETX;
    }
  } else {
    alloc_type = ul_alloc_t::NEWTX;
  }

  return alloc_ul(user, alloc, alloc_type);
}

bool sf_sched::alloc_phich(sched_ue* user, sched_interface::ul_sched_res_t* ul_sf_result)
{
  using phich_t    = sched_interface::ul_sched_phich_t;
  auto& phich_list = ul_sf_result->phich[ul_sf_result->nof_phich_elems];

  auto p = user->get_active_cell_index(cc_cfg->enb_cc_idx);
  if (not p.first) {
    // user does not support this carrier
    return false;
  }
  uint32_t cell_index = p.second;

  ul_harq_proc* h = user->get_ul_harq(tti_params.tti_tx_ul, cell_index);

  /* Indicate PHICH acknowledgment if needed */
  if (h->has_pending_ack()) {
    phich_list.phich = h->get_pending_ack() ? phich_t::ACK : phich_t::NACK;
    phich_list.rnti  = user->get_rnti();
    log_h->debug("SCHED: Allocated PHICH for rnti=0x%x, value=%s\n",
                 user->get_rnti(),
                 phich_list.phich == phich_t::ACK ? "ACK" : "NACK");

    ul_sf_result->nof_phich_elems++;
    return true;
  }
  return false;
}

void sf_sched::set_bc_sched_result(const pdcch_grid_t::alloc_result_t& dci_result,
                                   sched_interface::dl_sched_res_t*    dl_result)
{
  for (const auto& bc_alloc : bc_allocs) {
    sched_interface::dl_sched_bc_t* bc = &dl_result->bc[dl_result->nof_bc_elems];

    // assign NCCE/L
    bc->dci.location = dci_result[bc_alloc.dci_idx]->dci_pos;

    /* Generate DCI format1A */
    prb_interval prb_range = prb_interval::rbgs_to_prbs(bc_alloc.rbg_range, cc_cfg->P);
    int          tbs       = generate_format1a(prb_range, bc_alloc.req_bytes, bc_alloc.rv, bc_alloc.rnti, &bc->dci);

    // Setup BC/Paging processes
    if (bc_alloc.alloc_type == alloc_type_t::DL_BC) {
      if (tbs <= (int)bc_alloc.req_bytes) {
        log_h->warning("SCHED: Error SIB%d, rbgs=(%d,%d), dci=(%d,%d), len=%d\n",
                       bc_alloc.sib_idx + 1,
                       bc_alloc.rbg_range.start(),
                       bc_alloc.rbg_range.stop(),
                       bc->dci.location.L,
                       bc->dci.location.ncce,
                       bc_alloc.req_bytes);
        continue;
      }

      // Setup BC process
      bc->index = bc_alloc.sib_idx;
      bc->type  = sched_interface::dl_sched_bc_t::BCCH;
      bc->tbs   = (uint32_t)bc_alloc.req_bytes;

      log_h->debug("SCHED: SIB%d, rbgs=(%d,%d), dci=(%d,%d), rv=%d, len=%d, period=%d, mcs=%d\n",
                   bc_alloc.sib_idx + 1,
                   bc_alloc.rbg_range.start(),
                   bc_alloc.rbg_range.stop(),
                   bc->dci.location.L,
                   bc->dci.location.ncce,
                   bc_alloc.rv,
                   bc_alloc.req_bytes,
                   cc_cfg->cfg.sibs[bc_alloc.sib_idx].period_rf,
                   bc->dci.tb[0].mcs_idx);
    } else {
      // Paging
      if (tbs <= 0) {
        log_h->warning("SCHED: Error Paging, rbgs=%s, dci=(%d,%d)\n",
                       bc_alloc.rbg_range.to_string().c_str(),
                       bc->dci.location.L,
                       bc->dci.location.ncce);
        continue;
      }

      // Setup Paging process
      bc->type = sched_interface::dl_sched_bc_t::PCCH;
      bc->tbs  = (uint32_t)tbs;

      log_h->info("SCHED: PCH, rbgs=%s, dci=(%d,%d), tbs=%d, mcs=%d\n",
                  bc_alloc.rbg_range.to_string().c_str(),
                  bc->dci.location.L,
                  bc->dci.location.ncce,
                  tbs,
                  bc->dci.tb[0].mcs_idx);
    }

    dl_result->nof_bc_elems++;
  }
}

void sf_sched::set_rar_sched_result(const pdcch_grid_t::alloc_result_t& dci_result,
                                    sched_interface::dl_sched_res_t*    dl_result)
{
  for (const auto& rar_alloc : rar_allocs) {
    sched_interface::dl_sched_rar_t* rar = &dl_result->rar[dl_result->nof_rar_elems];

    // Assign NCCE/L
    rar->dci.location = dci_result[rar_alloc.alloc_data.dci_idx]->dci_pos;

    /* Generate DCI format1A */
    prb_interval prb_range = prb_interval::rbgs_to_prbs(rar_alloc.alloc_data.rbg_range, cc_cfg->P);
    int tbs = generate_format1a(prb_range, rar_alloc.alloc_data.req_bytes, 0, rar_alloc.alloc_data.rnti, &rar->dci);
    if (tbs <= 0) {
      log_h->warning("SCHED: Error RAR, ra_rnti_idx=%d, rbgs=%s, dci=(%d,%d)\n",
                     rar_alloc.alloc_data.rnti,
                     rar_alloc.alloc_data.rbg_range.to_string().c_str(),
                     rar->dci.location.L,
                     rar->dci.location.ncce);
      continue;
    }

    // Setup RAR process
    rar->tbs        = rar_alloc.alloc_data.req_bytes;
    rar->nof_grants = rar_alloc.rar_grant.nof_grants;
    std::copy(&rar_alloc.rar_grant.msg3_grant[0], &rar_alloc.rar_grant.msg3_grant[rar->nof_grants], rar->msg3_grant);

    // Print RAR allocation result
    for (uint32_t i = 0; i < rar->nof_grants; ++i) {
      const auto& msg3_grant    = rar->msg3_grant[i];
      uint16_t    expected_rnti = msg3_grant.data.temp_crnti;
      log_h->info("SCHED: RAR, temp_crnti=0x%x, ra-rnti=%d, rbgs=%s, dci=(%d,%d), rar_grant_rba=%d, "
                  "rar_grant_mcs=%d\n",
                  expected_rnti,
                  rar_alloc.alloc_data.rnti,
                  rar_alloc.alloc_data.rbg_range.to_string().c_str(),
                  rar->dci.location.L,
                  rar->dci.location.ncce,
                  msg3_grant.grant.rba,
                  msg3_grant.grant.trunc_mcs);
    }

    dl_result->nof_rar_elems++;
  }
}

void sf_sched::set_dl_data_sched_result(const pdcch_grid_t::alloc_result_t& dci_result,
                                        sched_interface::dl_sched_res_t*    dl_result,
                                        sched_ue_list&                      ue_list)
{
  for (const auto& data_alloc : data_allocs) {
    sched_interface::dl_sched_data_t* data = &dl_result->data[dl_result->nof_data_elems];

    // Assign NCCE/L
    data->dci.location = dci_result[data_alloc.dci_idx]->dci_pos;

    // Generate DCI Format1/2/2A
    auto ue_it = ue_list.find(data_alloc.rnti);
    if (ue_it == ue_list.end()) {
      continue;
    }
    sched_ue*           user        = &ue_it->second;
    uint32_t            cell_index  = user->get_active_cell_index(cc_cfg->enb_cc_idx).second;
    uint32_t            data_before = user->get_pending_dl_new_data();
    const dl_harq_proc& dl_harq     = user->get_dl_harq(data_alloc.pid, cell_index);
    bool                is_newtx    = dl_harq.is_empty();

    int tbs = user->generate_dl_dci_format(
        data_alloc.pid, data, get_tti_tx_dl(), cell_index, tti_alloc.get_cfi(), data_alloc.user_mask);

    if (tbs <= 0) {
      log_h->warning("SCHED: DL %s failed rnti=0x%x, pid=%d, mask=%s, tbs=%d, buffer=%d\n",
                     is_newtx ? "tx" : "retx",
                     user->get_rnti(),
                     data_alloc.pid,
                     data_alloc.user_mask.to_hex().c_str(),
                     tbs,
                     user->get_pending_dl_new_data());
      continue;
    }

    // Print Resulting DL Allocation
    log_h->info("SCHED: DL %s rnti=0x%x, cc=%d, pid=%d, mask=0x%s, dci=(%d,%d), n_rtx=%d, tbs=%d, buffer=%d/%d\n",
                !is_newtx ? "retx" : "tx",
                user->get_rnti(),
                cc_cfg->enb_cc_idx,
                data_alloc.pid,
                data_alloc.user_mask.to_hex().c_str(),
                data->dci.location.L,
                data->dci.location.ncce,
                dl_harq.nof_retx(0) + dl_harq.nof_retx(1),
                tbs,
                data_before,
                user->get_pending_dl_new_data());

    dl_result->nof_data_elems++;
  }
}

//! Finds eNB CC Idex that currently holds UCI
uci_pusch_t is_uci_included(const sf_sched*        sf_sched,
                            const sf_sched_result& other_cc_results,
                            const sched_ue*        user,
                            uint32_t               current_enb_cc_idx)
{
  uci_pusch_t uci_alloc = UCI_PUSCH_NONE;

  if (not user->get_active_cell_index(current_enb_cc_idx).first) {
    return UCI_PUSCH_NONE;
  }

  // Check if UCI needs to be allocated
  const sched_interface::ue_cfg_t& ue_cfg = user->get_ue_cfg();
  for (uint32_t enbccidx = 0; enbccidx < other_cc_results.enb_cc_list.size() and uci_alloc != UCI_PUSCH_ACK_CQI;
       ++enbccidx) {
    auto p = user->get_active_cell_index(enbccidx);
    if (not p.first) {
      continue;
    }
    uint32_t ueccidx = p.second;

    // Check if CQI is pending for this CC
    const srslte_cqi_report_cfg_t& cqi_report = ue_cfg.supported_cc_list[ueccidx].dl_cfg.cqi_report;
    if (srslte_cqi_periodic_send(&cqi_report, sf_sched->get_tti_tx_ul(), SRSLTE_FDD)) {
      if (uci_alloc == UCI_PUSCH_ACK) {
        uci_alloc = UCI_PUSCH_ACK_CQI;
      } else {
        uci_alloc = UCI_PUSCH_CQI;
      }
    }

    // Check if DL alloc is pending
    bool needs_ack_uci = false;
    if (enbccidx == current_enb_cc_idx) {
      needs_ack_uci = sf_sched->is_dl_alloc(user->get_rnti());
    } else {
      auto& dl_result = other_cc_results.enb_cc_list[enbccidx].dl_sched_result;
      for (uint32_t j = 0; j < dl_result.nof_data_elems; ++j) {
        if (dl_result.data[j].dci.rnti == user->get_rnti()) {
          needs_ack_uci = true;
          break;
        }
      }
    }
    if (needs_ack_uci) {
      if (uci_alloc == UCI_PUSCH_CQI) {
        // Once we include ACK and CQI, stop the search
        uci_alloc = UCI_PUSCH_ACK_CQI;
      } else {
        uci_alloc = UCI_PUSCH_ACK;
      }
    }
  }
  if (uci_alloc == UCI_PUSCH_NONE) {
    return uci_alloc;
  }

  // If UL grant allocated in current carrier
  uint32_t ue_cc_idx      = other_cc_results.enb_cc_list.size();
  int      sel_enb_cc_idx = -1;
  if (sf_sched->is_ul_alloc(user->get_rnti())) {
    ue_cc_idx      = user->get_active_cell_index(current_enb_cc_idx).second;
    sel_enb_cc_idx = current_enb_cc_idx;
  }

  for (uint32_t enbccidx = 0; enbccidx < other_cc_results.enb_cc_list.size(); ++enbccidx) {
    for (uint32_t j = 0; j < other_cc_results.enb_cc_list[enbccidx].ul_sched_result.nof_dci_elems; ++j) {
      // Checks all the UL grants already allocated for the given rnti
      if (other_cc_results.enb_cc_list[enbccidx].ul_sched_result.pusch[j].dci.rnti == user->get_rnti()) {
        auto p = user->get_active_cell_index(enbccidx);
        // If the UE CC Idx is the lowest so far
        if (p.first and p.second < ue_cc_idx) {
          ue_cc_idx      = p.second;
          sel_enb_cc_idx = enbccidx;
        }
      }
    }
  }
  if (sel_enb_cc_idx == (int)current_enb_cc_idx) {
    return uci_alloc;
  } else {
    return UCI_PUSCH_NONE;
  }
}

void sf_sched::set_ul_sched_result(const pdcch_grid_t::alloc_result_t& dci_result,
                                   sched_interface::ul_sched_res_t*    ul_result,
                                   sched_ue_list&                      ue_list)
{
  /* Set UL data DCI locs and format */
  for (const auto& ul_alloc : ul_data_allocs) {
    sched_interface::ul_sched_data_t* pusch = &ul_result->pusch[ul_result->nof_dci_elems];

    auto ue_it = ue_list.find(ul_alloc.rnti);
    if (ue_it == ue_list.end()) {
      continue;
    }
    sched_ue* user       = &ue_it->second;
    uint32_t  cell_index = user->get_active_cell_index(cc_cfg->enb_cc_idx).second;

    srslte_dci_location_t cce_range = {0, 0};
    if (ul_alloc.needs_pdcch()) {
      cce_range = dci_result[ul_alloc.dci_idx]->dci_pos;
    }

    // If UCI is encoded in the current carrier
    uci_pusch_t uci_type = is_uci_included(this, *cc_results, user, cc_cfg->enb_cc_idx);

    /* Generate DCI Format1A */
    uint32_t pending_data_before = user->get_pending_ul_new_data(get_tti_tx_ul(), cell_index);
    int      tbs                 = user->generate_format0(pusch,
                                     get_tti_tx_ul(),
                                     cell_index,
                                     ul_alloc.alloc,
                                     ul_alloc.needs_pdcch(),
                                     cce_range,
                                     ul_alloc.msg3_mcs,
                                     uci_type);

    ul_harq_proc* h = user->get_ul_harq(get_tti_tx_ul(), cell_index);
    // Allow TBS=0 in case of UCI-only PUSCH
    if (tbs < 0 || (tbs == 0 && pusch->dci.tb.mcs_idx != 29)) {
      log_h->warning("SCHED: Error %s %s rnti=0x%x, pid=%d, dci=(%d,%d), prb=%s, bsr=%d\n",
                     ul_alloc.type == ul_alloc_t::MSG3 ? "Msg3" : "UL",
                     ul_alloc.is_retx() ? "retx" : "tx",
                     user->get_rnti(),
                     h->get_id(),
                     pusch->dci.location.L,
                     pusch->dci.location.ncce,
                     ul_alloc.alloc.to_string().c_str(),
                     user->get_pending_ul_new_data(get_tti_tx_ul(), cell_index));
      continue;
    }

    // Print Resulting UL Allocation
    log_h->info("SCHED: %s %s rnti=0x%x, cc=%d, pid=%d, dci=(%d,%d), prb=%s, n_rtx=%d, tbs=%d, bsr=%d (%d-%d)\n",
                ul_alloc.is_msg3() ? "Msg3" : "UL",
                ul_alloc.is_retx() ? "retx" : "tx",
                user->get_rnti(),
                cc_cfg->enb_cc_idx,
                h->get_id(),
                pusch->dci.location.L,
                pusch->dci.location.ncce,
                ul_alloc.alloc.to_string().c_str(),
                h->nof_retx(0),
                tbs,
                user->get_pending_ul_new_data(get_tti_tx_ul(), cell_index),
                pending_data_before,
                user->get_pending_ul_old_data(cell_index));

    pusch->current_tx_nb = h->nof_retx(0);

    ul_result->nof_dci_elems++;
  }
}

alloc_outcome_t sf_sched::alloc_msg3(sched_ue* user, const sched_interface::dl_sched_rar_grant_t& rargrant)
{
  // Derive PRBs from allocated RAR grants
  prb_interval msg3_alloc = prb_interval::riv_to_prbs(rargrant.grant.rba, cc_cfg->nof_prb());

  alloc_outcome_t ret = alloc_ul(user, msg3_alloc, sf_sched::ul_alloc_t::MSG3, rargrant.grant.trunc_mcs);
  if (not ret) {
    log_h->warning("SCHED: Could not allocate msg3 within %s\n", msg3_alloc.to_string().c_str());
  }
  return ret;
}

void sf_sched::generate_sched_results(sched_ue_list& ue_db)
{
  cc_sched_result* cc_result = cc_results->get_cc(cc_cfg->enb_cc_idx);

  /* Pick one of the possible DCI masks */
  pdcch_grid_t::alloc_result_t dci_result;
  //  tti_alloc.get_pdcch_grid().result_to_string();
  tti_alloc.get_pdcch_grid().get_allocs(&dci_result, &cc_result->pdcch_mask);

  /* Register final CFI */
  cc_result->dl_sched_result.cfi = tti_alloc.get_pdcch_grid().get_cfi();

  /* Generate DCI formats and fill sched_result structs */
  set_bc_sched_result(dci_result, &cc_result->dl_sched_result);

  set_rar_sched_result(dci_result, &cc_result->dl_sched_result);

  set_dl_data_sched_result(dci_result, &cc_result->dl_sched_result, ue_db);

  set_ul_sched_result(dci_result, &cc_result->ul_sched_result, ue_db);

  /* Store remaining sf_sched results for this TTI */
  cc_result->dl_mask    = tti_alloc.get_dl_mask();
  cc_result->ul_mask    = tti_alloc.get_ul_mask();
  cc_result->tti_params = tti_params;
}

uint32_t sf_sched::get_nof_ctrl_symbols() const
{
  return tti_alloc.get_cfi() + ((cc_cfg->cfg.cell.nof_prb <= 10) ? 1 : 0);
}

int sf_sched::generate_format1a(prb_interval     prb_range,
                                uint32_t         tbs_bytes,
                                uint32_t         rv,
                                uint16_t         rnti,
                                srslte_dci_dl_t* dci)
{
  /* Calculate I_tbs for this TBS */
  int tbs = tbs_bytes * 8;
  int i;
  int mcs = -1;
  for (i = 0; i < 27; i++) {
    if (srslte_ra_tbs_from_idx(i, 2) >= tbs) {
      dci->type2_alloc.n_prb1a = srslte_ra_type2_t::SRSLTE_RA_TYPE2_NPRB1A_2;
      mcs                      = i;
      tbs                      = srslte_ra_tbs_from_idx(i, 2);
      break;
    }
    if (srslte_ra_tbs_from_idx(i, 3) >= tbs) {
      dci->type2_alloc.n_prb1a = srslte_ra_type2_t::SRSLTE_RA_TYPE2_NPRB1A_3;
      mcs                      = i;
      tbs                      = srslte_ra_tbs_from_idx(i, 3);
      break;
    }
  }
  if (i == 28) {
    Error("Can't allocate Format 1A for TBS=%d\n", tbs);
    return -1;
  }

  Debug("ra_tbs=%d/%d, tbs_bytes=%d, tbs=%d, mcs=%d\n",
        srslte_ra_tbs_from_idx(mcs, 2),
        srslte_ra_tbs_from_idx(mcs, 3),
        tbs_bytes,
        tbs,
        mcs);

  dci->alloc_type       = SRSLTE_RA_ALLOC_TYPE2;
  dci->type2_alloc.mode = srslte_ra_type2_t::SRSLTE_RA_TYPE2_LOC;
  dci->type2_alloc.riv  = srslte_ra_type2_to_riv(prb_range.length(), prb_range.start(), cc_cfg->cfg.cell.nof_prb);
  dci->pid              = 0;
  dci->tb[0].mcs_idx    = mcs;
  dci->tb[0].rv         = rv;
  dci->format           = SRSLTE_DCI_FORMAT1A;
  dci->rnti             = rnti;
  dci->ue_cc_idx        = std::numeric_limits<uint32_t>::max();

  return tbs;
}

} // namespace srsenb
