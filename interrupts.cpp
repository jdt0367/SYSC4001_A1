/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 * 
 * @author Jordan Trach
 * @author Josh Weitzel
 *
 */

#include"interrupts.hpp"
#include<cmath>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    std::string address; //interrupt numb
    std::string duration; //duration of current ISR
    int currTime = 0;
    int context = 10; //Duration of context save/restore
    int ISR = 0; //0 = read, 1 = write
    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        //if activity is CPU, get duration
        //if activity is SYSCALL, switch to kernel mode
            //get device vector table position and multiply by 2 to get "memory position"
            //save context
            //fetch address with vector table position
            //do something with ISR????
        //if activity is END IO, switch out of kernel mode and output the end duration
        //assume syscalls will alternate between reading and printing

        if(activity == "CPU"){
            execution += std::to_string(currTime) + ", " + std::to_string(duration_intr) + ", CPU burst\n";
            currTime += duration_intr;
        }else if(activity == "SYSCALL"){
            execution += std::to_string(currTime) + ", " + std::to_string(1) + ", Checking if interrupts are enabled\n";
            currTime++;
            execution += std::to_string(currTime) + ", " + std::to_string(1) + ", Checking device flag\n";
            currTime++;
            std::pair<std::string, int> boilerRtn = intr_boilerplate(currTime, duration_intr, context, vectors);
            currTime = boilerRtn.second;
            execution += boilerRtn.first;
            //execute
            if(ISR){
                //writing
                //set flag - printing in progress 10%
                execution += std::to_string(currTime) + ", " + std::to_string(round(delays[duration_intr] * 0.1)) + ", Set device flag\n";
                currTime += round(delays[duration_intr] * 0.1);
                //push data to buffer 30%
                execution += std::to_string(currTime) + ", " + std::to_string(round(delays[duration_intr] * 0.3)) + ", Pushing data from memory\n";
                currTime += round(delays[duration_intr] * 0.3);
                //call device driver 60%
                execution += std::to_string(currTime) + ", " + std::to_string(round(delays[duration_intr] * 0.6)) + ", Call device driver\n";
                currTime += round(delays[duration_intr] * 0.6);
                ISR = 0;
            } else {
                //reading
                //set flag - reading in progress 10%
                execution += std::to_string(currTime) + ", " + std::to_string(round(delays[duration_intr] * 0.1)) + ", Set device flag\n";
                currTime += round(delays[duration_intr] * 0.1);
                //call device driver 50%
                execution += std::to_string(currTime) + ", " + std::to_string(round(delays[duration_intr] * 0.5)) + ", Call device driver\n";
                currTime += round(delays[duration_intr] * 0.6);
                //read from buffer 40%
                execution += std::to_string(currTime) + ", " + std::to_string(round(delays[duration_intr] * 0.4)) + ", Reading data to memory\n";
                currTime += round(delays[duration_intr] * 0.3);
                ISR = 1;
            }
            currTime += delays[duration_intr];
        }else if(activity == "END_IO"){
            execution += std::to_string(currTime) + ", " + std::to_string(1) + ", IRET\n";
            currTime++;
            execution += std::to_string(currTime) + ", " + std::to_string(delays[duration_intr]) + ", Clearing flag for end of I/O " + std::to_string(duration_intr) + ": interrupt\n";
            currTime += delays[duration_intr];
        }
        /************************************************************************/
    }

    input_file.close();

    write_output(execution);

    return 0;
}
