To run in vs2017
install Windows Universal CRT SDK component to vs2017
Go to Project > Properties > General and change windows SDK version to 10.0.16299.0, it should work with any version of 10
For now SDL folder has both version 2.0.3 and 2.0.7 since without 2.0.3 there were issues with DECLSPEC not being recognised. Will need to look into more