#!/usr/bin/env bash

# Copyright 2021 ETH Zurich
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# SPDX-License-Identifier: Apache-2.0
# Author: Robert Balas (balasr@iis.ee.ethz.ch)

# var that points to this project's root
ROOT=$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")/.." && pwd)
export FREERTOS_PROJ_ROOT="$ROOT"
export FREERTOS_CONFIG_FAMILY="safety-island"

export CONFIG_TARGET="safety-island"

source "$ROOT/env/default-config.sh"

export CONFIG_FREERTOS_CHIP_INCLUDE=SafetyIsland_CV32E40P

export CONFIG_PLUSARG_SIM=y

export CONFIG_CLUSTER=n
export CONFIG_DRIVER_FLL=n
export CONFIG_DRIVER_CLKDIV=n
export CONFIG_DRIVER_CLKCONST=y
export CONFIG_DRIVER_INT=clic2
export CONFIG_SOC_EU=n
export CONFIG_GPIO=n
export CONFIG_PINMUX=n
export CONFIG_FC_EVENT=n
export CONFIG_STDIO=uartchs

