# Operating Systems Laboratory

This repository contains the complete laboratory work for the Operating Systems course, covering 10 comprehensive lab sessions focused on Linux system administration, kernel programming, process management, and system programming concepts.

## 📚 Overview

This laboratory course provides hands-on experience with:
- Linux system administration and command-line operations
- Kernel module development and kernel data structures
- Process communication and synchronization mechanisms
- Multi-threading and multi-processing programming
- System programming concepts using C language
- Operating system concepts implementation

## 🏗️ Project Structure

```
Operating Systems lab/
├── OSLab1/                    # Linux Introduction
│   ├── OSLab1.pdf            # Lab report
│   └── Presentation 1.pptx   # Presentation slides
├── OSLab2/                    # Command Line Scripting
│   ├── OSLab2.pdf            # Lab report
│   └── Presentation 2.pptx   # Presentation slides
├── OSLab3/                    # Kernel Module Programming
│   ├── OSLab3.pdf            # Lab report
│   ├── Presentation 3.pptx   # Presentation slides
│   ├── Presentation 3-part2.pdf
│   └── Code/                 # Source code modules
│       ├── HashTableModule/  # Hash table kernel module
│       ├── MultiTypeModule/  # Multi-type data module
│       ├── ProcessInfoModule/ # Process information module
│       └── StructBirthdayModule/ # Birthday structure module
├── OSLab4/                    # Inter-Process Communication
│   ├── OSLab4.pdf            # Lab report
│   ├── Presentation 4.pdf    # Presentation slides
│   └── Code/                 # IPC implementation
│       ├── server1.c         # Server implementation
│       ├── server2.c         # Alternative server
│       ├── client1.c         # Client implementation
│       └── client2.c         # Alternative client
├── OSLab5/                    # Multi-Process Programming
│   ├── OSLab5.pdf            # Lab report
│   ├── Presentation 5.pptx   # Presentation slides
│   └── Report_form.pdf       # Report template
├── OSLab6/                    # Multi-Threading Programming
│   ├── OSLab6.pdf            # Lab report
│   └── Report_form.pdf       # Report template
├── OSLab7/                    # Pipe Communication
│   ├── OSLab7.pdf            # Lab report
│   └── Presentation 7.pptx   # Presentation slides
├── OSLab8/                    # CPU Scheduling
│   └── OSLab8.pdf            # Lab report
├── OSLab9/                    # Semaphore Implementation
│   └── OSLab9.pdf            # Lab report
├── OSLab10/                   # Race Conditions & Deadlock
│   ├── OSLab10.pdf           # Lab report
│   └── deadlock_Presentation.pdf # Presentation slides
├── OS_LAB_InstructionManual.pdf  # Complete lab manual
├── how to setup Linux.pdf     # Linux setup guide
└── README.md                  # This file
```

## 🎯 Lab Objectives

### Lab 1: Linux Introduction
**Objective**: Familiarization with the Linux operating system environment
- Basic Linux commands and file system navigation
- User interface and system administration basics
- Understanding Linux architecture and philosophy

### Lab 2: Command Line Scripting
**Objective**: Learning command-line scripting and automation in Linux
- Shell scripting fundamentals
- Command-line automation techniques
- System administration tasks automation

### Lab 3: Kernel Module Programming & Data Structures
**Objective**: Understanding kernel module development and kernel data structures
- Writing and loading kernel modules
- Working with kernel data structures (hash tables, linked lists)
- Module compilation and system integration
- **Key Components**:
  - HashTableModule: Demonstrates kernel hashtable usage
  - MultiTypeModule: Shows multi-type data handling
  - ProcessInfoModule: Process information retrieval
  - StructBirthdayModule: Custom data structure implementation

### Lab 4: Inter-Process Communication
**Objective**: Learning inter-process communication methods
- **Shared Memory**: Memory-based communication between processes
- **Message Passing**: Socket-based communication
- **Implementation**: Client-server architecture with warehouse management system
- **Key Features**:
  - Product management (create, add, reduce, remove)
  - Real-time inventory tracking
  - Network communication protocols

### Lab 5: Multi-Process Programming
**Objective**: Multi-process programming and normal distribution visualization
- Process creation and management
- Process synchronization
- Data visualization and statistical analysis
- Normal distribution plotting

### Lab 6: Multi-Threading Programming
**Objective**: Multi-threading implementation and Prefix Sum calculation
- Thread creation and management
- Thread synchronization techniques
- Parallel algorithm implementation
- Performance optimization

### Lab 7: Pipe Communication
**Objective**: Understanding system call pipe implementation
- Pipe creation and usage
- Parent-child process communication
- Data flow management
- File descriptor handling

### Lab 8: CPU Scheduling
**Objective**: Implementation of CPU scheduling algorithms
- First-Come-First-Served (FCFS)
- Shortest Job First (SJF)
- Round Robin (RR)
- Priority scheduling
- Performance analysis and comparison

### Lab 9: Semaphore Implementation
**Objective**: Process scheduling for optimal resource utilization
- Semaphore concepts and implementation
- Resource management
- Deadlock prevention
- Process synchronization

### Lab 10: Race Conditions & Deadlock
**Objective**: Advanced multi-threading, race condition prevention, and deadlock avoidance
- Race condition identification and prevention
- Deadlock detection and avoidance algorithms
- Thread safety implementation
- Advanced synchronization techniques

## 🛠️ Technical Requirements

### Development Environment
- **Operating System**: Linux (Ubuntu recommended)
- **Virtual Environment**: VirtualBox or VMware for Windows users
- **Compiler**: GCC (GNU Compiler Collection)
- **Build Tools**: Make utility
- **Kernel Development**: Linux kernel headers and development tools

### Prerequisites
- Basic C programming knowledge
- Understanding of operating system concepts
- Familiarity with command-line interfaces
- Basic Linux system administration skills

## 📖 Documentation

### Main Resources
- **OS_LAB_InstructionManual.pdf**: Complete laboratory manual with detailed instructions
- **how to setup Linux.pdf**: Step-by-step guide for setting up Linux environment
- Individual lab reports (OSLab1.pdf through OSLab10.pdf)
- Presentation slides for most labs

### Code Documentation
- Well-commented source code in C
- Makefiles for building kernel modules
- Implementation examples for each concept
- Error handling and debugging information

## 🚀 Getting Started

### 1. Environment Setup
1. Install VirtualBox or VMware on Windows
2. Download Ubuntu Linux ISO
3. Follow the setup guide in `how to setup Linux.pdf`
4. Install development tools: `sudo apt-get install build-essential`

### 2. Kernel Module Development
```bash
cd OSLab3/Code/HashTableModule/
make
sudo insmod hashtable_module.ko
sudo rmmod hashtable_module
```

### 3. IPC Programs
```bash
cd OSLab4/Code/
gcc -o server server1.c
gcc -o client client1.c
./server &
./client
```

## 🔧 Building and Running

### Kernel Modules
```bash
make                    # Build the module
sudo insmod *.ko       # Load the module
sudo rmmod <module>    # Unload the module
dmesg                  # View kernel messages
```

### C Programs
```bash
gcc -o program program.c    # Compile
./program                   # Run
```

## 📊 Learning Outcomes

Upon completion of this laboratory course, students will have:

1. **Practical Linux Experience**: Hands-on experience with Linux system administration
2. **Kernel Programming Skills**: Ability to write and load kernel modules
3. **System Programming Knowledge**: Understanding of low-level system programming
4. **Process Management**: Expertise in process creation, communication, and synchronization
5. **Threading Concepts**: Multi-threading implementation and synchronization
6. **Algorithm Implementation**: Real-world implementation of OS algorithms
7. **Problem-Solving Skills**: Ability to identify and solve OS-related problems

## 🤝 Contributing

This repository contains educational materials. For improvements or corrections:
1. Review the existing code and documentation
2. Identify areas for enhancement
3. Submit suggestions through appropriate channels

## 📝 License

This project is for educational purposes. All materials are intended for academic use in Operating Systems courses.

## 👥 Authors

- **Course**: Operating Systems Laboratory
- **Institution**: Computer Engineering Department
- **Semester**: 6th Semester
- **Academic Year**: Operating Systems Lab Course

## 📞 Support

For questions or issues related to the laboratory work:
- Review the lab manual (`OS_LAB_InstructionManual.pdf`)
- Check individual lab reports for specific instructions
- Consult the setup guide (`how to setup Linux.pdf`)