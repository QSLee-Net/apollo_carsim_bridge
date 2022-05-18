# $SRC_DIR: .proto所在的源目录
# --cpp_out: 生成c++代码
# $DST_DIR: 生成代码的目标目录
# xxx.proto: proto文件
# protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/xxx.proto

#!/usr/bin/env bash
TOP_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd -P)"
SRC_DIR=${TOP_DIR}/proto
DST_DIR=${TOP_DIR}/include
protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/*.proto
