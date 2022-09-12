# FOC Solutions
## 1. Underlying Solutions
Here I found some underlying solutions for FOC control, but now I cannot decided which method should I use to solve the problem.

### 1.1 Solutions from Fire based on stm32
The codes from Fire is a possible solution. The code is perfectly match the Board which I am using. But the problem is, how can I implement it as a haptic Knob. How is the solution looks like? Now the Fire provides the position loop control and now it can run normally. But now the problem is the Fire didn't provide any code to make the motor like a haptic knob. So I need to achieve it by my self. Maybe I need to rewrite the program logic and I really need time to finish it.

### 1.2 Simple FOC based on Arduino
This solution seems like more promising because there lots of example in the Internet as well as it is easy to implement to the STM development board. But the only thing is, I need to learn the Arduino repository, it will take some time. 

### 1.3 decision

Using the Fire repository to make a simple demo -> learn arduino + Plantform IO -> Try to implement it using the plantform IO --> connection with ESP32 and create the connection