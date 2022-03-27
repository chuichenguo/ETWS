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

#include "srsue/hdr/metrics_stdout.h"

#include <float.h>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>

using namespace std;

namespace srsue {

char const* const prefixes[2][9] = {
    {
        "",
        "m",
        "u",
        "n",
        "p",
        "f",
        "a",
        "z",
        "y",
    },
    {
        "",
        "k",
        "M",
        "G",
        "T",
        "P",
        "E",
        "Z",
        "Y",
    },
};

void metrics_stdout::set_ue_handle(ue_metrics_interface* ue_)
{
  ue = ue_;
}

void metrics_stdout::toggle_print(bool b)
{
  do_print = b;
}

void metrics_stdout::print_table(const bool display_neighbours)
{
  if (display_neighbours) {
    cout << "--------Signal-------------Neighbour--DL-------------------------------------UL----------------------"
         << endl;
    cout << "cc pci  rsrp    pl    cfo  pci  rsrp  mcs   snr turbo  brate   bler   ta_us  mcs   buff  brate   bler"
         << endl;
  } else {
    cout << "--------Signal--------------DL-------------------------------------UL----------------------" << endl;
    cout << "cc pci  rsrp    pl    cfo   mcs   snr turbo  brate   bler   ta_us  mcs   buff  brate   bler" << endl;
  }
  table_has_neighbours = display_neighbours;
  n_reports            = 0;
}

void metrics_stdout::set_metrics(const ue_metrics_t& metrics, const uint32_t period_usec)
{
  if (ue == nullptr) {
    return;
  }

  // always print RF error
  if (metrics.rf.rf_error) {
    printf("RF status: O=%d, U=%d, L=%d\n", metrics.rf.rf_o, metrics.rf.rf_u, metrics.rf.rf_l);
  }

  if (!do_print) {
    return;
  }

  if (metrics.stack.rrc.state != RRC_STATE_CONNECTED) {
    cout << "--- disconnected ---" << endl;
    return;
  }

  bool display_neighbours = false;
  if (metrics.phy.nof_active_cc > 1) {
    display_neighbours = metrics.stack.rrc.neighbour_cells.size() > metrics.phy.nof_active_cc - 1;
  } else {
    display_neighbours = metrics.stack.rrc.neighbour_cells.size() > 0;
  }

  // print table header every 10 reports
  if (++n_reports > 10) {
    print_table(display_neighbours);
  }

  // also print table header if neighbours are added/removed in between
  if (display_neighbours != table_has_neighbours) {
    print_table(display_neighbours);
  }

  for (uint32_t r = 0; r < metrics.phy.nof_active_cc; r++) {
    cout << std::setw(2) << r;
    cout << std::setw(4) << metrics.phy.info[r].pci << std::setw(0);
    cout << float_to_string(metrics.phy.ch[r].rsrp, 2);
    cout << float_to_string(metrics.phy.ch[r].pathloss, 2);
    cout << float_to_eng_string(metrics.phy.sync[r].cfo, 2);

    // Find strongest neighbour for this EARFCN (cells are ordered)
    if (display_neighbours) {
      bool has_neighbour = false;
      for (auto& c : metrics.stack.rrc.neighbour_cells) {
        if (c.earfcn == metrics.phy.info[r].dl_earfcn && c.pci != metrics.phy.info[r].pci) {
          cout << std::setw(4) << c.pci << std::setw(0);
          cout << float_to_string(c.rsrp, 2);
          has_neighbour = true;
          break;
        }
      }
      if (!has_neighbour) {
        cout << "  n/a";
        cout << "  n/a";
      }
    }

    cout << float_to_string(metrics.phy.dl[r].mcs, 2);
    cout << float_to_string(metrics.phy.ch[r].sinr, 2);
    cout << float_to_string(metrics.phy.dl[r].turbo_iters, 2);

    cout << float_to_eng_string((float)metrics.stack.mac[r].rx_brate / (metrics.stack.mac[r].nof_tti * 1e-3), 2);
    if (metrics.stack.mac[r].rx_pkts > 0) {
      cout << float_to_string((float)100 * metrics.stack.mac[r].rx_errors / metrics.stack.mac[r].rx_pkts, 1) << "%";
    } else {
      cout << float_to_string(0, 1) << "%";
    }

    cout << float_to_string(metrics.phy.sync[r].ta_us, 2);

    cout << float_to_string(metrics.phy.ul[r].mcs, 2);
    cout << float_to_eng_string((float)metrics.stack.mac[r].ul_buffer, 2);
    cout << float_to_eng_string((float)metrics.stack.mac[r].tx_brate / (metrics.stack.mac[r].nof_tti * 1e-3), 2);
    if (metrics.stack.mac[r].tx_pkts > 0) {
      cout << float_to_string((float)100 * metrics.stack.mac[r].tx_errors / metrics.stack.mac[r].tx_pkts, 1) << "%";
    } else {
      cout << float_to_string(0, 1) << "%";
    }
    cout << endl;
  }

  if (metrics.rf.rf_error) {
    printf("RF status: O=%d, U=%d, L=%d\n", metrics.rf.rf_o, metrics.rf.rf_u, metrics.rf.rf_l);
  }
}

std::string metrics_stdout::float_to_string(float f, int digits)
{
  std::ostringstream os;
  const int          precision =
      SRSLTE_MIN((int)((f == 0.0f || f == 100.0f) ? digits - 1 : digits - log10f(fabsf(f)) - 2 * FLT_EPSILON), 3);
  os << std::setw(6) << std::fixed << std::setprecision(precision) << f;
  return os.str();
}

std::string metrics_stdout::float_to_eng_string(float f, int digits)
{
  const int degree = (f == 0.0) ? 0 : lrint(floor(log10f(fabs(f)) / 3));

  std::string factor;

  if (abs(degree) < 9) {
    if (degree < 0)
      factor = prefixes[0][abs(degree)];
    else
      factor = prefixes[1][abs(degree)];
  } else {
    return "failed";
  }

  const double scaled = f * pow(1000.0, -degree);
  if (degree != 0) {
    return float_to_string(scaled, digits) + factor;
  } else {
    return " " + float_to_string(scaled, digits) + factor;
  }
}

} // namespace srsue
