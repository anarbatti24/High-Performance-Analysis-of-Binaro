#!/bin/bash

set -e

P_FREQ=2100000
E_FREQ=1500000

if [ -z "$1" ]; then
    echo "Error: No executable provided."
    echo "Usage: $0 ./your_program [program_arguments...]"
    exit 1
fi

TARGET_PROGRAM="$1"
shift

P_CORES=(0 1 2 3 4 5 6 7 8 9 10 11)
E_CORES=(12 13 14 15 16 17 18 19)

declare -A OLD_MIN
declare -A OLD_MAX
declare -A OLD_GOV

# ------------------------------------------------------------
# Find the cpufreq policy corresponding to a CPU
# ------------------------------------------------------------

get_policy() {
    local cpu="$1"

    for policy in /sys/devices/system/cpu/cpufreq/policy*; do
        if grep -qw "$cpu" "$policy/affected_cpus"; then
            echo "$policy"
            return
        fi
    done

    echo "ERROR: Could not find policy for CPU $cpu" >&2
    exit 1
}

# ------------------------------------------------------------
# Save current settings
# ------------------------------------------------------------

echo "[+] Saving current CPU settings..."

for cpu in "${P_CORES[@]}" "${E_CORES[@]}"; do
    policy=$(get_policy "$cpu")

    OLD_MIN[$cpu]=$(cat "$policy/scaling_min_freq")
    OLD_MAX[$cpu]=$(cat "$policy/scaling_max_freq")
    OLD_GOV[$cpu]=$(cat "$policy/scaling_governor")

    echo "    CPU $cpu -> $policy"
    echo "        min=${OLD_MIN[$cpu]}"
    echo "        max=${OLD_MAX[$cpu]}"
    echo "        governor=${OLD_GOV[$cpu]}"
done

# ------------------------------------------------------------
# Restore everything
# ------------------------------------------------------------

restore() {
    echo
    echo "[-] Restoring CPU settings..."

    for cpu in "${P_CORES[@]}" "${E_CORES[@]}"; do
        policy=$(get_policy "$cpu")

        # Restore max first
        echo "${OLD_MAX[$cpu]}" | sudo tee \
            "$policy/scaling_max_freq" > /dev/null

        # Restore min
        echo "${OLD_MIN[$cpu]}" | sudo tee \
            "$policy/scaling_min_freq" > /dev/null

        # Restore governor
        echo "${OLD_GOV[$cpu]}" | sudo tee \
            "$policy/scaling_governor" > /dev/null
    done

    echo "[+] CPU settings restored."
}


trap restore EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

# ------------------------------------------------------------
# Lock frequencies
# ------------------------------------------------------------

echo "[+] Locking P-cores to 2.1 GHz..."

for cpu in "${P_CORES[@]}"; do
    policy=$(get_policy "$cpu")

    echo "$P_FREQ" | sudo tee \
        "$policy/scaling_min_freq" > /dev/null

    echo "$P_FREQ" | sudo tee \
        "$policy/scaling_max_freq" > /dev/null
done

echo "[+] Locking E-cores to 1.5 GHz..."

for cpu in "${E_CORES[@]}"; do
    policy=$(get_policy "$cpu")

    echo "$E_FREQ" | sudo tee \
        "$policy/scaling_min_freq" > /dev/null

    echo "$E_FREQ" | sudo tee \
        "$policy/scaling_max_freq" > /dev/null
done

echo "[+] Frequencies locked."
echo "[+] Running: $TARGET_PROGRAM $*"
echo "------------------------------------------------"

# ------------------------------------------------------------
# Run benchmark
# ------------------------------------------------------------

sudo perf stat -d -d -d "$TARGET_PROGRAM" "$@"

echo "------------------------------------------------"
echo "[+] Benchmark finished."
