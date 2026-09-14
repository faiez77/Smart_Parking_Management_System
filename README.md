# 🚗 Smart Parking Management System

![C++](https://img.shields.io/badge/Language-C%2B%2B-blue)
![OOP](https://img.shields.io/badge/Concept-OOP-green)
![Status](https://img.shields.io/badge/Project-Complete-brightgreen)

## 📌 Overview

A console-based parking management system in C++ that simulates real-world parking operations: vehicle entry/exit, slot allocation, ticketing, duration-based billing, and persistent storage across runs.

## 🎯 Key Features

- 🚘 **O(1) Slot Allocation** — assigns the next free slot instantly via a queue, not a scan
- 🎫 **Ticket-Based System** — unique ticket ID per parked vehicle
- 🔍 **Vehicle Search** — O(log n) lookup by vehicle number
- 📊 **Active Vehicle Tracking** — lists all currently parked vehicles
- 💰 **Duration-Based Billing** — ₹2/minute, rounded up, with a ₹10 minimum fare
- 💾 **Data Persistence** — parking state survives program restarts via file handling (`fstream`)
- ✅ **Input Validation** — non-numeric menu input is rejected and re-prompted, not left to hang or crash
- 🛡️ **Edge-case handling** — see [Edge Cases Handled](#️-edge-cases-handled) below

## 🛠️ Tech Stack

- **Language:** C++ (C++11 or later)
- **Concepts:** OOP (encapsulation), RAII-style file handling
- **Data structures:** `vector` (slot storage, direct-indexed), `queue` (free-slot allocation), `map` (ticket/vehicle lookup)
- **File handling:** `fstream`
- **Time handling:** `ctime`

## 🧠 System Design

Three core classes:

- **`Slot`** — represents a single parking slot (id + occupancy state)
- **`Ticket`** — vehicle entry record (ticket id, slot id, entry time, vehicle number)
- **`ParkingLot`** — core logic: allocation, billing, persistence

### ⚡ Time Complexity (corrected)

Slot ids are sequential (1 to n), so slot lookup and release use direct indexing (`slots[id-1]`) instead of scanning — this keeps allocation and release O(1):

| Operation | Complexity | How |
|---|---|---|
| Park Vehicle | **O(1)** | Pulls the next free slot id from a `queue` instead of scanning for one |
| Remove Vehicle | **O(1)** for slot release, **O(log n)** for the map erase | Slot ids are sequential, so `slots[id-1]` gives direct access; the `map` operations are the only log-n part |
| Search Vehicle | **O(log n)** | `std::map` lookup by vehicle number |
| Show Active Vehicles | **O(n)** | Must visit every active ticket |

## 💾 File Persistence

Parking state survives program restarts using plain-text file storage (`parking.txt`), written via `fstream`:

- **On every park/remove**, the full list of currently active tickets is rewritten to `parking.txt` — one line per active vehicle, in the format: `vehicleNumber slotId entryTimestamp ticketId`
- **On startup**, `ParkingLot`'s constructor reads `parking.txt` (if it exists) and reconstructs the in-memory state: which slots are occupied, active tickets, and the vehicle-to-ticket mapping — so the program resumes exactly where it left off
- **First run**, or if the file is missing, this is treated as an empty lot rather than an error
- **Corrupt or malformed lines** (e.g. an out-of-range slot id) are skipped individually with a warning, rather than aborting the whole load
- The ticket ID counter is restored to one past the highest ticket ID found on disk, so new tickets never collide with previously saved ones

## 🛡️ Edge Cases Handled

- **Invalid menu input** — typing a non-numeric choice (e.g. "abc") is caught and re-prompted instead of crashing or hanging the program
- **Duplicate vehicle** — parking a vehicle number that's already active is rejected with a clear message
- **Vehicle not found** — searching for or removing a vehicle not currently parked is handled gracefully, not a crash
- **Parking full** — once all slots are occupied, further park attempts are rejected with `Parking Full!`
- **Very short stays** — a vehicle parked and removed within the same minute is still billed a ₹10 minimum fare, not ₹0
- **Corrupt/partial save file** — if `parking.txt` has an invalid record (e.g. an out-of-range slot id), that line is skipped with a warning instead of crashing on load

## ▶️ How to Run

### Build
```bash
make
```
(or directly: `g++ -Wall -Wextra Parking_System.cpp -o Parking_System`)

### Run
```bash
./Parking_System
```

### Clean
```bash
make clean
```

## 🚀 Future Improvements

- 🌐 GUI or web interface
- 🧾 Historical parking log (beyond just currently-active tickets)
- 💳 Payment integration
- 🏢 Multi-floor parking support
- 📱 Mobile app support

## Author

Saiyed Faiez Husnain
Electrical Engineering, IIT Indore
