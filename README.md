                                                        USE VIM FOR BEST VISUAL FORMATTING


Mehta Harsh Hemantkumar
Computer Science And Engineering
Roll No:  10010177


networks
========

IITG Year 3 - Network Assignments

Installation:

    cd networks
    make

Running:

    * Step1: Run one or more servers as specifiedi below.

        cd server_$(server_name)_$(server_no)
        ./server_$(server_no)


                            $server_name    $server_no    Port 
    
        Iterative           iterative       1             9111
        Select              select          2             9222
        Multiprocessing     multiprocess    3             9333

    

    * Step2: Run the client (or any of the datasets) using the format specified in question
    
        cd client
        
        Method 1:

            ./client hostname port myname add date(MMDDYY) start_time(HHMM) end_time(HHMM)  Message(only one word without spaces)
        
            example:
            ./client 127.0.0.1 9222 harsh add 092313 0800 0900 birthday
        
        Method 2:
            sh dataset_$(server_name) // This has specific set of add commands which would get run 


    * Step3: Now we have added the data, now we can do any set of other operations like : get,getall,remove,update as format specified in the question statement


Comments:

    1. All Logical, Syntax and Semantic checks as specified in the question are implemented, they would come up as one starts checking for it.
    
    2. Code is structured and behaves as following

        client: 

            1. Parse Args
            2. Checks for date, time, no._of_args etc.
            3. Send string in proper format to server

        server:

            0. Check for events that has already happened for all_users and remove them.
            1. Call appropriate function call (ex. event_add() for add)
            2. Get the args, make meaning from it.
            3. Perform appropriate actions.
            4. Send back response.

    3. Multiprocessing (Not multi-threading) is used in the corresponding server using fork() system call. 


- Thank You !! 
