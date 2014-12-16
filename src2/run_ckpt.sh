./ckpt-modeloff-bin > /tmp/ckpt.log 2>&1 &
echo "ckpt-modeloff-bin is running"

proc_pid=$(ps -ef|grep ckpt-modeloff-bin | awk '{print $2}' | head -1)
echo "proc_pid = ${proc_pid}"


sleep 4

cr_checkpoint -p ${proc_pid}
echo "${proc_pid} has been checkpoint as ./context.${proc_pid}"

cr_checkpoint --term ${proc_pid}
echo "${proc_pid} is killed"

sleep 2

:<<END
END
