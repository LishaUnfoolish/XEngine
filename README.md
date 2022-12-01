#### XEngine
###一个通用的框架,后续将会添加dds中间件和pipeline for graph模块.

注意:
  本项目提供一个docker环境,方便测试使用,也能直接在Ubuntu上运行,但需要安装xmake和clang15+
  
#运行
1.  ./docker/scripts/dev_start.sh start   #进入docker
2.  ./run.sh build_opt                    #进入docker内编译,如果不在docker内运行则需自行安装xmake
3.  ./run.sh run_hpcs_benchmark           #运行测试用例,如果不在docker内运行则需自行安装xmake

#如果需要自己制作docker镜像可以使用[docker镜像制作工具](https://github.com/LishaUnfoolish/XEngine_docker_tools)制作自己的docker,然后将./docker/scripts/dev_start.sh脚本中的DEV="lixiaoxmm/xengine:v1.3.xmake_1668767811"修改为自己的源
