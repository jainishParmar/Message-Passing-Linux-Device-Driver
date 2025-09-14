# 📌 Message-Passing Linux Device Driver

A simple **Linux kernel module** that implements a **character device driver** for message passing between **user space** and **kernel space**.  
This project demonstrates how user applications can interact with the kernel through **system calls** (`open`, `read`, `write`, `release`) and provides a foundation for understanding **Linux device drivers**.  

---

## ✨ Features
- 🖥️ Character device driver implemented in **C**  
- 🔄 Supports **sending and receiving messages** between user-space programs and kernel space  
- 📡 Demonstrates **data passing across privilege boundaries** (user ↔ kernel)  
- 📂 Registered in `/dev` with **major/minor numbers**  
- ⚙️ Provides a basis for exploring **process synchronization and advanced driver concepts**  

---

## 📂 Project Structure
- ├── msgdriver.c # Kernel module source code
- ├── user_app.c # Example user-space program to test the driver
- ├── Makefile # Build rules for kernel module and user app
- └── README.md # Project documentation
  
---

## 📝 Learning Outcomes
- Understood **kernel ↔ user space communication**  
- Learned about **character devices and file operations**  
- Practiced **safe data transfer** between kernel and user space  

---




