# Smart_Parking_Management_System

# 🚗 Smart Parking Management System

![C++](https://img.shields.io/badge/Language-C%2B%2B-blue)
![OOP](https://img.shields.io/badge/Concept-OOP-green)
![Status](https://img.shields.io/badge/Project-Completed-brightgreen)

---

## 📌 Overview

The **Smart Parking Management System** is a console-based application built using **C++** that simulates real-world parking operations.

It efficiently handles:
- Vehicle entry & exit
- Slot allocation
- Ticket management
- Parking duration & billing
- Data persistence using file handling

---


## 🎯 Key Features

- 🚘 *Automatic Slot Allocation*  
  Assigns the nearest available parking slot dynamically  

- 🎫 *Ticket-Based System*  
  Generates a unique Ticket ID for each parked vehicle  

- 🔍 *Vehicle Search*  
  Quickly find parked vehicles using vehicle number  

- 📊 *Active Vehicle Tracking*  
  Displays all currently parked vehicles  

- 💰 *Parking Cost Calculation*  
  Calculates charges based on parking duration  

- 💾 *Data Persistence*  
  Saves and restores parking data using file handling (fstream)  

---
## 🛠️ Tech Stack

- **Language:** C++  
- **Concepts:** OOP (Encapsulation, Abstraction)  
- **Data Structures:** `map`, `vector`  
- **File Handling:** `fstream`  
- **Time Handling:** `ctime`

---

## 🧠 System Design

The system is structured using three core classes:

- *Slot* → Represents each parking slot  
- *Ticket* → Stores vehicle entry details  
- *ParkingLot* → Core logic for managing parking operations  

Efficient mappings:
- vehicle → ticket
- ticket → details
- slot → vehicle

This ensures *O(1) lookup time complexity* for most operations.

---

## ▶️ How to Run

### 🔹 Compile
```bash
g++ main.cpp -o parking
