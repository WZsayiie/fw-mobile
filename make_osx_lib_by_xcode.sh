#!/bin/bash

set -e -u

cd `dirname $0`

### configuration begin
dst_dir="app_unity/Assets/Plugins/iOS"
dst_fil="library"

src_dir=""
src_dir="$src_dir library/BASIS/libbasis"
src_dir="$src_dir library/foundation/libfoundation"
src_dir="$src_dir library/subsystem/libsubsystem"

sh generate_headers.sh
### configutation end

src_fil="`find $src_dir -name '*.cpp'`"

mak_arg=""
mak_arg="$mak_arg -dynamiclib"
mak_arg="$mak_arg -std=c++14"
mak_arg="$mak_arg -IGENERATED_HEADERS"
mak_arg="$mak_arg -lc++"
mak_arg="$mak_arg $src_fil"

rm -rf   $dst_dir/$dst_fil.bundle
mkdir -p $dst_dir/$dst_fil.bundle/Contents/MacOS
clang -o $dst_dir/$dst_fil.bundle/Contents/MacOS/$dst_fil $mak_arg