/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include<interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    std::string address; //device address
    std::string duration; //duration of current ISR

    bool mode; //kernel mode



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
            //store duration, then keep it until we get an END_IO call
        //if activity is END IO, switch out of kernel mode and output the end duration

        if(activity == "CPU"){

        }else if(activity == "SYSCALL"){
            
        }else if(activity == "END_IO"){

        }else{
            //panic
        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
