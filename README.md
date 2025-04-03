# yamcs_system_demo

Reference demonstration application of a YAMCS system with applications made with the [yamcs_link](https://github.com/rdx-aegse/yamcs_link) jumpstart library and fprime applications. 

## Prerequisites
 
TODO: add details
- Any linux based environment
- Docker
- Docker compose

## Architecture

![architecture diagram](/docs/architecture.jpg)

More details available on the [data flow for TM, TC and events at the packet level](/docs/architecture-tm,tc,events.jpg).

The YAMCS mission databases are generated as CSV files. Since YAMCS expects XLS files, in this demonstration the CSVs are compiled into XLS files by the yamcs_server application. 

## Getting started
### Running for the first time

```bash
git clone https://github.com/rdx-aegse/yamcs_system_demo
cd yamcs_system_demo
git submodule update --init --recursive --progress
chmod +x ./run.sh
./run.sh
```

The log window will eventually say YAMCS's web client will be accessible at (ip):(port) in any web browser. 
YAMCS studio can also be used instead of the web client. Both usages are out of scope. 
Note: there is a cooldown period for socket re-use of approximately one minute. If re-run too fast, the application will not spin up completely. 

### Experiment the demonstration

The demo spins up two YAMCS instances, accessible from the top left corner of the web client:
- In fprime_demo
  - In Telemetry/Parameters, demo/, most TM is visible especially the counter BD_Cycle
  - To see more TM, go in Commanding/Send a command, and send sendTlm/SET_LEVEL with argument 2.
  - Go in Events, you should see an event for emitting the command and another for successful execution
  - Go back in Telemetry/Parameters, demo/, you should see all TM but warnings, queue depths and errors, including memory and CPU usage;
- In yamcs_link_demo: same things to experiment, except that there is only one command made available with dummy arguments, and only one event which will be emitted when the command is received. 

### Updating

Later, at any point, to update the submodules, run the following at the root of yamcs_system_demo:
```bash
git pull
git submodule update --init --recursive --progress
```

## Navigating the repository

Each Docker container has its own folder, in which scripts/ contain code that support the containerisation, and src/ contains operational code to be demonstrated. Both are copied into the container and run from there. 
