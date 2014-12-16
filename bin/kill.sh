proc_pid=$(ps -ef|grep ckpt-modeloff-bin | awk '{print $2}' | head -1)
echo "proc_pid = ${proc_pid}"


kill -9 ${proc_pid}
echo "${proc_pid} is killed"

:<<END
END
