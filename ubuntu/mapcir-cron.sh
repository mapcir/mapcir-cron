#!/bin/bash -e

USER=root
NAME=mapcir-cron
DESC="Mapcir Cron service"
DIR={YOUR_PATH}
PIDFILE={YOUR_PATH}/$NAME.pid
DAEMON={YOUR_PATH}/mapcir-cron
DAEMON_COMMON_OPTIONS="--quiet --chuid $USER --pidfile $PIDFILE"
DAEMON_ARGS="--f={YOUR_PATH}/configs.json"
STOP_SIGNAL=INT
#LOG={YOUR_PATH}/logs/$NAME.log

#export PATH="${PATH:+$PATH:}/usr/sbin:/sbin"

test -x $DAEMON || exit 0

. /lib/lsb/init-functions

# Set the ulimit variable if you need defaults to change
ULIMIT="-n 4096"

if [ -z "$PIDFILE" ]; then
    PIDFILE=/run/$NAME.pid
fi

if [ -n "$ULIMIT" ]; then
    # Set ulimit if it is set in /etc/default/mapcir-cron
    ulimit $ULIMIT
fi

do_start(){
    start-stop-daemon --start $DAEMON_COMMON_OPTIONS --chdir $DIR --make-pidfile --background --startas \
        /bin/bash -- -c "exec $DAEMON $DAEMON_ARGS 2>&1"
}

do_stop(){
    opt=${@:-}
    start-stop-daemon --stop $DAEMON_COMMON_OPTIONS --signal $STOP_SIGNAL --oknodo $opt --remove-pidfile
}

do_status(){
    start-stop-daemon --status $DAEMON_COMMON_OPTIONS && exit_status=$? || exit_status=$?
    case "$exit_status" in
        0)
            log_daemon_msg "Program $NAME is running."
            ;;
        1)
            log_daemon_msg "Program $NAME is not running and the pid file exists."
            ;;
        3)
            log_daemon_msg "Program $NAME is not running."
            ;;
        4)
            log_daemon_msg "Unable to determine program $NAME status."
            ;;
    esac
}

case "$1" in
  status)
        do_status
        ;;
  start)
        log_daemon_msg "Starting $NAME"
        do_start
        ;;
  stop)
        log_daemon_msg "Stopping $NAME"
        do_stop
        ;;
  restart)
        log_daemon_msg "Stopping $NAME"
        do_stop --retry 30
        log_daemon_msg "Starting $NAME"
        do_start
        ;;
  *)
        echo "Usage: "$1" {status|start|stop|restart}"
        echo ""
        exit 1
esac

exit 0