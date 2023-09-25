#### XEngine [Chinese](https://github.com/LishaUnfoolish/XEngine/blob/master/README.md)|[English](https://github.com/LishaUnfoolish/XEngine/blob/master/README_en.md)
###XEngine is a general framework designed to be used as a toy at home.


###Note:
  This project provides a docker environment for convenient testing and usage, and can also run directly on Ubuntu by installing bazel and clang15+.

###build:
There is a build and run process:
1. use ./docker/scripts/dev_start.sh start to start and pull the Docker images;
2. enter the Docker environment with ./docker/scripts/dev_into.sh;
3. compile inside Docker with ./run.sh build_opt;

###run:
4. run unit tests with ./run.sh run_unit_test.

#If you would like to create your own docker image, you can use the [docker image creation tool](https://github.com/LishaUnfoolish/XEngine_docker_tools) to create your own docker. After that, you can modify the DEV="lixiaoxmm/xengine:v1.3.xmake_1668767811" in the script ./docker/scripts/dev_start.sh to your source.


###release note:<br>
1.The graph framework is flexible and currently uses an adjacency matrix to store information of each node. In the future, there will be options to add an adjacency list and chain forward star for more capabilities. Additionally, various graph algorithms can be implemented and interested developers can make pull requests (PRs).(https://github.com/LishaUnfoolish/XEngine/tree/master/hpcs/graph)<br>
2.The flow builder module can be used in conjunction with the graph module to construct workflows for various tasks.。(https://github.com/LishaUnfoolish/XEngine/blob/master/hpcs/task/flow_builder.hpp)<br>
3.The Runner module is responsible for executing workflows built by the Builder. Currently, a step is taken by feature and future coroutine scheduling will be added to the task directory..(https://github.com/LishaUnfoolish/XEngine/blob/master/hpcs/task/runner.hpp)<br>
4.The 'scheduler' module is the scheduler for the coroutine pool. Currently, only a simple scheduler has been implemented, which is similar to the GMP architecture. In the future, different policies such as priority scheduler, time slice scheduler, preemptive scheduler, etc. will be added. These can be configured by writing a JSON configuration file.<br>
5.Add CUDA opencv4 environment and test code


