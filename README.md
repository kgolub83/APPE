# Acceleration pedal position encoder

Purpose of this model is to identify safety requirements and safety measures according to functional safety methodology and ISO26262 standard. For model purpose hypothetical example of electronic accelerator pedal position encoder as part of the vehicles electronic acceleration control and throttle control system is used (APPE from this point). 
The primary purpose of this model is to study and analyze the potential hazards that could result with failures impacting the functions of acceleration control systems where the throttle position is controlled electronically. Model focus will be on the safety software mechanisms and system architecture that can be used to achieve target ASIL (Automotive Safety Integrity Level) and minimize potential hazard risk. There are various implementation strategies to achieve target ASIL, this model should not be taken as guideline to achieve target ASIL, goal is to encourage critical thinking and studying regarding systems safety. The ISO 26262 standard provides a flexible framework and explicit guidance to different methods and approaches and various implementations may employ a variety of techniques to do so. Identification of the safety integrity requirements and implementation techniques of these functions are covered at the concept level and are not based on the actual system implementation.

The objective of the functional safety concept is to derive a set of functional safety requirements from the safety goals, and to allocate them to the preliminary architectural elements of the system, or to external measures (Part 3 Clause 8.1 in ISO 26262). 

## Hazard classification

For our model we will identify potential hazardous situations as follows:
- potential uncontrolled vehicle acceleration
- potential uncontrolled vehicle deceleration
- erroneous acceleration/deceleration response
- potential vehicle stalling

## Model description

Focus of models case study will be acceleration pedal position encoder, targeted for ASIL D applications. The Acceleration Pedal (AP) assembly allows the driver to command a desired torque. When the driver presses the AP, the AP Position Sensor (APPS) measures the angular position of the AP. The APPS converts the angular position of the AP to a voltage signal, processed and encoded by the processing unit – APPE and transmitted to the Engine Control Module (ECM). The signals are processed bay two independent processing units, sent over redundant communication channels to APPE supervisor unit, where the signal is validated and encoded using calibration tables (LUTs).

Framework model (Virtual processors, communication channels…) is implemented in C++,  application safety code and modules are implemented in C.

Although lock step processing (same code runs on two synchronized CPUs – hardware faults mitigation) is most popular system architecture, in this example we will take more conservative approach with dual asymmetric processing and arbitration unit (supervisor) in lock step operation.

Our modeled system architecture is based on two virtual processors (2 parallel threads) running different code while performing same function (acceleration pedal position decoder) and arbitration unit, implemented also as virtual processor (third parallel thread).


Two communication channels (between each processor and arbitration unit) implemented as circular FIFO buffers are modeled. 

Acceleration pedal position sensor (APPS) data is generated with matlab (octave) script and read as .csv input file.

File contains two data vectors containing 10bit ADC data for each complementary logarithmic position sensor.

For each sample one run cycle is executed in parallel for each processing unit. Task is to implement functional safety software for each processing unit. Single data point for every run cycle is provided to user program passed trough function interface (struct data pointer) called every run cycle.

Each processing unit processes data for both sensor channels using different implementation techniques and packs data to separate communication structures sent to arbitration unit.

Communication data is protected with data integrity checksum (CRC16 – CCITT polynomial used), signed with cryptography 32 bit SIP Hash signature, and encrypted with XTEA encryption algorithm.

Arbitration unit checks data integrity, compares results from both processing units, run system checks, decoding LUT tables integrity and provides acceleration position output data, switches to appropriate system state (or safe state) and writes system log.

## Faults

APPS communication fault – corrupted/missing data
APPS refresh rate fault, signal discontinued – watchdog fault
APPS Idle position fault 
APP value interpretation fault
APPE calibration LUT corrupted
APP value change fault – change over time out of bounds (step change)
APP range fault – guard region (to detect open, short hardware faults)
APPE processing fault – control algorithm, data inconsistency

## Safe states definitions

A safe state may be the intended operating mode, a degraded operating mode, or a switched off mode (Part 1 Clause 1.102 of ISO 26262). The developer of the functional safety concept attempts to maximize the availability of the item while ensuring the safety of the vehicle operation. Therefore, a careful consideration must be given to selecting the safe states in relation to the potential failure modes. 
The safe states for our APPE are either full operation (full torque availability), restricted operation (0 < Torque < Full), or switched off mode (zero torque).

APPE safe states:
- SafeState1 -> warningOperational
- SafeState2 -> warningRestrictive
- SafeState3 -> errorSpeedLimit
- SafeState4 -> fatalSafeStop

The acceleration pedal position maps (LUT) are to be checked: 
- one map corrupted -> SS1
- all redundant maps corrupted -> SS4 

The communications channels between the APPS and the ACS are to be checked
- packet lost/data corruption -> SS1 --comWarningIntegrator (2,1,2)
- communication timeout single channel -> SS2 --timeout 2 communication intervals (2,1,4)

In case of a failure of one redundant element, the ACS is to transition into Safe State
- determent fault -> SS2 -> one position sensor out of range, invalid value change, single processing fault
- uncertainty fault -> SS3 -> different parallel processing algorithm results

In case of a failure of one redundant element -> SS3

In case of a failure in this arbitration strategy, the ACS system is to
transition into Safe State -> SS3

All faults that result in a failure to determine the pedal position are to be detected. In
case of a failure in detecting the throttle position, the system is to transition into Safe -> SS4

## Usage

1. Install GNU Octave
2. Instal eog (eye of GNOME)
3. Run make
4. Run make exec

## License
[MIT](https://choosealicense.com/licenses/mit/)
 
