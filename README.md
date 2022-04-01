# Riset_UWB_DWM1000  
#### Difference between two library :  
>DW1000 :  
  - lancar buat 3 anchor 1 tag  
  - Jangkauan tu de moon  
  - data rate tinggi  

>DW1000Ng :  
  - lancar buat 3 anchor 1 tag  
  - data rate kurang cepat  
  - tambah interrupt (on progress)  
#### DW1000 Setup
>Param for Long Range between Anchor and Tag   
```
1. Switch to Channel 2  
2. Preamble code => 3
3. Set TX Power register (0x1E) to de maxx  
  --> Giv this register value of (0x1F1F1F1FL)  
```
>Param for High Data Rate  `
```
1. Set datarate => 6800kbps  
2. Set pulseFrequency => 16MHz  
3. Set preambleLength => 128  
4. Turn off fvcking feature => smartpower(false)  
5. Mode => LONGDATA_FAST_ACCURACY  
```
