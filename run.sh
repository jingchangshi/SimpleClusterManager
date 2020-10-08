cd /home/jcshi/SimpleClusterManager
nohup ./SCM_Server > /dev/null 2>&1 & echo $! > $(hostname).pid
