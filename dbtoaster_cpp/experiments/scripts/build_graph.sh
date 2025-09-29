#!/bin/bash

CURRENT_SCRIPT=$(readlink -f $0)
CURRENT_PATH=$(dirname "${CURRENT_SCRIPT}")
source "${CURRENT_PATH}/common.sh"

RELEASE_PATH="${BACKEND_PATH}/ddbtoaster/release"

mkdir -p "${EXPERIMENTS_PATH}/bin"
cd "${BACKEND_PATH}"

echo "Building line5c..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++14 -pedantic "${RELEASE_PATH}/lib/dbt_c++/driver/main.cpp" \
  -I "${RELEASE_PATH}/lib/dbt_c++/lib/" \
  -I "${RELEASE_PATH}/lib/dbt_c++/driver/" \
  -include "${EXPERIMENTS_PATH}/hpps/line5c.hpp" \
  -include "${RELEASE_PATH}/lib/dbt_c++/driver/application/line5/application_line5.h" \
  -o "${EXPERIMENTS_PATH}/bin/line5c"

echo "Building line5s..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++14 -pedantic "${RELEASE_PATH}/lib/dbt_c++/driver/main.cpp" \
  -I "${RELEASE_PATH}/lib/dbt_c++/lib/" \
  -I "${RELEASE_PATH}/lib/dbt_c++/driver/" \
  -include "${EXPERIMENTS_PATH}/hpps/line5s.hpp" \
  -include "${RELEASE_PATH}/lib/dbt_c++/driver/application/line5/application_line5s.h" \
  -o "${EXPERIMENTS_PATH}/bin/line5s"

echo "Building line5cd..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++14 -pedantic "${RELEASE_PATH}/lib/dbt_c++/driver/main.cpp" \
  -I "${RELEASE_PATH}/lib/dbt_c++/lib/" \
  -I "${RELEASE_PATH}/lib/dbt_c++/driver/" \
  -include "${EXPERIMENTS_PATH}/hpps/line5cd.hpp" \
  -include "${RELEASE_PATH}/lib/dbt_c++/driver/application/line5/application_line5.h" \
  -o "${EXPERIMENTS_PATH}/bin/line5cd"

echo "Building line5gc..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++14 -pedantic "${RELEASE_PATH}/lib/dbt_c++/driver/main.cpp" \
  -I "${RELEASE_PATH}/lib/dbt_c++/lib/" \
  -I "${RELEASE_PATH}/lib/dbt_c++/driver/" \
  -include "${EXPERIMENTS_PATH}/hpps/line5gc.hpp" \
  -include "${RELEASE_PATH}/lib/dbt_c++/driver/application/line5/application_line5.h" \
  -o "${EXPERIMENTS_PATH}/bin/line5gc"

echo "Building line5c_p..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++14 -pedantic "${RELEASE_PATH}/lib/dbt_c++/driver/main.cpp" \
  -I "${RELEASE_PATH}/lib/dbt_c++/lib/" \
  -I "${RELEASE_PATH}/lib/dbt_c++/driver/" \
  -include "${EXPERIMENTS_PATH}/hpps/line5c.hpp" \
  -include "${RELEASE_PATH}/lib/dbt_c++/driver/application/line5/application_line5_p.h" \
  -o "${EXPERIMENTS_PATH}/bin/line5c_p"

echo "Building line5c_w..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++14 -pedantic "${RELEASE_PATH}/lib/dbt_c++/driver/main.cpp" \
  -I "${RELEASE_PATH}/lib/dbt_c++/lib/" \
  -I "${RELEASE_PATH}/lib/dbt_c++/driver/" \
  -include "${EXPERIMENTS_PATH}/hpps/line5c.hpp" \
  -include "${RELEASE_PATH}/lib/dbt_c++/driver/application/line5/application_line5_w.h" \
  -o "${EXPERIMENTS_PATH}/bin/line5c_w"

echo "Building line5gc_w..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++14 -pedantic "${RELEASE_PATH}/lib/dbt_c++/driver/main.cpp" \
  -I "${RELEASE_PATH}/lib/dbt_c++/lib/" \
  -I "${RELEASE_PATH}/lib/dbt_c++/driver/" \
  -include "${EXPERIMENTS_PATH}/hpps/line5gc.hpp" \
  -include "${RELEASE_PATH}/lib/dbt_c++/driver/application/line5/application_line5_w.h" \
  -o "${EXPERIMENTS_PATH}/bin/line5gc_w"

echo "Building dumbbell..."
g++ -O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++14 -pedantic "${RELEASE_PATH}/lib/dbt_c++/driver/main.cpp" \
  -I "${RELEASE_PATH}/lib/dbt_c++/lib/" \
  -I "${RELEASE_PATH}/lib/dbt_c++/driver/" \
  -include "${EXPERIMENTS_PATH}/hpps/dumbbell.hpp" \
  -include "${RELEASE_PATH}/lib/dbt_c++/driver/application/dumbbell/application_dumbbell.h" \
  -o "${EXPERIMENTS_PATH}/bin/dumbbell"

echo "Done."
