# kill -9 $(cat $HOME/SCM_Server_$(hostname).pid)
ps aux | grep SCM_Server | awk '{print $2}' | while read i; do kill -9 $i; done

