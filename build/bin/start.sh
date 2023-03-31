#!/bin/bash
LOCALIP=`ifconfig lo | awk 'NR==2{print $2}'`

BINARY=""
IPADDRESS=""
PORT=""

usage() {
    echo "Usage:"
    echo "start.sh [-b BINARY] [-a IPADDRESS] [-p PORT] [-h HELP]"
    echo "Description:"
    echo "BINARY, the binary file, can be intend_server or intend_client."
    echo "        You can just specify them by coding \"server\" or \"client\""
    echo "IPADDRESS, the ip address."
    exit -1
}

while getopts 'b:a:p:h' OPT; do
    case $OPT in
        b) BINARY="$OPTARG";;
        a) IPADDRESS="$OPTARG"
           LOCALIP="$IPADDRESS";;
        p) PORT="$OPTARG";;
        h) usage;;
        ?) usage;;
    esac
done

if [ ! $BINARY ]
then
    echo "-b BINARY must be specified"
elif [ $BINARY = "server" ]
then
    echo "server is luanching..."
    ./intend_server $LOCALIP $PORT
elif [ $BINARY = "client" ]
then
    echo "client is luanching..."
    ./intend_client $LOCALIP $PORT
fi
