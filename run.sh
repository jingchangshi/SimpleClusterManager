cd /home/jcshi/SimpleClusterManager
nohup ./server > /dev/null 2>&1 & echo $! > $(hostname).pid
