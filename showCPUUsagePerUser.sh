RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color
echo -e "${RED}The CPU usage(2nd column) and memory usage(3rd column) of the currently login users are as follows,${NC}"

own=$(id -nu)
cpus=$(lscpu | grep "^CPU(s):" | awk '{print $2}')
normal_users=$(cut -d: -f1,3 /etc/passwd | egrep ':[0-9]{4}$' | cut -d: -f1)
proc_users=$(ps ax o user:20 | awk '{print $1}' | sort -u)
proc_normal_users=''
for pu in $proc_users; do
  for nu in $normal_users; do
    if [[ $pu == $nu ]]; then
      proc_normal_users=$proc_normal_users$pu" "
    fi
  done
done

# for user in $(who | awk '{print $1}' | sort -u)
for user in $proc_normal_users
do
  # print other user's CPU usage in parallel but skip own one because
  # spawning many processes will increase our CPU usage significantly
  if [ "$user" = "$own" ]; then continue; fi
  (top -b -n 1 -u "$user" | awk -v user=$user -v CPUS=$cpus 'NR>7 { sum += $9; } END { print user, sum, sum/CPUS; }') &
  # don't spawn too many processes in parallel
  sleep 0.05
done
wait

# print own CPU usage after all spawned processes completed
top -b -n 1 -u "$own" | awk -v user=$own -v CPUS=$cpus 'NR>7 { sum += $9; } END { print user, sum, sum/CPUS; }'

echo -e "${BLUE}In the 2nd column, 100 means 1 CPU is being used.${NC}"
echo -e "${RED}If someone is using this node, try other nodes please!${NC}"
