# Description


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
  Timer for Analysis output, default is 5 seconds

> -l  
 Print list of interfaces, default is first in `ifconfig`  
 but you can provide yours

> -d  
> App will not print output periodically 

>  -h  
> Print help message  