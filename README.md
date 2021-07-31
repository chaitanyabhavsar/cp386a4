# Assignment 4

## Project Title

CP386: Assignment 4 - Spring 2020

## Motivation

In this assignment, we will try to practice the concept of deadlock avoidance. We will write a multiple threaded program that implements the banker's algorithm. 

## Installation 

- Visual Studio Code
- Oracle VM Virtual Box
- GCC Compiler for C
- Cyberduck

## Features

Request Resources
- 'RQ' Command is used to request resources for customer. This command fails if the sequence is unsafe or the requested resources are not available.
<img src="./images/pic1.jpg/" width=400>

Release Resources
- 'RL' Command is used to release the resources currently being used by the customer. This command fails if the resources are not being used already.
<img src="./images/pic5.jpg/" width=400>
<img src="./images/Pic6.jpg/" width =200>

Status
- 'Status' Command will print arrays and matrices for available, maximum, allocated, and needed resources.
<img src="./images/pic2.jpg/" width=400>

Run
- 'Run' command will execute customers as threads in a safe sequence. Each thread requests the resources it needs, releases them, and allows next thread to run.
<img src="./images/pic3.jpg/" width=400>

Exit
- 'exit' command will exit the program

## Individual Contribution (Function Wise)

| Function | Chaitanya Bhavsar | Jiten Aylani | 
| ------------- | ------------- | ------------- |
| int main() |✔️ |  |
| int banker() |  | ✔️ |
| void currReCnt() |✔️ |  |
| int customCnt() |  | ✔️ |
| void neededCnt() |✔️| ✔️ |
| void *runTh() | ✔️ | ✔️ |


## Sample Test
*Please see images for screenshots

## Authors
- Chaitanya Bhavsar 
- Jiten Aylani

## Credits

## License 


