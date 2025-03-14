# yamcs_system_demo

Reference demonstration application of a YAMCS system with applications made with the [yamcs_link](https://github.com/rdx-aegse/yamcs_link) jumpstart library and fprime applications (TBD). 

## Prerequisites
 
TODO: add details
- Any linux based environment
- Docker
- Docker compose

## Architecture

TODO: add diagram

The YAMCS mission databases are generated as CSV files. Since YAMCS expects XLS files, in this demonstration the CSVs are compiled into a single XLS by the yamcs_server application. The server application waits for the CSV files to appear in their dedicated folder (in a shared volume between both containers), then proceeds to launching YAMCS. 

## Getting started

```bash
git clone --recurse-submodules https://github.com/rdx-aegse/yamcs_system_demo
cd yamcs_system_demo
chmod +x ./run.sh
./run.sh
```

The log window will eventually say YAMCS's web client will be accessible at localhost:8090 in any web browser. 
YAMCS studio can also be used instead of the web client. Both usages are out of scope. 

Then, to update the submodules:
```bash
git pull
git submodule update --init --recursive
```
## Navigating the repository

Each Docker container has its own folder, in which scripts/ contain code that support the containerisation, and src/ contains operational code to be demonstrated. Both are copied into the container and run from there. 
