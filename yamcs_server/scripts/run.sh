#!/bin/bash

#TODO: replace this with health checks at docker compose level
#Wait for yamcs_link to generate the mdbs. 6 CSVs per mission database, see yamcs_mdb_gen.py (TODO: parametrise this)
/scripts/waitforfiles.sh /mdb_shared 12 *.csv

#Convert all CSVs to XLS, since labyamcs apps will have filled it when this docker container is assembled
/scripts/mkmdb.sh /mdb_shared /mdb_shared

#Run YAMCS server with the generated XLS
cd /yamcs_cont/yamcs_repo
mvn yamcs:run

echo 'Exiting YAMCS server'

