#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <ctime>
#include <fstream>
using namespace std;

// ---------------- SLOT CLASS ----------------
class Slot {
public:
    int id;
    bool isOccupied;

    Slot(int id) {
        this->id = id;
        this->isOccupied = false;
    }
};

// ---------------- TICKET CLASS ----------------
class Ticket {
public:
    int ticketId;
    int slotid;
    time_t entrytime;
    string vehicleNumber;

    Ticket() {
        ticketId = -1;
        slotid = -1;
        entrytime = 0;
    }

    Ticket(int id, int slotid, string vnum) {
        this->ticketId = id;
        this->slotid = slotid;
        this->vehicleNumber = vnum;
        this->entrytime = time(0);
    }
};

// ---------------- PARKING LOT CLASS ----------------
class Parkinglot {
    vector<Slot> slots;

    map<int, Ticket> activeTickets;      // ticketID → Ticket
    map<string, int> vehicleToTicket;    // vehicle → ticketID
    map<int, string> slotToVehicle;      // slot → vehicle

    int ticketCounter;

public:
    Parkinglot(int totalSlots) {
        ticketCounter = 1;

        for (int i = 1; i <= totalSlots; i++) {
            slots.push_back(Slot(i));
        }
        loadFromFile();
    }

    // ---------------- PARK VEHICLE ----------------
    void parkvehicle(string vehiclenumber) {

        if (vehicleToTicket.find(vehiclenumber) != vehicleToTicket.end()) {
            cout << "Vehicle already parked!\n";
            return;
        }

        for (auto &slot : slots) {
            if (!slot.isOccupied) {
                slot.isOccupied = true;

                int ticketId = ticketCounter++;

                Ticket t(ticketId, slot.id, vehiclenumber);

                activeTickets[ticketId] = t;
                vehicleToTicket[vehiclenumber] = ticketId;
                slotToVehicle[slot.id] = vehiclenumber;

                saveTofile();

                cout << "Vehicle parked at slot " << slot.id << endl;
                cout << "Ticket ID: " << ticketId << endl;

                return;
            }
        }

        cout << "Parking Full!\n";
        
    }

    // ---------------- REMOVE VEHICLE ----------------
    void removeVehicle(string vehiclenumber) {

        if (vehicleToTicket.find(vehiclenumber) == vehicleToTicket.end()) {
            cout << "Vehicle not found\n";
            return;
        }

        int ticketId = vehicleToTicket[vehiclenumber];
        Ticket t = activeTickets[ticketId];

        // free slot
        for (auto &slot : slots) {
            if (slot.id == t.slotid) {
                slot.isOccupied = false;
                break;
            }
        }

        slotToVehicle.erase(t.slotid);

        time_t exittime = time(0);
        int duration = (exittime - t.entrytime) / 60;
        int cost = duration * 2;   // ₹2 per minute

        cout << "Vehicle exited from slot " << t.slotid << endl;
        cout << "Duration: " << duration << " minutes\n";
        cout << "Total cost: " << cost << endl;

        activeTickets.erase(ticketId);
        vehicleToTicket.erase(vehiclenumber);

        saveTofile();
    }

    // ------------Search Vehicle-----------------
    void searchVehicle(string vehiclenumber){
        if(vehicleToTicket.find(vehiclenumber)==vehicleToTicket.end()){
            cout<<"Vehicle not found in parking \n";
            return;
        }
    int ticketId = vehicleToTicket[vehiclenumber];
    Ticket t = activeTickets[ticketId];
    
    cout<<"\n--- Vehicle Found ---\n";
    cout<<"Vehicle Number: "<<vehiclenumber<<endl;
    cout<<"Ticket Id: "<<t.ticketId<<endl;
    cout<<"Slot Number: "<<t.slotid<<endl;

    cout<<"Entry Time: "<<ctime(&t.entrytime);

    }

    // ---------------- SHOW SLOT STATUS ----------------
    void showParkingStatus() {
        cout << "\n--- Parking Slots ---\n";

        for (auto &slot : slots) {
            cout << "Slot " << slot.id << " : ";

            if (slot.isOccupied) {
                cout << "Occupied by " << slotToVehicle[slot.id] << endl;
            } else {
                cout << "Free\n";
            }
        }
    }

    // ---------------- SHOW ACTIVE VEHICLES ----------------
    void showActiveVehicles() {
        cout << "\n--- Active Vehicles ---\n";

        if (activeTickets.empty()) {
            cout << "No vehicles parked\n";
            return;
        }

        for (auto &it : activeTickets) {
            Ticket t = it.second;

            cout << "Ticket ID: " << t.ticketId
                 << ", Vehicle: " << t.vehicleNumber
                 << ", Slot: " << t.slotid << endl;
        }
    }

// -----Saving----
void saveTofile(){
    ofstream file("parking.txt");
    for(auto &entry : activeTickets){
        Ticket t = entry.second;
        file << t.vehicleNumber <<" "
        << t.slotid <<" "
        << t.entrytime <<" "
        << t.ticketId <<endl;
    }
    file.close();
}
void loadFromFile() {
    ifstream file("parking.txt");
    
    if(!file) return;

    string vehicle;
    int slotid, ticketId;
    time_t entrytime;

    while (file >> vehicle >> slotid >> entrytime >> ticketId) {

        // mark slot occupied
        for (auto &slot : slots) {
            if (slot.id == slotid) {
                slot.isOccupied = true;
                break;
            }
        }

        // recreate ticket
        Ticket t;
        t.vehicleNumber = vehicle;
        t.slotid = slotid;
        t.entrytime = entrytime;
        t.ticketId = ticketId;

        activeTickets[ticketId] = t;
        vehicleToTicket[vehicle] = ticketId;
        slotToVehicle[slotid] = vehicle;

        ticketCounter = max(ticketCounter,ticketId+1);
    }

    file.close();
}
};

// ---------------- MAIN FUNCTION ----------------
int main() {
    Parkinglot p(5);

    int choice;
    string vehicle;

    while (true) {
        cout << "\n1. Park Vehicle\n";
        cout << "2. Remove Vehicle\n";
        cout << "3. Show Slots\n";
        cout<< "4.Search Vehicle\n";
        cout << "5. Show Active Vehicles\n";
        cout << "6. Exit\n";

        cin >> choice;

        if (choice == 1) {
            cout << "Enter vehicle number: ";
            cin >> vehicle;
            p.parkvehicle(vehicle);
        }
        else if (choice == 2) {
            cout << "Enter vehicle number: ";
            cin >> vehicle;
            p.removeVehicle(vehicle);
        }
        else if (choice == 3) {
            p.showParkingStatus();
        }
        else if (choice == 4) {
            cout<<"Enter Vehicle number: ";
            cin>>vehicle;
            p.searchVehicle(vehicle);
        }
        else if (choice == 5) {
            p.showActiveVehicles();
        }
        else {
            break;
        }
    }

    return 0;
}
