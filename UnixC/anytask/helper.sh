if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters. Only 'start' and 'stop' options could be passed."
	exit 1
fi

SERVICE_TO_RUN=/home/tsimafeip/anytask/a.out
PID_FILE=/home/tsimafeip/anytask/pidfile

if [ "$1" = 'start' ]; then
	if [ -f "$PID_FILE" ]; then
		PID=$(<"$PID_FILE")
		if  ps -p $PID > /dev/null
		then
			echo "$SERVICE_TO_RUN is already running." 1>&2
		fi
	else
		echo "$SERVICE_TO_RUN will be launched on background."
		"$SERVICE_TO_RUN" &
	fi
elif [ "$1" = 'stop' ]; then
	if [ -f "$PID_FILE" ]; then
		PID=$(<"$PID_FILE")
		kill -s SIGTERM $PID
		counter=5
		while [ $counter -gt 0 ]
		do
			if ps -p $PID > /dev/null
			then
				sleep 1
				counter=$(( $counter - 1 ))
			else
				break
		    fi
		done
		
		if [ $counter -eq 0 ]
		then
			kill -s SIGKILL $PID
		fi		
	else
		echo "No pidfile to stop found. The process $SERVICE_TO_RUN has already stopped." 1>&2
	fi
else
	echo "Unknown command! Only 'start' and 'stop' options are available."
	exit 1
fi
