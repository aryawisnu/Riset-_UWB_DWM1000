# Riset_UWB_DW1000/DWM1000  
#### Difference between two library :  
>DW1000 :  
  - good for 3 anchor and 1 tag configuration
  - Long range up to 24 meter tu de moon
  - high sampling rate up to 60Hz 
  - position data calculation on tag
  - no safety feature when one of the anchor lost signal
>DW1000Ng :  
  - good for 3 anchor and 1 tag configuration
  - slow data rate because not using interrupt function
  - position data calculation in main anchor  
  - if there is one anchor that fail to connect, this program gonna loop back to de main anchor
#### DW1000 Setup
>Param for Long Range between Anchor and Tag   
```
1. Switch to Channel 2  
2. Preamble code => 3
3. Set TX Power register (0x1E) to de maxx  
  --> Giv this register value of (0x1F1F1F1FL)  
```
>Param for High Sampling Rate  
```
1. Set datarate => 6800kbps  
2. Set pulseFrequency => 16MHz  
3. Set preambleLength => 128  
4. Turn off fvcking feature => smartpower(false)  
5. Mode => LONGDATA_FAST_ACCURACY  
```
>To Do List 
- [x] Send position data to pixhawk using ROS (on progress)
- [ ] Send position data to pixhawk using NMEA
- [ ] Find calculation for RTLS using 4 anchor
- [x] Drone loiter using UWB position data (on progress)
- [ ] Search param for higher data rate
