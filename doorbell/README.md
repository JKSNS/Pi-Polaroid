# Doorbell

## Experience Summary

### How many hours did it take to complete assignment?
3 hours. 

### What did you enjoy about this lab?
I enjoyed making the secret menu feature. A very fun easter egg to wrap this project up, very fitting. 

### What were the major challenges you had with this lab? Try to be as detailed as possible.
The only major challenge that I had with this lab was making my main executable file run off boot. I must've misconfigured it initially which caused the program to fail. After realizing and recognizing my misconfiguration, I was able to fix my mistake and finish the lab. 

## Lab Specific Tasks

### systemd Daemons
1. What is a daemon?

A daemon is a background process that can run continuously on computer systems. 

2. What is `systemd`?

'systemd' is a service manager for linux systems. 

3. What program controls `systemd` daemons?

'systemctl' controls 'systemd' and its managed services. 

4. What kind of file is made to create a daemon in `systemd`?

In 'systemd', a '.service' file is made to create a daemon. 

5. Explain the difference between systemctl start and systemctl enable. When would you use one over the other?

'systemctl start' actually starts a service in systemd. This, however, only lasts until you turn the daemon off or shutdown your system. 'systemctl enable' on the other hand creates a symbolic link to start said daemon on every inital boot. 
Generally speaking, you would use 'systemctl start' during testing and development of new daemons. Then once you iron out said daemons, you would use 'systemctl enable' to make those daemons persistent across boots. 

6. Can you think of another real-world application where you might want a program to start automatically on boot? Describe the scenario.

Windows does a great job of automating this process for its users. In Windows, you can easily assign applications to run off boot: Spotify, Epic Games Launcher, VS Code, etc... Essentially, you can assign any application that you frequently use to run off boot, saving you time while logging back in after a reboot. 

Several other real-world examples where I would want the program to start automatically on boot include: 

1. My home security and automation systems. 
2. Personal VPN servers. 
3. File backup and integrity monitoring services. 

7. Do a little research on your own and list 3 different daemons that run by default in Linux. Why are those programs better as daemons than normal programs?

Three different daemons that run by default in Linux are: 

1. 'crond': Handles scheduled tasks, allowing for automation in your environment with zero user input necessary. 
2. 'syslogd': Handles general system logging, which otherwise would be incredibly overwhelming. This plays incredibly well with log aggregation tools. 
3. 'ntpd': Handles the systems network time protocol. This service works better as a daemon because it tracks your time zone, saving you time. 
