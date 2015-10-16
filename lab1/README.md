## cs640lab1 - build your own smart switch with switchyard

### CS640 2015 Fall
### Team: [Yunhe Liu](https://github.com/yunheL), [Xuyi Ruan](https://github.com/ruanxuyi)

### [Lab1 Link: ] (http://pages.cs.wisc.edu/~sskausik08/640-assignments/)


## TODO: 

~~1. forward packet to know port mapped from the source address.~~  
~~2. implement code for learning new incoming packet - add (source address, port) to the forward table~~  
3. code up various test scripts for the smart switch.  
~~4. implemented code for `time_out` function~~  
~~5. to fix the issue of `time_out` function thread keep running, after return from main function.~~  



## Encountered Issues: 

### `.srpy` test file issue:

Set up environment with python3 and required packages. 

When ran command below: 

`./srpy.py -t -s examples/hubtests.srpy examples/myhub.py`

Encounted err such as: 

```
AttributeError: 'Scenario' object has no attribute 'support_files'
```

OR

```
AttributeError: 'IPv4Address' object has no attribute '__dict__'
```

**Solution**:  problem with the .srpy test file. 

Recompile the test case: 

`python3 ./srpy.py -c -s examples/hubtests.py`

then run: 

`./srpy.py -t -s examples/hubtests.srpy examples/myhub.py`

now everthing should work.


### how to stop new thread when main function return. 

### Solution: 
simply use the python buildin function to detect if thread for main function still alive: 

	`threading.main_thread().is_alive()` will do the job.


## References:

[switchyard's github pages]
(https://github.com/jsommers/switchyard/blob/master/examples/exercises/learning_switch/learning_switch.rst)

[time_out_thread_ref](http://stackoverflow.com/questions/11083349/python-threading-timer-stays-alive-after-calling-cancel-method)

[get_current_time code](http://stackoverflow.com/questions/5998245/get-current-time-in-milliseconds-in-python)


[Set up VirtualEnv](http://docs.python-guide.org/en/latest/dev/virtualenvs/)


