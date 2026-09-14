# 🚗 Smart Parking Management System

![C++](https://img.shields.io/badge/Language-C%2B%2B17-blue)
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
- 🧪 **Edge-case test suite** — see [Testing](#-testing) below

## 🛠️ Tech Stack

- **Language:** C++17
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

The original version of this project claimed `O(log n)` for slot removal, but the implementation actually did a **linear scan** through the slot list to free a slot — a real discrepancy between the documented and actual complexity. This version fixes both the complexity *and* the documentation to match:

| Operation | Complexity | How |
|---|---|---|
| Park Vehicle | **O(1)** | Pulls the next free slot id from a `queue` instead of scanning for one |
| Remove Vehicle | **O(1)** for slot release, **O(log n)** for the map erase | Slot ids are sequential, so `slots[id-1]` gives direct access; the `map` operations are the only log-n part |
| Search Vehicle | **O(log n)** | `std::map` lookup by vehicle number |
| Show Active Vehicles | **O(n)** | Must visit every active ticket |

## 🔧 What changed from the original version

This started as a working but unpolished console app. Fixes made:

1. **Input validation** — the original used `cin >> choice` with no guard; non-numeric input put `cin` into a permanent fail state and spun the menu loop forever. Now invalid input is caught, cleared, and re-prompted.
2. **O(1) slot allocation** — the original scanned the whole slot vector to find a free one on every park/remove call, despite slot ids being sequential (1..n) and directly indexable. Replaced with a `queue<int>` of free slot ids plus direct `slots[id-1]` access.
3. **Corrected the complexity table** — documented complexity now matches what the code actually does (see above).
4. **Billing edge case** — the original computed `duration_minutes * 2` with truncated integer minutes, so a vehicle parked and removed within the same minute was billed ₹0. Now duration rounds up on any partial minute, and a ₹10 minimum fare applies.
5. **File I/O error handling** — `saveToFile`/`loadFromFile` now check the stream state and warn (rather than silently failing or crashing) on a write failure or a corrupt record.
6. **Const-correctness & reference semantics** — read-only methods are now `const`; loops iterate by `const auto&` instead of copying `Slot`/`Ticket` objects.

## ✅ Testing

`test_edge_cases.sh` pipes scripted menu input into the compiled binary and checks each output against an expected result — no test framework dependency, since the program is a simple console I/O loop.

```bash
bash test_edge_cases.sh
```

Covers: successful parking, duplicate-vehicle rejection, searching/removing a vehicle that doesn't exist, filling all slots (`Parking Full`), non-numeric menu input (must not hang or crash), and the minimum-fare billing edge case.

Current result: **7/7 passing.**

## ▶️ How to Run

### Build
```bash
make
```
(or directly: `g++ -std=c++17 -Wall -Wextra Parking_System.cpp -o Parking_System`)

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
