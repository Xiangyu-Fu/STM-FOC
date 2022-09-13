# Hardware
## 1. Baisc Structure
Simple FOC Library
1. **control board** ESP32 or STM32 (included by P-N-IHM03, https://www.conrad.de/de/p/stmicroelectronics-p-nucleo-ihm03-entwicklungsboard-1-st-2365225.html?hk=SEM&WT.mc_id=google_pla&gclid=Cj0KCQjwguGYBhDRARIsAHgRm4-5ctp2nojK1HZwCr5mb-pYhp9DgJgcxN4uNFmORbWfmHyfDrUdl_oaAsN1EALw_wcB#productDownloads)
2. driver board st (included by P-N-IHM03)
3. sensor (AS6500) 
   1. opical encoder : too high precision will limit the efficiency of FOC algorithm, https://www.mouser.de/ProductDetail/Broadcom-Avago/AEDB-9140-A14?qs=RuhU64sK2%252BuoGAn1MzeaaA%3D%3D
   2. magnetic encoder : consider this --> has already lots of successful examples and easy to mount
   https://www.mouser.de/ProductDetail/ams/AS5X47U-TS_EK_AB?qs=sGAEpiMZZMve4%2FbfQkoj%252BBDLPCj82ZLyYIPEtADg0FE%3D
4. gimbal motor (included by P-N-IHM03)
## 2. Arduino Structure based on ESP32/STM32
Hardware connection:
ESP32 -> STM32 -> Driver Shield -> BLDCM & Encoder


## 3. Current sensoring
ADC
- Inline Current Sence
- Low-Side Current Sense

## 4. Motion Control
### 4.1 moment control 