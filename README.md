# A Multicore Intermittent Operating System: Heterogeneity-aware Multicore Synchronization



<!-- ABOUT THE PROJECT -->
## Project Description
This project develops a multicore intermittent operating system upon FreeRTOS to facilitate programmers in developing applications on energy-harvesting multicore devices regardless of power stability, while exempting them from the responsibility of handling task concurrency and synchronization.

Intermittent systems enable batteryless devices to operate with energy harvesting by leveraging the complementary characteristics of volatile (VM) and non-volatile memory (NVM). Unfortunately, alternate and frequent accesses to heterogeneous memories for accumulative execution across power cycles can significantly hinder computation progress. We advocate that intermittent systems should move toward heterogeneous multicore architectures, so that core heterogeneity can be leveraged to mitigate the forward progress reduction caused by memory heterogeneity. To demonstrate the efficacy, a delegable and adaptive synchronization protocol is proposed to allow memory accesses to be delegated between cores and dynamically adapt to diverse memory access latency. Moreover, our design guarantees task serializability across multiple cores and maintains data consistency despite frequent power failures


## Implementaton Description

<p align="center">
  <img src="https://i.imgur.com/HrdU833.jpg" width="768" />
</p>

We integrated our heterogeneity-aware design into FreeRTOS, a real-time operating system supporting many kinds of commercial microcontrollers, to realize an intermittent-aware multi-core operating system. Two modules, namely a data manager and a recovery handler, were added on top of the memory manager and the task scheduler of FreeRTOS, with 1218 lines of C code scattered into 13 files. The data manager is responsible for data management, adaptive synchronization, and two-phase validation, while the recovery handler allows delegable commitment and instant system recovery. Our FreeRTOS-extended operating system is then deployed on a Cypress CY8CKIT-062-WiFi-BT device, which features heterogeneous dual cores and hybrid memories, to enable intermittent-aware multicore concurrency control.

The data manager maintains memory space used by data versions and copies in hybrid memories. To this end, the data manager uses the functions, pvPortMalloc() and vPortFree(), provided by the memory manager to allocate data and reclaim invalid data. A data structure is maintained in NVM to record the location and size of the consistent version of each data object, whereas the corresponding information of temporary copies and working versions are maintained in VM. To avoid data corruption, tasks must access data objects through our read, write, and commit operations, which replace the original implementations provided by FreeRTOS.

The recovery handler enables updates of consistent versions to be delayed and delegated between CPU cores. Once a commit operation is delayed, the data objects modified by the corresponding task and the delayed updates will be stored in a VM buffer, and they will be removed from the buffer after the operation is performed by some CPU core to actually update the consistent versions in NVM. Note that delegable commitment is orthogonal to how NVM updates are implemented, regardless of synchronous and asynchronous I/O. In our implementation, NVM updates are conducted asynchronously via a direct memory access (DMA) controller to reduce the CPU time wasted during data movement. However, the CPU still requires some time to interact with the DAM controller, and delegable commitment can transfer the wasted CPU time from the big core to the LITTLE core.

<!-- TABLE OF CONTENTS -->
## Table of Contents
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Setup and Build](#setup-and-build)
* [License](#license)
* [Contact](#contact)
<!--* [Contributing](#contributing)-->
<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

Here are the basic software and hardware you need to build the applications running on this intermittent OS. 

* [PSoC Creator](https://www.cypress.com/products/psoc-creator-integrated-design-environment-ide "link") (recommended versions: 4.3)
* [CY8CKIT-062-WiFi-BT](https://www.cypress.com/documentation/development-kitsboards/psoc-6-wifi-bt-pioneer-kit-cy8ckit-062-wifi-bt "link")

### Setup and Build

1. Download/clone this repository

2. Import this project to your workspace in PSoC Creator by the following steps:
  * start page -> Open Existing Project
  * search the directory of the downloaded project
  * select the project file, namely ``Heterogeneity_aware_intermittent_systems.cyprj``
  
3. Build and flash to your device

Now, the demo project is ready to go. Just launch the demo application by clicking the debug button. In PSoC Creator, you can trace how the design works step by step. You can also use the reset button on the board to simulate power failure and resumption and see logging outputs from your COM port to check the forward progress of tasks running on this dual-core device. 

## License

See the [LICENSE](https://github.com/meenchen/HAMIS_DEMO/blob/master/LICENSE) file for license rights and limitations
