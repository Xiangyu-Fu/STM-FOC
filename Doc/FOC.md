# FOC field orientied control 

## 1. Six-step commutation of brushless DC motor
The commutation operation is triggered when the rotor is in a specific position. The commutation is passive commutation. If you want to increase the speed, you must increase the current, so that the magnetic field generated by the stator is stronger, so that the rotor can reach the target point faster and then trigger the commutation, as shown in the table below.

## 2. How to get the angle of the rotor?
We already know that we need to detect the angle first and then commutate, so how to detect the current angle? There are the following three ways. 
1. Calculate the current angle by installing the **encoder**. 
2. Calculate the current angle by installing the **Hall sensor**.
3. Calcualte the current angle **decting the current**.
   
### 2. 1 Encoder method to obtain the current angle of the motor
There are two types of encoders, incremetal encoder and absolute encoder. 

Increemtal encoder: A calibration need to be done before each startup, and in order to prever the pulse loss caused by the performence of micro controller, ther encoder needs to be calibrated once per revolution. Therefore, the ABZ three-axis encoder is often used, and the AB output the orthogonal signal, z axis output the interruption. 

Absolute encoder, only one calibration needs to be done before leaving the factory, afterwards, if there is not disassembly, no calibration required. The communication method are generally SPI and I2C, and the influence of the communication time on the system needs to be considered. Why we need to do the calibration to the encoder, because we cannot ensure that the 0 angle of the encoder just correspond to the 0 angle of the rotor (eletrical angle: in general the default electrical angle is 0)

### 2.2 Hall sensor method to obtain the angle of the motor

### 2.3 Detecting of the current to botain the angle of the motor

Because of the rotation, the motor will generate some wave. Then we can detect the current to determine which range of the current of the motor is. 

## 3. field orientied control 
![](https://pic1.zhimg.com/80/v2-b12a2650b001faa9cd31ecbb309eaca0_720w.jpg)
### 3.1 open loop of the FOC
We can review the process of learing brust direct current motor. First we need to rotate the motor and then we need to implement hte speed control ,and implement the position control. The same can be done during we learing FOC. First we ignore the position loop and the speed loop, then there is only leave the SVPMW and the part of current detection. Since we only need to make the rotor turn, the current detection is no more needed. We can directly give a target speed and target operation.

Compare to the six-step commutation method, the FOC will be more smooth and efficient.

### 3.2 introduction to IQ&ID
![](https://pic2.zhimg.com/80/v2-5ad65403fd235e0cf835825a887f7f65_720w.jpg)
looking closely, we can see that, it is ideal when the direction of the magnetic field generated by the three-winding is always tangential to the rotor magnet. In this way the rotational force is the largest which driving by the same current. When the direction of the magnetic field generated by three-winding is papendicular to the direction of the magnetic field , the rotor will be sucked in place, all of the current will be use to generate heat. So we set the rotor magnet as the reference, and then establish the coordinate DQ,  the DQ coordinate rotate with the rotor.

So how can we make the direction of the magnetic field generate by the winding be equivalent to the IQ and ID? Here we introduce the inverse Park transformation and the Clark inverse transformation.

#### 3.2.1 Inverse Park tranformation
![](https://pic1.zhimg.com/80/v2-1fe9b7613bf0e4e546faf82a34e33938_720w.jpg)
We know that the DQ coordinate system is a rotating coordinate system, but our three-phase windings are stationary, so the first task is to transform the rotating coordinate system into a stationary coordinate system, that is, to find a stationary coordinate system so that the magnetic field generated is equivalent to the DQ coordinate. , we name this coordinate system $????$ coordinate system, and the transformation process from DQ coordinate system to $????$ coordinate system is called **inverse Park transformation**.

$$
\begin{aligned}
U_?? = U_d\cos ?? - U_q \sin ??  \\
U_?? = U_q\cos ?? + U_d \sin ??    
\end{aligned}

$$
where the $\alpha$ axis is aligned with the phase a winding. 

After inverse Park transformation, we turn the rotating coordinate system into a stationary coordinate system, and the next step is to transform the stationary two-phase coordinate system to the 3-phase winding coordinate system.

#### 3.2.2 Inverse Clark Transformation
![](https://pic2.zhimg.com/80/v2-179c5210973a7cb6914add6761288a55_720w.jpg)
The purpose of Clark's inverse transformation is to *convert two vertical coordinate systems into three-phase winding coordinate systems*. After Clark inverse transformation, ID and IQ can be generated through three-phase windings. (Note that the reason why there is no Clark transformation in the FOC control block diagram is that only I?? and I?? are needed through the simplification of the following formula)



But the hardware we can finally control is the conduction of the three-phase full bridge, that is, the six MOS tubes, so what we ultimately want is the conduction time of the MOS tubes, that is, the high level time of the timer output, that is, the duty cycle of the PWM ratio, so we need to find the Mos conduction time through Ua Ub Uc next.


### 3.3 Introduction to 8 vectors
We know that our input is to control the conduction time of Ua, Ub, Uc, and the rotor angle. Our goal (output) is ID & IQ. The problem we want to solve is how to generate output through input. Before solving the problem, we must first take a look at what our input is and what the physical meaning is. Since the two MOSs on the upper and lower sides of the same bridge arm cannot be turned on (short-circuited) at the same time, there are 8 states in total for the 6 MOSs. Since each state generates a magnetic field in a fixed direction, we call the 8 states 8 kinds of vectors.

![](https://pic2.zhimg.com/80/v2-c62a6c4cca6d1e8eb24d203f768e17f5_720w.jpg)

(Note: Vector 0 & 7 are 0 vector which have no size and direction)
| vector| A phase | B phase | C phase| 
| ---- | ---- |  ---- |  ---- | 
|0|	0|	0|	0|
|4|	1|	0|	0|
6|	1|	1|	0|
2|	0|	1|	0|
3|	0|	1|	1|
1|	0|	0|	1|
5|	1|	0|	1|
7|	1|	1|	1|

Assuming that our motor is at the 0?? position, we only need to generate a 90?? vector to make the motor rotate, then we only need half of the time that vector 2 and vector 6 act in one cycle to generate a 90??. In summary, we can control the direction of the vector by controlling the conduction time ratio of adjacent vectors, and change the size of the vector by inserting a 0 vector, so we can generate any direction and a vector of any size. (As long as we can generate a vector in any direction, we can first read the current rotor angle ID, and then **generate a vector that is 90 degrees ahead**, which is the IQ).


We know that the range of the vector direction is 0-360 degrees, so what is the size of the vector? Since the three-phase winding resistance is basically the same, the maximum value of each vector is $U_{dc} \cdot \frac{2}{3}$, where $U_{dc}$ is the bus voltage (power supply voltage). But we found that the maximum value of the vector is different in all directions. In order to make the motor rotate smoothly, the maximum value of our target vector should be the radius of the inscribed circle of the hexagon in the above figure, that is, $\frac{2}{3}U_{dc}\cdot \frac{\sqrt{3}}{2} = \frac{U_{dc}}{\sqrt{3}}$. So the maximum value of the traget vector should be $\frac{U_{dc}}{\sqrt{3}}$. Refer to the figure below for details.
![](https://pic3.zhimg.com/80/v2-86fe67b96888a285705f938f0ce86a42_720w.jpg)

## 3.4 generation of SVPWM
Let's see a example, if we want to generate a random vector between 0-60 degree.

![](https://pic2.zhimg.com/80/v2-6b047a91d1ceeb89a36a0e72524ed6ad_720w.jpg)

By calculation, we can get the vector action duration of several other sectors. Since all the results are basically the same format, we assume: 
$$
\begin{aligned}
K &= \sqrt{3} * Ts / U_{dc} \\
U_1 &= U_?? \\
U_2 &= -\frac{\sqrt{3} }{2} * U_?? - \frac{U_??}{2} \\
U_3 &= \frac{\sqrt{3} }{2} * U_?? - \frac{U_??}{2} 
\end{aligned}
$$

We can conclude that
![image](https://user-images.githubusercontent.com/54738414/189078224-36ed92a6-7f6a-42bb-a94a-121304c8ebda.png)

After the above table is obtained, we can generate vectors of any size and any direction through 8 vectors. Through the table, it is found that the vectors used in different sectors are different, so we need to do sector judgment. As mentioned earlier, there are 3 ways to judge the sector, among which the current detection is explained here.

## 3.5 Judgement of the sector using Current detection
Look closely the inverse clark transformation and the U1, U2, U3 we obtained above, we can conclude that , the two formular set is quiet similar.

![](https://pic2.zhimg.com/80/v2-ba3f912502a89400ba9aaaccd7131375_720w.jpg)

From the image we can use the positive and negative value of the U1 U2 and U3 to judge the current sector. Aussme when $U1 > 0, A= 1$, otherwise $A = 0$.
When $U2>0
B=1$, otherwise $B=0$; set $U3>0$ when $C=1$, otherwise $C=0$; set $N=4C+2B+A$.


![image](https://user-images.githubusercontent.com/54738414/189080273-57659763-a6dc-4000-a4b6-008ca10762e4.png)

Now that we have obtained the action duration of each vector, we can find the on-time of each Mos, and the next step is to convert these on-times into PWM form.

## 3.6 Seven-segment SVPWM

We know that any one direction and size of the vector needs the role of the 8 vector market, then how to control the hardware circuit to achieve it, we are familiar with the hardware circuit control is PWM mode, so there is a way to replace the vector action time into the Mos conduction time? Of course you can, first we look at the MOS conduction time corresponding to each vector, and then by combining these times to form the PWM mode.

![](https://pic2.zhimg.com/80/v2-4bf2d93bddb187e8f7187a270fa22205_720w.jpg)

The next thing we have to do is to arrange and combine these vector signals into a PWM signal. For example, in the first sector we use the vector T0 T4 T6 T7. Because we want to decrese the number of the swithing of the MOS. 

![](https://pic2.zhimg.com/80/v2-641eec6a499610d0956a29728dc8e931_720w.jpg)

From the above figure, we can conclude that, in each of the swithing time, only one phase changed: 000->100 ->110 -> 111->110->100->000. This is so-called 7-segment SVPWM modulation method.

According to the same method we can analyze all of the PWM signal of all sector.

![](https://pic2.zhimg.com/80/v2-156c527153f9fa0aad26e8e9382217f1_720w.jpg)

![](https://pic2.zhimg.com/80/v2-ff81eadc68e1b627276d458e432c1371_720w.jpg)

![](https://pic2.zhimg.com/80/v2-148fcd070dd592b0ab476253d19f02e9_720w.jpg)

## 3.7 FOC current colse-loop

In fact, at this point, I think everyone should already understand the role of Clark transformation and Park transformation. Let's open our perspective and see what we just did in the open loop. 
1. Give the target current  
2. Control the three-phase full-bridge circuit to output the target current through the timer. (Don't worry about voltage and current, because there is only a coefficient between them) 

So if we want to close the loop, we only need to detect the actual current and make a PID closed loop. So why use Clark transformation and Park transformation, because if we do not use these transformations, then our target current and actual current are both sine waves, which is very unfavorable for our PID control, because the PID we are familiar with is the target value does not change, Let the actual value follow the target value. And the three-phase current Ia Ib Ic we got is not intuitive, I?? and I?? are not intuitive, but Iq and Id are more intuitive, Id is used for heating, and Iq is used for rotation. The closed-loop control of Iq and Id is easy to understand and can also cooperate with the speed loop, because if you want to turn fast, increase Iq.

### 3.7.1 Clark transfomation
Clark transformation: 3 phase -> 2 phase
![](https://pic1.zhimg.com/80/v2-c3b8a09a237a6f53524db6b5d6980b40_720w.jpg)

### 3.7.2 Park transformation
Park transformation: stationary -> rotation
![](https://pic3.zhimg.com/80/v2-365f1da0f5d98d62b534f9de36473982_720w.jpg)


## 3.8 current sampling
1. The phase current sampling can only be performed when the lower arm of the three-phase full bridge is open. 
2. Mos will interfere with the phase current at the moment of switching, so the sampling time avoids the moment when the Mos is switched on.  
3. The resistor can conduct current sampling at any time
4.  According to the SVPWM waveform, it is found that each sector has two-phase current changes with small changes, and one-phase current changes are large. Therefore, when three-resistor sampling is performed, two phases with small current changes should be collected. 
5.  When the target current is large, there may be a phase with a duty cycle close to 100%, which makes the current of this phase unable to be sampled. Therefore, the double-resistor sampling should limit the maximum output current and flow out the current sampling time. 
6.  A single resistor needs to sample the current twice in one cycle.

# 4. FOC close-loop control 
In this part, I integrate some content provide by Zhihui.
## 4.1 PID control 
![](https://pic4.zhimg.com/80/v2-3ec15f2e8ab0b97b7818e1d94d9dbd9b_720w.jpg)

There three mainly used PID control loop in the FOC control, from inner to the outer loop are: current loop, speed loop and positon loop. 

That means??? First we use the current feedback to control the current of moter(torque) $\rightarrow$  we control the rotate speed by controling the motor speed, $\rightarrow$  the position will be contorlled by the motor speed.

In actual use, since the encoder cannot directly return the motor speed, the motor speed can be represented by calculating the change in the encoded value within a certain period of time (that is, the average speed is used to represent the instantaneous speed). When the motor speed is relatively high, this method is possible; but in the position control mode, the motor speed will be very slow (because the rotor is required to be fixed at a certain position), at this time, the average speed measurement method will exist. Very large error (rotor does not move or moves very slowly, the encoder has no output or only 1 or 2 pulses).

## 4.2 Position control
In order to avoid the error resluted by speed loop, so we can only the two-loop control which combine the position and current control, but we need do some modification to the position loop, the block diagram looks as follows:
![](https://pic3.zhimg.com/80/v2-af17ac8465092f198e2f4f2ea8dad752_720w.jpg).

Since the speed loop is removed, here we use the full PIO control for the position loop. Compare to the above, we add the differential term, because the differential of the postion is speed, hence we can reduce the vibration of the position control and accelerate the convergence. (The intergal part is aim to eliminate the stationary error.)

There are Park transformation and corresponding inverse Park transformation in the entire control flow chart, but there is no inverse Clark transformation corresponding to Clark transformation. Instead, an SVPWM module is used.

# 5. summary
The principle of this FOC and the entire control chain have been explained, recall the whole process, and then try to answer the question mentioned at the beginning: why do so many transformations and inverse transformations in FOC control?



Because the so-called "vector control" is actually **decoupling**, decoupling the three-phase flux linkages that are coupled with each other into easily controllable quadrature axes $I_q$ and direct axes $I_d$. The whole process is like when we are doing signal processing, it is a reason to transform the signal into the frequency domain through FFT and then inversely transform it back to the time domain through IFFT.



It is also worth mentioning that this article introduces the sensory FOC control method. In fact, FOC can achieve non-inductive control (that is, no additional sensors such as encoders are required). Of course, the control algorithm will be more complicated, and it needs to be introduced before Feedback control, observer and other concepts, the advantage of insensitivity is that the structure installation is simpler, and the risk of position sensor failure can be avoided, etc. Of course, this is another topic.