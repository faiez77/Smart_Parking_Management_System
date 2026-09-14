// Smart Parking Management System
// Console-based OOP parking simulator with O(1) slot allocation,
// input validation, and persistent storage via file handling.

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <ctime>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;

// ---------------- SLOT CLASS ----------------
class Slot {
public:
    int id;
    bool isOccupied;

    explicit Slot(int id) : id(id), isOccupied(false) {}
};

// ---------------- TICKET CLASS ----------------
class Ticket {
public:
    int ticketId;
    int slotId;
    string vehicleNumber;
    time_t entryTime;

    Ticket() : ticketId(-1), slotId(-1), entryTime(0) {}

    Ticket(int id, int slotId, const string& vnum)
        : ticketId(id), slotId(slotId), vehicleNumber(vnum), entryTime(time(nullptr)) {}
};

// ---------------- PARKING LOT CLASS ----------------
class ParkingLot {
    vector<Slot> slots;                 // index i holds slot with id (i+1) -- enables O(1) lookup
    map<int, Ticket> activeTickets;     // ticketId -> Ticket
    map<string, int> vehicleToTicket;   // vehicleNumber -> ticketId
    queue<int> freeSlotIds;             // O(1) allocation instead of scanning for a free slot
    int ticketCounter;

    static const int RATE_PER_MINUTE = 2;   // Rs. 2 per minute
    static const int MINIMUM_FARE = 10;     // Rs. 10 minimum charge, even for very short stays

public:
    explicit ParkingLot(int totalSlots) : ticketCounter(1) {
        for (int i = 1; i <= totalSlots; i++) {
            slots.push_back(Slot(i));
            freeSlotIds.push(i);
        }
        loadFromFile();
    }

    // ---------------- PARK VEHICLE ----------------
    // O(1): pulls the next free slot id from the queue instead of scanning.
    void parkVehicle(const string& vehicleNumber) {
        if (vehicleNumber.empty()) {
            cout << "Vehicle number cannot be empty.\n";
            return;
        }
        if (vehicleToTicket.count(vehicleNumber)) {
            cout << "Vehicle already parked!\n";
            return;
        }
        if (freeSlotIds.empty()) {
            cout << "Parking Full!\n";
            return;
        }

        int slotId = freeSlotIds.front();
        freeSlotIds.pop();
        slots[slotId - 1].isOccupied = true;

        int ticketId = ticketCounter++;
        Ticket t(ticketId, slotId, vehicleNumber);
        activeTickets[ticketId] = t;
        vehicleToTicket[vehicleNumber] = ticketId;

        saveToFile();
        cout << "Vehicle parked at slot " << slotId << "\n";
        cout << "Ticket ID: " << ticketId << "\n";
    }

    // ---------------- REMOVE VEHICLE ----------------
    // O(1) slot release via direct indexing (slots[id-1]) instead of a linear scan.
    void removeVehicle(const string& vehicleNumber) {
        auto it = vehicleToTicket.find(vehicleNumber);
        if (it == vehicleToTicket.end()) {
            cout << "Vehicle not found\n";
            return;
        }

        int ticketId = it->second;
        Ticket t = activeTickets[ticketId];

        slots[t.slotId - 1].isOccupied = false;
        freeSlotIds.push(t.slotId);

        time_t exitTime = time(nullptr);
        int durationMinutes = static_cast<int>((exitTime - t.entryTime) / 60);
        // Round up any partial minute so a 10-second stay still counts as 1 minute,
        // and apply a minimum fare so very short stays aren't billed Rs. 0.
        if ((exitTime - t.entryTime) % 60 != 0) durationMinutes += 1;
        int cost = durationMinutes * RATE_PER_MINUTE;
        if (cost < MINIMUM_FARE) cost = MINIMUM_FARE;

        cout << "Vehicle exited from slot " << t.slotId << "\n";
        cout << "Duration: " << durationMinutes << " minute(s)\n";
        cout << "Total cost: Rs. " << cost << "\n";

        activeTickets.erase(ticketId);
        vehicleToTicket.erase(vehicleNumber);
        saveToFile();
    }

    // ---------------- SEARCH VEHICLE ----------------
    // O(log n) via std::map lookup.
    void searchVehicle(const string& vehicleNumber) const {
        auto it = vehicleToTicket.find(vehicleNumber);
        if (it == vehicleToTicket.end()) {
            cout << "Vehicle not found in parking\n";
            return;
        }
        const Ticket& t = activeTickets.at(it->second);
        cout << "\n--- Vehicle Found ---\n";
        cout << "Vehicle Number: " << vehicleNumber << "\n";
        cout << "Ticket ID: " << t.ticketId << "\n";
        cout << "Slot Number: " << t.slotId << "\n";
        cout << "Entry Time: " << ctime(&t.entryTime);
    }

    // ---------------- SHOW SLOT STATUS ----------------
    void showParkingStatus() const {
        cout << "\n--- Parking Slots ---\n";
        for (const auto& slot : slots) {
            cout << "Slot " << slot.id << " : " << (slot.isOccupied ? "Occupied" : "Free") << "\n";
        }
    }

    // ---------------- SHOW ACTIVE VEHICLES ----------------
    void showActiveVehicles() const {
        cout << "\n--- Active Vehicles ---\n";
        if (activeTickets.empty()) {
            cout << "No vehicles parked\n";
            return;
        }
        for (const auto& entry : activeTickets) {
            const Ticket& t = entry.second;
            cout << "Ticket ID: " << t.ticketId
                 << ", Vehicle: " << t.vehicleNumber
                 << ", Slot: " << t.slotId << "\n";
        }
    }

    // ---------------- FILE PERSISTENCE ----------------
    void saveToFile() const {
        ofstream file("parking.txt");
        if (!file) {
            cerr << "Warning: could not write parking.txt (changes not saved to disk)\n";
            return;
        }
        for (const auto& entry : activeTickets) {
            const Ticket& t = entry.second;
            file << t.vehicleNumber << " "
                 << t.slotId << " "
                 << t.entryTime << " "
                 << t.ticketId << "\n";
        }
    }

    void loadFromFile() {
        ifstream file("parking.txt");
        if (!file) return;  // no saved state yet -- not an error

        string vehicle;
        int slotId, ticketId;
        time_t entryTime;
        int lineNum = 0;

        while (file >> vehicle >> slotId >> entryTime >> ticketId) {
            lineNum++;
            if (slotId < 1 || slotId > static_cast<int>(slots.size())) {
                cerr << "Warning: skipping corrupt record on line " << lineNum
                     << " (invalid slot id " << slotId << ")\n";
                continue;
            }
            slots[slotId - 1].isOccupied = true;

            Ticket t;
            t.vehicleNumber = vehicle;
            t.slotId = slotId;
            t.entryTime = entryTime;
            t.ticketId = ticketId;
            activeTickets[ticketId] = t;
            vehicleToTicket[vehicle] = ticketId;
            ticketCounter = max(ticketCounter, ticketId + 1);
        }

        // Rebuild the free-slot queue from whatever wasn't marked occupied above.
        while (!freeSlotIds.empty()) freeSlotIds.pop();
        for (const auto& slot : slots) {
            if (!slot.isOccupied) freeSlotIds.push(slot.id);
        }
    }
};

// ---------------- INPUT HELPER ----------------
// Guards against non-numeric input, which would otherwise put cin into a
// permanent fail state and spin the menu loop forever.
int readMenuChoice() {
    int choice;
    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number: ";
    }
    return choice;
}

// ---------------- MAIN FUNCTION ----------------
int main() {
    ParkingLot p(5);
    string vehicle;

    while (true) {
        cout << "\n1. Park Vehicle\n";
        cout << "2. Remove Vehicle\n";
        cout << "3. Show Slots\n";
        cout << "4. Search Vehicle\n";
        cout << "5. Show Active Vehicles\n";
        cout << "6. Exit\n";
        cout << "Choice: ";

        int choice = readMenuChoice();

        if (choice == 1) {
            cout << "Enter vehicle number: ";
            cin >> vehicle;
            p.parkVehicle(vehicle);
        } else if (choice == 2) {
            cout << "Enter vehicle number: ";
            cin >> vehicle;
            p.removeVehicle(vehicle);
        } else if (choice == 3) {
            p.showParkingStatus();
        } else if (choice == 4) {
            cout << "Enter vehicle number: ";
            cin >> vehicle;
            p.searchVehicle(vehicle);
        } else if (choice == 5) {
            p.showActiveVehicles();
        } else if (choice == 6) {
            break;
        } else {
            cout << "Invalid option. Choose 1-6.\n";
        }
    }
    return 0;
}
