#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

mkdir -p "${EXPERIMENTS_PATH}/bin"

echo "Building line5c..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic "${EXPERIMENTS_PATH}/src/main.cpp" \
  -I "${BACKEND_PATH}/lib" -I "${EXPERIMENTS_PATH}/src" -I "${EXPERIMENTS_PATH}/src/lib" \
  -include "${EXPERIMENTS_PATH}/hpps/line5c.hpp" \
  -include "${EXPERIMENTS_PATH}/src/application/line5/application_line5.hpp" \
  -o "${EXPERIMENTS_PATH}/bin/line5c"

echo "Building line5s..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic "${EXPERIMENTS_PATH}/src/main.cpp" \
  -I "${BACKEND_PATH}/lib" -I "${EXPERIMENTS_PATH}/src" -I "${EXPERIMENTS_PATH}/src/lib" \
  -include "${EXPERIMENTS_PATH}/hpps/line5s.hpp" \
  -include "${EXPERIMENTS_PATH}/src/application/line5/application_line5.hpp" \
  -o "${EXPERIMENTS_PATH}/bin/line5s"

echo "Building line5m..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic "${EXPERIMENTS_PATH}/src/main.cpp" \
  -I "${BACKEND_PATH}/lib" -I "${EXPERIMENTS_PATH}/src" -I "${EXPERIMENTS_PATH}/src/lib" \
  -I "${EXPERIMENTS_PATH}/src/ring" -include "${EXPERIMENTS_PATH}/hpps/line5m.hpp" \
  -include "${EXPERIMENTS_PATH}/src/application/line5/application_line5.hpp" \
  -o "${EXPERIMENTS_PATH}/bin/line5m"

echo "Building line5gc..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic "${EXPERIMENTS_PATH}/src/main.cpp" \
  -I "${BACKEND_PATH}/lib" -I "${EXPERIMENTS_PATH}/src" -I "${EXPERIMENTS_PATH}/src/lib" \
  -include "${EXPERIMENTS_PATH}/hpps/line5gc.hpp" \
  -include "${EXPERIMENTS_PATH}/src/application/line5/application_line5.hpp" \
  -o "${EXPERIMENTS_PATH}/bin/line5gc"

echo "Building line5c_p..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic "${EXPERIMENTS_PATH}/src/main.cpp" \
  -I "${BACKEND_PATH}/lib" -I "${EXPERIMENTS_PATH}/src" -I "${EXPERIMENTS_PATH}/src/lib" \
  -include "${EXPERIMENTS_PATH}/hpps/line5c.hpp" \
  -include "${EXPERIMENTS_PATH}/src/application/line5/application_line5_p.hpp" \
  -o "${EXPERIMENTS_PATH}/bin/line5c_p"

echo "Building line5c_w..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic "${EXPERIMENTS_PATH}/src/main.cpp" \
  -I "${BACKEND_PATH}/lib" -I "${EXPERIMENTS_PATH}/src" -I "${EXPERIMENTS_PATH}/src/lib" \
  -include "${EXPERIMENTS_PATH}/hpps/line5c.hpp" \
  -include "${EXPERIMENTS_PATH}/src/application/line5/application_line5_w.hpp" \
  -o "${EXPERIMENTS_PATH}/bin/line5c_w"

echo "Building line5gc_w..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic "${EXPERIMENTS_PATH}/src/main.cpp" \
  -I "${BACKEND_PATH}/lib" -I "${EXPERIMENTS_PATH}/src" -I "${EXPERIMENTS_PATH}/src/lib" \
  -include "${EXPERIMENTS_PATH}/hpps/line5gc.hpp" \
  -include "${EXPERIMENTS_PATH}/src/application/line5/application_line5_w.hpp" \
  -o "${EXPERIMENTS_PATH}/bin/line5gc_w"

echo "Building dumbbell..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic "${EXPERIMENTS_PATH}/src/main.cpp" \
  -I "${BACKEND_PATH}/lib" -I "${EXPERIMENTS_PATH}/src" -I "${EXPERIMENTS_PATH}/src/lib" \
  -include "${EXPERIMENTS_PATH}/hpps/dumbbell.hpp" \
  -include "${EXPERIMENTS_PATH}/src/application/dumbbell/application_dumbbell.hpp" \
  -o "${EXPERIMENTS_PATH}/bin/dumbbell"

echo "Building line5sd..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic "${EXPERIMENTS_PATH}/src/main.cpp" \
  -I "${BACKEND_PATH}/lib" -I "${EXPERIMENTS_PATH}/src" -I "${EXPERIMENTS_PATH}/src/lib" \
  -include "${EXPERIMENTS_PATH}/hpps/line5s.hpp" \
  -include "${EXPERIMENTS_PATH}/src/application/line5/application_line5_d.hpp" \
  -o "${EXPERIMENTS_PATH}/bin/line5sd"

echo "Done."
