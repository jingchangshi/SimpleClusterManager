# Simple cluster manager

## SCM

Simple cluster manager (SCM) is a small C program to manage a really small cluster.
Supercomputer employs the job schedule system like slurm as the manager.
For a small cluster, there is no need for such system. On a small cluster, the user normally 1) submit a job using command like `mpirun -n 40 -host node4 ~/code/prog_run` 2) login to a compute node and then run it.
Under such circumstances, the user want to know which compute node is available. The plain approach is to login to a compute node and check if this node is running any computing job by `top`.
SCM is a small utility program reducing the efforts for the user to know which node is available.
SCM currently supports the following features

- Deploy the server on all the compute nodes and start the client on the master node
- Run the client on the master node to print the CPU loading of all compute nodes in the terminal
- Use pthread to send/receive sockets to/from all the compute nodes to reduce the communication time. 8 nodes cost almost no additional time. 1 second is spent on calculating the CPU usage by reading `/proc/stat` 2 times. The interval between 2 reading operations is 1 second. Maybe a better way of calculating the CPU loading could be done.

The output is as follows,

```
Simple Cluster Manager (SCM) shows CPU loading of compute nodes.
SCM is implemented by Jingchang Shi.
Go to https://github.com/desperadoshi/SimpleClusterManager/issues for issues.
It is possible that due to the slow network, you have to wait for max 2 seconds of timeout.
=== Explanations begin ===
CPU usage is in [0, 100%]
If CPU usage is about 85% on a node,
that node is probably running all its 40 CPU cores.
If CPU usage is less than 1% on a node,
that node is probably occupied by nobody.
=== Explanations end   ===
node1: Not connected!
node7: CPU usage = 100.00%
node5: CPU usage = 100.00%
node6: CPU usage = 100.00%
node2: CPU usage = 0.05%
node3: CPU usage = 0.07%
node4: CPU usage = 0.10%
node8: CPU usage = 0.07%
```

TODO for SCM

- Parse a configuration file instead of hard coding the configuration

## Shell script to implement the same feature

[scm_shownodeuser](scm_shownodeuser) implements the same feature.
For example, on our small cluster, there is a `master` and 7 compute nodes `node[2-8]`.
Thus run `scm_shownodeuser` gives the following output.

```
| Node No.      |       CPU usage       |       Users                    |
| node2         |       .5              |       User1                    |
| node3         |       2.1             |       User1 User2              |
| node4         |       4.3             |       User1 User4 User2        |
| node5         |       3884.7          |       User2                    |
| node6         |       3988.4          |       User3                    |
| node7         |       3892.8          |       User2                    |
| node8         |       .2              |       User1                    |
```


