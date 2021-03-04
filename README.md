# **H**eterogeneity-**a**ware Concurrency and Synchronization for **M**ulticore **I**ntermittent **S**ystems

<!-- ABOUT THE PROJECT -->
## Project Description

This project provides a demo of a heterogeneity-aware intermittent system which addresses concurrent task execution on multicore intermittent systems. To maximize forward progress, our design shows how heterogeneous multi-cores can help in improving the forward progress in resolving the latency problem in data access. In particular, a light-weighted synchronization mechanism is proposed to allow data accesses being realized over heterogeneous cores to adapt to different I/O latencies of volatile and non-volatilememory. The serialization of task executions is always guaranteed with the capability of an instantsystem recovery even when the system is interrupted by any power failure. Our proposed design isrealized in a FreeRTOS-extended intermittent operating system running over a Cypress platformfeaturing heterogeneous dual cores and hybrid memories.

## Implementaton Description
We realize the heterogeneity-aware design, particularlyadaptive synchronizationanddelegable task commitment, and integrate it into an intermittent operating system,which was built upon on FreeRTOS according to the failure-resilient design to enable intermittent-aware task concurrency on single-core devices. 
Our implementation comprises an additional 13 files and 996 lines of C code, among which 24 lines are scattered in 2 files belonging to the kernel. is realized on top of the task scheduler and memory manager of FreeRTOS. For portability across different platforms and operating systems, HAMIS is integrated with minimized kernel code modifications. Specifically, the implementation comprises 13 files and 1218 lines of C code, among which 34 lines are scattered in 2 files belonging to the kernel.
The data manager uses the functions, pvPortMalloc() and vPortFree(), provided by the memory manager of FreeRTOS to allocate data and reclaim invalid data, as well as the functions, vTaskSuspend() and vTaskResume(), provided by the scheduler of FreeRTOS to respectively remove tasks from and add tasks into the ready queue for adaptive synchronization. The recovery handler uses the function, xTaskCreate(), of the scheduler to recreate unfinished tasks.

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
