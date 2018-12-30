rm -rf tanker
g++ iicdriver.cpp  main.cpp  motor_driver.cpp  pwmserv_driver.cpp  tanker.cpp -o tanker -lwiringPi
