#### XEngine [中文](https://github.com/LishaUnfoolish/XEngine/blob/master/README.md)|[英文](https://github.com/LishaUnfoolish/XEngine/blob/master/README_en.md)
###一个通用的框架，封控在家做来玩的玩具。


###注意:
  本项目提供一个docker环境方便测试使用,也能直接在Ubuntu上运行,但需要安装xmake和clang15+

###build:
1.  ./docker/scripts/dev_start.sh start   #启动和拉取docker镜像
2.  ./docker/scripts/dev_into.sh          #进入docker
3.  ./run.sh build_opt                    #进入docker内编译,如果不在docker内运行则需自行安装xmake

###run:
4.  ./run.sh run_unit_test           #运行测试用例,如果不在docker内运行则需自行安装xmake

#如果需要自己制作docker镜像可以使用[docker镜像制作工具](https://github.com/LishaUnfoolish/XEngine_docker_tools)制作自己的docker,然后将./docker/scripts/dev_start.sh脚本中的DEV="lixiaoxmm/xengine:v1.3.xmake_1668767811"修改为自己的源


###release note:<br>
1.graph框架,做的比较灵活,现在采用邻接矩阵来存储每个节点的信息,后续有时间会增加邻接表和链式前向星作为可选
模板还有可以实现各种图算法,有兴趣的同学也可以pr.(https://github.com/LishaUnfoolish/XEngine/tree/master/hpcs/graph)<br>
2.flow builder模块可配合graph做各种任务构建workflow。(https://github.com/LishaUnfoolish/XEngine/blob/master/hpcs/task/flow_builder.hpp)<br>
3.runner模块负责执行builder构建的workflow,现阶段通过feature做一步,后续会在task目录增加协程调度.(https://github.com/LishaUnfoolish/XEngine/blob/master/hpcs/task/runner.hpp)<br>
4.scheduler模块为协程池的调度器,目前只实现了一个简单的调度器,类似GMP的架构,后续会增加不同policy的调度器,比如:优先级调度器,时间片调度器,抢占式调度器等等,可通过编写Json配置文件配置.<br>
5.增加cuda环境和测试代码


