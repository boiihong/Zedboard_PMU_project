# Zedboard_PMU_project
1. Using Performance Monitoring Unit in Zedboard

2. Project Name: The research on hardware trojan detection, protection and countermeasures.

3. Funding Institution: South Korea National Institute of Scientific Research.

4. Duration: From March to October 2015.

5. My role: Implementing bare-metal program to monitor an processor performance like cache miss, branch predict miss, and cycle count etc. 

6. Detailed description: The program consists of two parts. One is a performance monitoring part which is running on Zedboard and the other is visualization part which is continuously displaying results. The first part is implemented with C/ASM and the second part is implemented with JAVA. Figure. 1 describes a flow of the running system. ①A target program to be monitored is executed. ②A preconfigured private timer interrupt halts the target program and send the PMU data to host pc through a serial interconnection. ③A final result is dynamically visualized by a java program running on x86 host pc.

7. Published paper: Minsu Kim, Sunhee Kong, Boeui Hong, Lei Xu, Weidong Shi and Taeweon Suh, "Evaluating Coherence-exploiting Hardware Trojan," In the Proceedings of the Design, Automation and Test in Europe (DATE), pp 157~162, Lausanne, Swizerland, March 2017.

8. directory

  -Host : Run on x86 pc

  -Board: Run on Zedboard

