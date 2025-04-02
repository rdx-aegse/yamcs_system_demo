# yamcs_system_demo

Reference demonstration application of a YAMCS system with applications made with the [yamcs_link](https://github.com/rdx-aegse/yamcs_link) jumpstart library and fprime applications (TBD). 

## Prerequisites
 
TODO: add details
- Any linux based environment
- Docker
- Docker compose

## Architecture

![architecture diagram](/docs/architecture.jpg)

The YAMCS mission databases are generated as CSV files. Since YAMCS expects XLS files, in this demonstration the CSVs are compiled into XLS files by the yamcs_server application. 

## Getting started

```bash
git clone https://github.com/rdx-aegse/yamcs_system_demo
cd yamcs_system_demo
git submodule init 
git submodule update --recursive --progress
chmod +x ./run.sh
./run.sh
```

Later, at any point, to update the submodules, run the following at the root of yamcs_system_demo:
```bash
git pull
git submodule update --init --recursive
```
## Navigating the repository

Each Docker container has its own folder, in which scripts/ contain code that support the containerisation, and src/ contains operational code to be demonstrated. Both are copied into the container and run from there. 