# grpc_core
This project is a light-weight publisher-subscriber/service(Server-Client) communicate protocol based on gRPC. For cases having difficulty on installing ROS, or needy to combine control system to web page.

# preliminary
Install gRPC: https://grpc.io/docs/languages/cpp/quickstart/  

**Note:**
* The first step change to "export MY_INSTALL_DIR={your installation path}", e.g. "export MY_INSTALL_DIR=$HOME/corgi_ws/install".
* There is no need to reinstall cmake if it is version 3.13 or later.

***The following all use "$HOME/corgi_ws/install" to replace {your installation path}.***
# compile
```
$ cd grpc_core
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_PREFIX_PATH=$HOME/corgi_ws/install -DCMAKE_INSTALL_PREFIX=$HOME/corgi_ws/install
$ make -j16
$ make install
```

# local environment setting
These will write setting into your bash file.
```
$ echo export PATH=\$HOME/corgi_ws/install/bin:\$PATH >> ~/.bashrc
$ echo export CORE_LOCAL_IP="127.0.0.1" >> ~/.bashrc
$ echo export CORE_MASTER_ADDR="127.0.0.1:10010" >> ~/.bashrc
$ source ~/.bashrc
```
**"CORE_LOCAL_IP"** should be your local device IP, and **"CORE_MASTER_ADDR"** should be set to the same as master IP, i.e. IP of the device which runs **grpccore**.  
For example, if you have device A (**192.168.0.106**) and device B (**192.168.0.172**), and you run your core master node on device A on port **10010**. Then on each device A and B, the **"CORE_MASTER_ADDR"** should be **"192.168.0.106:10010"**, and **"CORE_LOCAL_IP"** on device A is **"192.168.0.106"**, **"CORE_LOCAL_IP"** on device B is **"192.168.0.172"**. 

# example compile
```
$ cd grpc_core/example/c++ 
$ mkdir build 
$ cd build 
$ cmake .. -DCMAKE_PREFIX_PATH=$HOME/corgi_ws/install
$ make -j16
```

# run example
### publisher & listener
```
$ grpccore      // terminal 1
$ ./NodeTestPub // terminal 2 (run in the build file of example/c++)
$ ./NodeTestSub // terminal 3 (run in the build file of example/c++)
```
This is the basic Publisher/Subscriber protocol, it support multiple subscribers subscribe to one topic, and also multiple publishers publish to a topic is legal but not recommended.
### Server & Client
```
$ grpccore                // terminal 1
$ ./NodeTestServiceServer // terminal 2 (run in the build file of example/c++)
$ ./NodeTestServiceClient // terminal 3 (run in the build file of example/c++)
```
This is the basic ServiceServer/Client protocol, if you launch multiple Server on one service, only the last one works functionally.

# Use self-defined message defined in grpc_core
Please refer to [grpc_node_test](https://github.com/kyle1548/grpc_node_test).
