#!/bin/bash

set -e

P_FREQ=2100000
E_FREQ=1500000

# P-core with the highest advertised maximum frequency.
# On your i7-13700H, CPU 4 belongs to a 5.0 GHz P-core.
BENCH_CPU=4

if [ -z "$1" ]; then
    exit 1
fi

TARGET_PROGRAM="$1"
shift

P_CORES=(0 1 2 3 4 5 6 7 8 9 10 11)
E_CORES=(12 13 14 15 16 17 18 19)

declare -A OLD_MIN
declare -A OLD_MAX
declare -A OLD_GOV

get_policy() {
    local cpu="$1"

    for policy in /sys/devices/system/cpu/cpufreq/policy*; do
        if grep -qw "$cpu" "$policy/affected_cpus"; then
            echo "$policy"
            return
        fi
    done

    exit 1
}

# Save current settings
for cpu in "${P_CORES[@]}" "${E_CORES[@]}"; do
    policy=$(get_policy "$cpu")

    OLD_MIN[$cpu]=$(cat "$policy/scaling_min_freq")
    OLD_MAX[$cpu]=$(cat "$policy/scaling_max_freq")
    OLD_GOV[$cpu]=$(cat "$policy/scaling_governor")
done

restore() {
    for cpu in "${P_CORES[@]}" "${E_CORES[@]}"; do
        policy=$(get_policy "$cpu")

        echo "${OLD_MAX[$cpu]}" | sudo tee \
            "$policy/scaling_max_freq" > /dev/null

        echo "${OLD_MIN[$cpu]}" | sudo tee \
            "$policy/scaling_min_freq" > /dev/null

        echo "${OLD_GOV[$cpu]}" | sudo tee \
            "$policy/scaling_governor" > /dev/null
    done
}

trap restore EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

# Lock P-cores to 2.1 GHz
for cpu in "${P_CORES[@]}"; do
    policy=$(get_policy "$cpu")

    echo "$P_FREQ" | sudo tee \
        "$policy/scaling_min_freq" > /dev/null

    echo "$P_FREQ" | sudo tee \
        "$policy/scaling_max_freq" > /dev/null
done

# Lock E-cores to 1.5 GHz
for cpu in "${E_CORES[@]}"; do
    policy=$(get_policy "$cpu")

    echo "$E_FREQ" | sudo tee \
        "$policy/scaling_min_freq" > /dev/null

    echo "$E_FREQ" | sudo tee \
        "$policy/scaling_max_freq" > /dev/null
done

# Run benchmark 20 times
sudo taskset -c "$BENCH_CPU" perf stat -r 20 \
    -e cycles,instructions,cache-references,cache-misses,\
L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses,\
branch-instructions,branch-misses \
    "$TARGET_PROGRAM" "$@"
