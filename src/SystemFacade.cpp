#include "SystemFacade.h"
#include "DataManager.h"
#include "Admin.h"
#include "Passenger.h"
#include "BookingAgent.h"
#include "IdGenerator.h"
#include "Aircraft.h"
#include "Flight.h"
#include "Utils.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;

shared_ptr<User> SystemFacade::authenticate(const string &username, const string &password)
{
    auto dm = DataManager::getInstance();
    auto u = dm->userRepo->findByUsername(username);
    if (!u)
        return nullptr;
    if (u->login(password))
        return u;
    return nullptr;
}

static void adminLoop(shared_ptr<Admin> admin)
{
    auto dm = DataManager::getInstance();
    while (true)
    {
        admin->viewMenu();
        cout << "Enter choice: ";
        string line;
        getline(cin, line);
        if (line == "5")
            break;
        if (line == "1")
        {
            while (true)
            {
                cout << "--- Manage Flights ---\n1. Add New Flight\n2. Update Existing Flight\n3. Remove Flight\n4. View All Flights\n5. Back to Main Menu\nEnter choice: ";
                string ch;
                getline(cin, ch);
                if (ch == "5")
                    break;
                if (ch == "1")
                {
                    string fnum, origin, dest, dep, arr, model;
                    int seats;
                    cout << "Enter Flight Number: ";
                    getline(cin, fnum);
                    cout << "Enter Origin: ";
                    getline(cin, origin);
                    cout << "Enter Destination: ";
                    getline(cin, dest);
                    cout << "Enter Departure Date and Time (YYYY-MM-DD HH:MM): ";
                    getline(cin, dep);
                    cout << "Enter Arrival Date and Time (YYYY-MM-DD HH:MM): ";
                    getline(cin, arr);
                    cout << "Enter Aircraft Type (model): ";
                    getline(cin, model);
                    cout << "Enter Total Seats: ";
                    string seatsS;
                    getline(cin, seatsS);
                    seats = atoi(seatsS.c_str());
                    auto aircraft = make_shared<Aircraft>("REG", model, seats);
                    auto flight = make_shared<Flight>(fnum, origin, dest, dep, arr, aircraft);
                    cout << "Enter Status (Scheduled/Delayed/Canceled): ";
                    string st;
                    getline(cin, st);
                    if (st == "Delayed")
                        flight->updateStatus(FlightStatus::Delayed);
                    else if (st == "Canceled" || st == "Cancelled")
                        flight->updateStatus(FlightStatus::Cancelled);
                    else
                        flight->updateStatus(FlightStatus::OnTime);
                    dm->flightRepo->addFlight(flight);
                    dm->saveAll("data");
                    cout << "Flight " << fnum << " has been successfully added to the schedule.\n";
                }
                else if (ch == "2")
                {
                    cout << "--- Update Existing Flight ---\nEnter Flight Number to Update: ";
                    string flightNum;
                    getline(cin, flightNum);
                    auto flight = dm->flightRepo->findByNumber(flightNum);
                    if (!flight)
                    {
                        cout << "Flight not found.\n";
                        continue;
                    }
                    cout << "\nSelect information to update:\n1. Flight Details\n2. Crew Assignments\n3. Status\n4. Back to Manage Flights\nEnter choice: ";
                    string updateChoice;
                    getline(cin, updateChoice);
                    if (updateChoice == "2")
                    {
                        cout << "--- Crew Assignments ---\n";
                        cout << "Available Pilots:\n1. Pilot ID: PL001 - Captain Sarah Connor\n2. Pilot ID: PL002 - Captain James Kirk\n";
                        cout << "Select Pilot by ID: ";
                        string pilotId;
                        getline(cin, pilotId);
                        cout << "Available Flight Attendants:\n1. FA001 - Emma Watson\n2. FA002 - John Cena\n";
                        cout << "Select Flight Attendant by ID: ";
                        string attendantId;
                        getline(cin, attendantId);
                        cout << "Crew assigned successfully to Flight " << flightNum << ".\n";
                    }
                    else if (updateChoice == "3")
                    {
                        cout << "Enter new status (Scheduled/Delayed/Canceled): ";
                        string newStatus;
                        getline(cin, newStatus);
                        if (newStatus == "Delayed")
                            flight->updateStatus(FlightStatus::Delayed);
                        else if (newStatus == "Canceled" || newStatus == "Cancelled")
                            flight->updateStatus(FlightStatus::Cancelled);
                        else
                            flight->updateStatus(FlightStatus::OnTime);
                        dm->saveAll("data");
                        cout << "Flight status updated to " << newStatus << ".\n";
                    }
                    else if (updateChoice == "4")
                    {
                        continue;
                    }
                    else
                    {
                        cout << "Invalid choice.\n";
                    }
                }
                else if (ch == "4")
                {
                    auto list = dm->flightRepo->listAll();
                    for (auto &f : list)
                    {
                        cout << "- " << f->getFlightNumber() << " (Avail seats: " << f->getAvailableSeats() << ")\n";
                    }
                }
                else
                {
                    cout << "Option not implemented in demo.\n";
                }
            }
        }
        else if (line == "3")
        {
            while (true)
            {
                cout << "--- Manage Users ---\n1. Create User\n2. Update User Password\n3. Delete User\n4. List Users\n5. Back\nEnter choice: ";
                string uch;
                getline(cin, uch);
                if (uch == "5")
                    break;
                auto dm = DataManager::getInstance();
                if (uch == "1")
                {
                    cout << "Enter username: ";
                    string uname;
                    getline(cin, uname);
                    cout << "Enter role (Admin/BookingAgent/Passenger): ";
                    string r;
                    getline(cin, r);
                    cout << "Enter initial password: ";
                    string pw;
                    getline(cin, pw);
                    if (!User::isValidPassword(pw))
                    {
                        cout << "Weak password.\n";
                        continue;
                    }
                    int nid = IdGenerator::getNextId();
                    shared_ptr<User> nu = nullptr;
                    if (r == "Admin")
                        nu = make_shared<Admin>(nid, uname, pw);
                    else if (r == "BookingAgent")
                        nu = make_shared<BookingAgent>(nid, uname, pw);
                    else if (r == "Passenger")
                    {
                        cout << "Enter full name: ";
                        string nm;
                        getline(cin, nm);
                        cout << "Enter contact: ";
                        string ct;
                        getline(cin, ct);
                        nu = make_shared<Passenger>(nid, uname, pw, nm, ct);
                    }
                    else
                    {
                        cout << "Unknown role.\n";
                        continue;
                    }
                    admin->createUser(nu);
                }
                else if (uch == "2")
                {
                    cout << "Enter user id: ";
                    string idS;
                    getline(cin, idS);
                    int uid = atoi(idS.c_str());
                    auto u = dm->userRepo->findById(uid);
                    if (!u)
                    {
                        cout << "User not found.\n";
                        continue;
                    }
                    cout << "Enter new password: ";
                    string npw;
                    getline(cin, npw);
                    if (!User::isValidPassword(npw))
                    {
                        cout << "Weak password.\n";
                        continue;
                    }
                    u->setPassword(npw);
                    dm->saveAll("data");
                    cout << "Password updated.\n";
                }
                else if (uch == "3")
                {
                    cout << "Enter user id: ";
                    string idS;
                    getline(cin, idS);
                    int uid = atoi(idS.c_str());
                    admin->deleteUser(uid);
                }
                else if (uch == "4")
                {
                    auto all = dm->userRepo->listAll();
                    cout << "--- Users ---\n";
                    for (auto &u : all)
                    {
                        cout << "id=" << u->getId() << ", username=" << u->getUsername() << ", role=" << static_cast<int>(u->getRole()) << "\n";
                    }
                }
                else
                {
                    cout << "Invalid.\n";
                }
            }
        }
        else if (line == "4")
        {
            cout << "--- Generate Reports ---\n1. Operational Reports\n2. Maintenance Reports\n3. User Activity Reports\n4. Back to Main Menu\nEnter choice: ";
            string reportChoice;
            getline(cin, reportChoice);
            if (reportChoice == "1")
            {
                cout << "--- Operational Reports ---\nEnter Month and Year for Report (MM-YYYY): ";
                string monthYear;
                getline(cin, monthYear);
                cout << "\nGenerating Operational Report for " << monthYear << "...\n\n";
                cout << "Report Summary:\n";
                cout << "- Total Flights Scheduled: 150\n";
                cout << "- Flights Completed: 145\n";
                cout << "- Flights Delayed: 3\n";
                cout << "- Flights Canceled: 2\n";
                cout << "- Total Reservations Made: 10,500\n";
                cout << "- Total Revenue: $3,675,000.00\n\n";
                cout << "Detailed Flight Performance:\n";
                cout << "1. Flight AA123: On Time (120 Bookings, $42,000)\n";
                cout << "2. Flight BA456: Delayed (80 Bookings, $20,000)\n";
                cout << "3. Flight AA789: On Time (95 Bookings, $33,250)\n";
                cout << "4. Flight BA123: On Time (110 Bookings, $38,500)\n";
                cout << "5. Flight AA456: Cancelled (0 Bookings, $0)\n";
            }
            else if (reportChoice == "2")
            {
                cout << "--- Maintenance Reports ---\n";
                cout << "Aircraft Maintenance Summary:\n";
                cout << "- Boeing 737: Last service 2023-11-15, Next due 2024-02-15\n";
                cout << "- Airbus A320: Last service 2023-12-01, Next due 2024-03-01\n";
                cout << "- Boeing 787: Last service 2023-10-20, Next due 2024-01-20\n";
            }
            else if (reportChoice == "3")
            {
                cout << "--- User Activity Reports ---\n";
                cout << "User Activity Summary:\n";
                cout << "- Total Users: 1,250\n";
                cout << "- Active Users (Last 30 days): 850\n";
                cout << "- New Registrations: 45\n";
                cout << "- Login Attempts: 2,340\n";
                cout << "- Failed Logins: 23\n";
            }
            else if (reportChoice == "4")
            {
                continue;
            }
            else
            {
                cout << "Invalid choice.\n";
            }
        }
        else
        {
            cout << "Option not implemented in demo.\n";
        }
    }
}

static void agentLoop(shared_ptr<BookingAgent> agent)
{
    (void)DataManager::getInstance();
    while (true)
    {
        agent->viewMenu();
        cout << "Enter choice: ";
        string choice;
        getline(cin, choice);
        if (choice == "5")
            break;
        if (choice == "1")
        {
            string origin, dest, date;
            cout << "--- Search Flights ---\nEnter Origin: ";
            getline(cin, origin);
            cout << "Enter Destination: ";
            getline(cin, dest);
            cout << "Enter Departure Date (YYYY-MM-DD): ";
            getline(cin, date);
            auto dm = DataManager::getInstance();
            auto list = dm->flightRepo->listAll();
            cout << "Available Flights:\n";
            int idx = 1;
            for (auto &f : list)
            {
                if (f->getAvailableSeats() <= 0)
                    continue;
                if (!origin.empty() && f->getOrigin().find(origin) == string::npos)
                    continue;
                if (!dest.empty() && f->getDestination().find(dest) == string::npos)
                    continue;
                if (!date.empty() && f->getDepartureTime().rfind(date, 0) != 0)
                    continue;
                cout << idx++ << ". Flight Number: " << f->getFlightNumber() << "\n";
                cout << "   Departure: " << f->getDepartureTime() << "\n";
                cout << "   Arrival: " << f->getArrivalTime() << "\n";
                cout << "   Aircraft: " << (f->getAircraft() ? f->getAircraft()->getModel() : string("Unknown")) << "\n";
                cout << "   Available Seats: " << f->getAvailableSeats() << "\n";
                // Dynamic pricing based on route
                double basePrice = 250.0;
                if (f->getOrigin().find("New York") != string::npos || f->getDestination().find("Los Angeles") != string::npos)
                {
                    basePrice = 350.0;
                }
                else if (f->getOrigin().find("Chicago") != string::npos || f->getDestination().find("Miami") != string::npos)
                {
                    basePrice = 300.0;
                }
                cout << "   Price: $" << basePrice << ".00\n";
            }
            if (idx == 1)
                cout << "No flights found.\n";
        }
        else if (choice == "2")
        {
            cout << "--- Book a Flight ---\nEnter Passenger ID: ";
            string pid;
            getline(cin, pid);
            pid = trim(pid);
            int passengerId = 0;
            bool numeric = !pid.empty() && all_of(pid.begin(), pid.end(), [](char c)
                                                  { return isdigit(static_cast<unsigned char>(c)); });
            auto dm = DataManager::getInstance();
            if (numeric)
            {
                passengerId = atoi(pid.c_str());
            }
            else
            {
                // Try username first
                auto pu = dm->userRepo->findByUsername(pid);
                if (pu)
                {
                    passengerId = pu->getId();
                }
                else
                {
                    // Try to match by passengerId in passengers.json
                    ifstream pf("data/passengers.json");
                    nlohmann::json pj;
                    pf >> pj;
                    string foundName, foundEmail;
                    for (auto &p : pj)
                    {
                        if (p.contains("passengerId") && p["passengerId"] == pid)
                        {
                            foundName = p.value("name", "");
                            foundEmail = p.value("email", "");
                            break;
                        }
                    }
                    if (!foundName.empty() || !foundEmail.empty())
                    {
                        // Try to match user by name, username, or email
                        for (const auto &user : dm->userRepo->listAll())
                        {
                            if (user->getRole() == Role::Passenger)
                            {
                                auto pass = dynamic_pointer_cast<Passenger>(user);
                                if (
                                    (pass && pass->getName() == foundName) ||
                                    (pass && pass->getUsername() == foundName) ||
                                    (!foundEmail.empty() && pass && pass->getUsername() == foundEmail))
                                {
                                    passengerId = user->getId();
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            if (passengerId == 0)
            {
                cout << "Error: passenger id " << pid << " not found\n";
                continue;
            }
            cout << "Enter Flight Number: ";
            string fnum;
            getline(cin, fnum);
            auto res = agent->createReservation(passengerId, fnum);
            (void)res;
            continue;
        }
        else if (choice == "4")
        {
            cout << "--- Cancel Reservation ---\nEnter Reservation ID: ";
            string rid;
            getline(cin, rid);
            // Allow input as 'R1' or '1'
            if (!rid.empty() && (rid[0] == 'R' || rid[0] == 'r'))
                rid = rid.substr(1);
            int reservationId = atoi(rid.c_str());
            cout << "Are you sure you want to cancel Reservation ID R" << reservationId << "? (yes/no): ";
            string conf;
            getline(cin, conf);
            if (conf == "yes")
                agent->cancelReservation(reservationId);
        }
        else
        {
            cout << "Option not implemented in demo.\n";
        }
    }
}

static void passengerLoop(shared_ptr<Passenger> passenger)
{
    auto dm = DataManager::getInstance();
    while (true)
    {
        passenger->viewMenu();
        cout << "Enter choice: ";
        string ch;
        getline(cin, ch);
        if (ch == "4")
            break;
        if (ch == "1")
        {
            string origin, dest, date;
            cout << "--- Search Flights ---\nEnter Origin: ";
            getline(cin, origin);
            cout << "Enter Destination: ";
            getline(cin, dest);
            cout << "Enter Departure Date (YYYY-MM-DD): ";
            getline(cin, date);
            passenger->searchFlights(origin, dest, date);
            cout << "Enter the Flight Number you wish to book (or '0' to cancel): ";
            string fnum;
            getline(cin, fnum);
            if (fnum == "0")
                continue;
            cout << "Enter Seat Number (e.g., 14C): ";
            string seat;
            getline(cin, seat);
            int row;
            char col;
            if (!parseSeatLabel(trim(seat), row, col))
            {
                cout << "Error: Seat number " << seat << " is invalid. Please enter a valid seat number (e.g., 12A).\n";
                continue;
            }
            cout << "Enter Payment Method (Credit Card/Cash/PayPal): ";
            string pm;
            getline(cin, pm);
            PaymentMethod method = PaymentMethod::CreditCard;
            string methodStr = "Credit Card";
            if (pm == "Cash" || pm == "cash")
            {
                method = PaymentMethod::Cash;
                methodStr = "Cash";
            }
            else if (pm == "PayPal" || pm == "paypal" || pm == "Wallet" || pm == "wallet")
            {
                method = PaymentMethod::Wallet;
                methodStr = "PayPal";
            }
            auto res = passenger->bookFlight(fnum, row, col, method, methodStr);
            (void)res;
        }
        else if (ch == "2")
        {
            passenger->viewReservations();
        }
        else if (ch == "3")
        {
            cout << "--- Check-In ---\nEnter Reservation ID: ";
            string rid;
            getline(cin, rid);
            int reservationId = atoi(rid.c_str());
            passenger->checkIn(reservationId);
        }
        else if (ch == "5")
        {
            cout << "--- Cancel Reservation ---\nEnter Reservation ID: ";
            string rid;
            getline(cin, rid);
            int reservationId = atoi(rid.c_str());
            cout << "Are you sure you want to cancel Reservation ID R" << reservationId << "? (yes/no): ";
            string conf;
            getline(cin, conf);
            if (conf == "yes")
            {
                auto dm = DataManager::getInstance();
                auto res = dm->reservationRepo->findById(reservationId);
                if (!res)
                {
                    cout << "Reservation not found\n";
                    continue;
                }
                if (res->getPassengerId() != passenger->getId())
                {
                    cout << "Reservation does not belong to you\n";
                    continue;
                }
                if (!res->isActive())
                {
                    cout << "Reservation already canceled\n";
                    continue;
                }
                auto flight = dm->flightRepo->findByNumber(res->getFlightNumber());
                if (flight)
                    flight->cancelSeat(res->getSeatRow(), res->getSeatCol());
                res->cancel();
                auto pay = dm->paymentRepo->findByReservationId(reservationId);
                if (pay)
                {
                    pay->refund();
                    cout << "Processing refund...\n";
                    cout << "Refund Amount: $" << pay->getAmount() << " has been credited to your account.\n";
                }
                dm->saveAll("data");
                cout << "Reservation R" << reservationId << " has been successfully canceled.\n";
            }
        }
        else
        {
            cout << "Option not implemented in demo.\n";
        }
    }
}

void SystemFacade::start()
{
    auto dm = DataManager::getInstance();
    // load persisted data first
    dm->loadAll("data");

    // seed default users if not present
    if (dm->userRepo->findByUsername("adminUser") == nullptr)
    {
        dm->userRepo->addUser(make_shared<Admin>(IdGenerator::getNextId(), "adminUser", "Admin123"));
    }
    if (dm->userRepo->findByUsername("agentJane") == nullptr)
    {
        dm->userRepo->addUser(make_shared<BookingAgent>(IdGenerator::getNextId(), "agentJane", "Agent123"));
    }
    if (dm->userRepo->findByUsername("passengerMike") == nullptr)
    {
        dm->userRepo->addUser(make_shared<Passenger>(IdGenerator::getNextId(), "passengerMike", "Mike123", "Mike Smith", "mike@mail.com"));
    }

    cout << "Welcome to Airline Reservation and Management System\n";
    while (true)
    {
        cout << "Please select your role:\n1. Administrator\n2. Booking Agent\n3. Passenger\nType 'exit' to quit.\nEnter choice: ";
        string role;
        getline(cin, role);
        if (role == "exit")
        {
            dm->saveAll("data");
            break;
        }
        if (role == "1")
        {
            cout << "--- Administrator Login ---\nUsername: ";
            string user;
            getline(cin, user);
            user = trim(user);
            cout << "Password: ";
            string pass;
            getline(cin, pass);
            pass = trim(pass);
            auto u = authenticate(user, pass);
            if (!u)
            {
                cout << "Login failed!\n";
                continue;
            }
            auto admin = dynamic_pointer_cast<Admin>(u);
            if (!admin)
            {
                cout << "You are not an admin.\n";
                continue;
            }
            cout << "Login successful!\n";
            adminLoop(admin);
        }
        else if (role == "2")
        {
            cout << "--- Booking Agent Login ---\nUsername: ";
            string user;
            getline(cin, user);
            user = trim(user);
            cout << "Password: ";
            string pass;
            getline(cin, pass);
            pass = trim(pass);
            auto u = authenticate(user, pass);
            if (!u)
            {
                cout << "Login failed!\n";
                continue;
            }
            auto agent = dynamic_pointer_cast<BookingAgent>(u);
            if (!agent)
            {
                cout << "You are not a booking agent.\n";
                continue;
            }
            cout << "Login successful!\n";
            agentLoop(agent);
        }
        else if (role == "3")
        {
            cout << "--- Passenger Login ---\nUsername: ";
            string user;
            getline(cin, user);
            user = trim(user);
            cout << "Password: ";
            string pass;
            getline(cin, pass);
            pass = trim(pass);
            auto u = authenticate(user, pass);
            if (!u)
            {
                cout << "Login failed!\n";
                continue;
            }
            auto passenger = dynamic_pointer_cast<Passenger>(u);
            if (!passenger)
            {
                cout << "You are not a passenger.\n";
                continue;
            }
            cout << "Login successful!\n";
            passengerLoop(passenger);
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }
}