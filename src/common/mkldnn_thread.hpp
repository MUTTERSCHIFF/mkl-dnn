/*******************************************************************************
* Copyright 2017-2018 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#ifndef MKLDNN_THREAD_HPP
#define MKLDNN_THREAD_HPP

#include "utils.hpp"

#if defined(_OPENMP)
#include <omp.h>
#else
inline int omp_get_max_threads() { return 1; }
inline int omp_get_num_threads() { return 1; }
inline int omp_get_thread_num() { return 0; }
inline int omp_in_parallel() { return 0; }
#endif

/* VisualStudio still support omp 2.0 */
#ifdef _MSC_VER
#define collapse(x)
#endif

namespace mkldnn {
namespace impl {

template <typename T, typename U>
inline void balance211(T n, U team, U tid, T &n_start, T &n_end) {
    T n_min = 1;
    T &n_my = n_end;
    if (team <= 1 || n == 0) {
        n_start = 0;
        n_my = n;
    } else if (n_min == 1) {
        // team = T1 + T2
        // n = T1*n1 + T2*n2  (n1 - n2 = 1)
        T n1 = utils::div_up(n, (T)team);
        T n2 = n1 - 1;
        T T1 = n - n2 * (T)team;
        n_my = (T)tid < T1 ? n1 : n2;
        n_start = (T)tid <= T1 ? tid * n1 : T1 * n1 + ((T)tid - T1) * n2;
    }

    n_end += n_start;
}

}
}

#endif

// vim: et ts=4 sw=4 cindent cino^=l0,\:0,N-s
