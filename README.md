# Application development of car interconnected based on Ad Hoc Network Model 
    Through the study of practical problems, vehicle ad hoc networks and 
    multi-sensor data fusion technology and other related technology, 
    we design a smart car's motion control system and interconnected system 
    which based on ad hoc network model in this project.
    
    This smart car uses ARM9 processer as core and embedded Linux as operating system, 
    including two function modules' design: embedded system module and single-chip microcomputer control module,
    according to the characteristics of the hardware platform and embedded software,
    we design the control program of single-chip microcomputer , ARM9 processer and PC client.
    
    We finally realize the system which can transmit control command, 
    vide message and warning signal between car and car or car and PC.

Keywords: VANET /Intelligent Vehicle/ARM

# Top Design
    Hardware system mainly contains : Embedded System SOC, Data Storage, interface, motor, vehicle model and power manager.
    Software system use the Linux System and kernel driver.
    As follow shows the graph
![Graph](/image_for_readme/1.png)      
    
    802.11p standard top design graph
![Graph](/image_for_readme/2.png)
    
     Intelligent Vehicle work flow : 
     1. Smart Car get the information of distance through the sensor of ultrasound.
     2. Controlling the PWM module to set up the speed.
     3. SOC send the message to AMR core for broadcasting the traffic condition
     
     sensor -> SOC -> motor driver
                   <-> ARM Core
                   
## Hardware System Design
    Camera connect with ARM Core by USB, sensors connect with SOC by GPIO,
    between ARM Core and SOC, we use RS232
    
![Graph](/image_for_readme/3.png)
   
## Software System Design
    Based on Linux System
    
![Graph](/image_for_readme/4.png)

## Development Process
    Embedded System Software development process
    
![Graph](/image_for_readme/5.png)
    
 
