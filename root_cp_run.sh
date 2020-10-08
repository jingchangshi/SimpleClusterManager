SCRIPT=$(readlink -f "$0")
SCRIPT_DIR=$(dirname "$SCRIPT")
cp ${SCRIPT_DIR}/SCM_Server /usr/local/bin/
nohup SCM_Server > /dev/null 2>&1 & echo $! > $HOME/SCM_Server_$(hostname).pid
