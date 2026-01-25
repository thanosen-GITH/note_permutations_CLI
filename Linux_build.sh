docker run --rm -v "$PWD":/src -w /src ubuntu:24.04 \
  bash -lc "apt-get update && apt-get install -y build-essential cmake && cmake -S . -B build-linux && cmake --build build-linux"
  
