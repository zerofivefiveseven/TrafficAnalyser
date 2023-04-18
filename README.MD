# Description
This app aimed to http/s traffic analysis related to hostnames and network interfaces such as wlo0.

App can not recognize host from HTTPS/-secured and then get information automatically from previous layers such as ip layer or tcp layer This technique implemented both by myself and PcapPlusPlus lib.

Unstable version contains ./run ./idea for one tab config creation

### Install guide
- `sudo chmod +x ./configure.sh` 
- `sudo ./configure.sh` 
- `sudo ./Analyser` (*that command will start the app*)
#### OR
- Install dependencies 
- - build-essential cmake
- - libpcap-dev
- - libstdc++6
- set main CMakeLists.txt in Clion
- Edit Configuration > Enable Run with root privileges 

### Options: 
`./Analyser [-i interface] [-r calc_period]  [-hld]`

> -i [interface]  
>  Use the specified interface.   
> Can be interface name (e.g eth0) or interface IPv4 address

> -r [seconds]  
  Timer for temporary Analysis output, default is 5 seconds

> -l  
 Print list of interfaces, default is first in `ifconfig`  
 but you can provide your before start

> -d  
> App will not print temporary output  

>  -h  
> Print help message  
