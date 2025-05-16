# CMD_board
## project overview
CMD_board is a project that aims to use an MSP432P401R as an external controller to run some command directly on the PC. It is interfaced with the PC through a python script that sends and receive informations via serial communication.

## Requirements
### Hardware
- x Texas Instruments MSP432P401R microcontroller
- 1x Educational BoosterPack MKII

### Software
- Code Composer Studio Integrated Development Environment (IDE)
- MSP432 DriverLib
- python
- pyserial

```
├── esp                     # esp software
│   ├── Debug/
│   ├── LcdDriver/
│   ├── main.c
│   ├── msp432p401r.cmd
│   ├── startup_msp432p401r_ccs.c
│   ├── system_msp432p401r.c
│   ├── targetConfigs/
│   └── utils                   # support functions
│       ├── controls                # manages I/O
│       │   ├── controls.c
│       │   └── controls.h
│       ├── display                 # manages display 
│       │   ├── display.c
│       │   └── display.h
│       └── serial                  # manages serial communication
│           ├── printf.c
│           ├── printf.h
│           ├── serial.c
│           └── serial.h
├── python_backend          # host pc script
│   ├── configuration.json
│   ├── dev_list_1.txt
│   ├── main.py
├── readme.md
└── tree.txt
```
## build and run
1. clone the repository
``` bash
git clone https://github.com/Filippo831/embedded_project_unitn.git
```
2. import esp/ project in Code Composer Studio
3. Go to: Project → Properties → Build → Arm Compiler → Include Options and add the source directory located in simplelink_msp432p4_sdk_3_40_01_02/
4. Go to: Project → Properties → Build → Arm Linker → File Search Path and add:
    msp432p4xx_driverlib.lib directory located in [..]/source/ti/devices/msp432p4xx/driverlib/ccs/
    grlib.a located in [..]/source/ti/grlib/lib/ccs/m4f/
5. Build and run the project
6. Navigate to **python_backend/** directory
7. Download pySerial
``` bash
pip install pyserial
```
8. Configure **configuration.json** with desired commands.
9. Run main.py script
``` bash 
python main.py
```



## Usage
- connect the board to an USB port on PC
- identify the location the USB is mapped under the /dev folder
- change the location on the python script 
``` python 
SERIAL_PORT = "/dev/tty*"
```
- compile the configuration file with this format
``` json
{
  "commands": [
    ...
    {
      "label": "string to write on the ESP display",
      "cmd": "cmd to run on host machine"
    },
    ...
  ]
}
```
- run the python script
- from the esp select the command to run with the cursor
- push the button to run the command
