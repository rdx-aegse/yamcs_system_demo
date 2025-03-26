#!/bin/sh

#Build fprime deployment
. /fprime_app/fprime-venv/bin/activate
#pip install setuptools --break-system-packages #a bug in the requirements.txt of fprime, see https://github.com/nasa/fprime/issues/3101
printf "yes\nyes\nyes" | fprime-util purge
fprime-util generate
fprime-util build -j8

#Generate YAMCS mdb
python3 /fprime2yamcs/main.py --fprime-artifacts "/fprime_app/build-artifacts/Linux/demo/dict" --fprime-topology "/fprime_app/demo/Top" --yamcs-mdb "/mdb_shared"

#Run deployment
#chmod +x build-artifacts/Linux/Ref/bin/Ref
#./build-artifacts/Linux/Ref/bin/Ref -a "127.0.0.1" -p $PORT_TC #TODO: update name/path if needed, and add PORT_TM (potentially PORT_EV)