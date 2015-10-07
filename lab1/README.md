## cs640lab1 - build your own smart switch with switchyard

## CS640 2015 Fall
## Team: Yunhe Liu, Xuyi Ruan


## TODO: 


## Counter Issues: 

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



## References:

[switchyard's github pages]
(https://github.com/jsommers/switchyard/blob/master/examples/exercises/learning_switch/learning_switch.rst)


[Set up VirtualEnv](http://docs.python-guide.org/en/latest/dev/virtualenvs/)


