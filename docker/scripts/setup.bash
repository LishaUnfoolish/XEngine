ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../" && pwd -P)"

export PATH=/usr/local/cuda/bin
export PATH=$PATH:/usr/local/sbin
export PATH=$PATH:/usr/local/bin
export PATH=$PATH:/usr/sbin
export PATH=$PATH:/usr/bin
export PATH=$PATH:/sbin
export PATH=$PATH:/bin
export PATH=$PATH:/opt/emsdk
export PATH=$PATH:/opt/emsdk/upstream/emscripten
export PATH=$PATH:/opt/emsdk/node/14.15.5_64bit/bin


export PYTHON_BIN_PATH=/usr/bin/python3.10


export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins/xcbglintegrations


export EMSDK=/opt/emsdk
export EM_CONFIG=/opt/emsdk/.emscripten
export EMSDK_NODE=/opt/emsdk/node/12.9.1_64bit/bin/node
export EM_CACHE=$HOME/.emscripten_cache


export CUDA_DEVICE_ORDER=PCI_BUS_ID