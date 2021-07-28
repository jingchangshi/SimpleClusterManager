ps aux | grep SCM_Server | awk '{print $2}' | while read i; do kill -9 $i; done
cp /home/data/build/SCM_Server.service /etc/systemd/system/
systemctl enable SCM_Server
systemctl start SCM_Server
systemctl status SCM_Server

