#include "BookingAgent.h"
#include "DataManager.h"
#include "Utils.h"
#include "IdGenerator.h"
#include "Payment.h"
#include "Reservation.h"
#include <iostream>

using namespace std;

BookingAgent::BookingAgent(int id, const string &username, const string &password)
    : User(id, username, password, Role::BookingAgent) {}

shared_ptr<Reservation> BookingAgent::createReservation(int passengerId, const string &flightNumber)
{
    auto dm = DataManager::getInstance();
    auto passenger = dm->userRepo->findById(passengerId);
    if (!passenger)
    {
        cout << "Error: passenger id " << passengerId << " not found\n";
        return nullptr;
    }
    auto flight = dm->flightRepo->findByNumber(flightNumber);
    if (!flight)
    {
        cout << "Error: flight " << flightNumber << " not found\n";
        return nullptr;
    }

    string seatLabel;
    cout << "Enter Seat Number (e.g., 12A or 158): ";
    getline(cin, seatLabel);
    seatLabel = trim(seatLabel);
    int row = 0;
    char col = 'A';
    bool valid = false;

    if (parseSeatLabel(seatLabel, row, col))
    {
        valid = true;
    }
    else
    {
        // Try numeric seat number
        bool isNumeric = !seatLabel.empty() && all_of(seatLabel.begin(), seatLabel.end(), ::isdigit);
        if (isNumeric)
        {
            int seatNum = stoi(seatLabel);
            auto aircraft = flight->getAircraft();
            int totalSeats = aircraft ? aircraft->getTotalSeats() : 0;
            int cols = 6; // default seats per row (A-F)
            if (seatNum >= 1 && seatNum <= totalSeats)
            {
                row = (seatNum - 1) / cols + 1;
                int colIdx = (seatNum - 1) % cols;
                col = 'A' + colIdx;
                valid = true;
            }
        }
    }
    if (!valid)
    {
        cout << "Error: Seat number " << seatLabel << " is invalid. Please enter a valid seat number (e.g., 12A or 158).\n";
        return nullptr;
    }
    if (!flight->isSeatAvailable(row, col))
    {
        cout << "Error: Seat " << row << col << " on Flight " << flightNumber << " is already occupied.\n";
        return nullptr;
    }
    int reservationId = IdGenerator::getNextId();
    // Dynamic pricing based on route
    double price = 350.0;
    if (flight->getOrigin().find("New York") != string::npos || flight->getDestination().find("Los Angeles") != string::npos)
    {
        price = 350.0;
    }
    else if (flight->getOrigin().find("Chicago") != string::npos || flight->getDestination().find("Miami") != string::npos)
    {
        price = 300.0;
    }
    else
    {
        price = 250.0;
    }
    auto res = make_shared<Reservation>(reservationId, passengerId, flightNumber, row, col, price);
    bool ok = flight->reserveSeat(row, col, reservationId);
    if (!ok)
    {
        cout << "Error: failed to reserve seat\n";
        return nullptr;
    }
    dm->reservationRepo->addReservation(res);
    cout << "Enter Payment Method (Credit Card/Cash/PayPal): ";
    string pmIn;
    getline(cin, pmIn);
    PaymentMethod pm = PaymentMethod::CreditCard;
    if (pmIn == "Cash")
        pm = PaymentMethod::Cash;
    else if (pmIn == "PayPal")
        pm = PaymentMethod::Wallet; // reuse Wallet for PayPal in demo
    cout << "Enter Payment Details: ";
    string details;
    getline(cin, details);
    int paymentId = IdGenerator::getNextId();
    auto pay = make_shared<Payment>(paymentId, reservationId, price, pm);
    pay->setDetails(details);
    if (pay->processPayment())
    {
        dm->paymentRepo->addPayment(pay);
        dm->saveAll("data");
        cout << "Booking successful!\nReservation ID: R" << reservationId
             << "\nPassenger: " << passenger->getUsername()
             << "\nFlight: " << flightNumber
             << "\nSeat: " << row << col
             << "\nTotal Cost: $" << price
             << "\nPayment Method: " << (pmIn.empty() ? "Credit Card" : pmIn) << "\n";
        return res;
    }
    else
    {
        flight->cancelSeat(row, col);
        dm->reservationRepo->removeReservation(reservationId);
        cout << "Error: payment failed, reservation rolled back\n";
        return nullptr;
    }
}

void BookingAgent::assistCheckIn(int reservationId)
{
    cout << "[Agent] assist checkin " << reservationId << "\n";
}

void BookingAgent::cancelReservation(int reservationId)
{
    auto dm = DataManager::getInstance();
    auto res = dm->reservationRepo->findById(reservationId);
    if (!res)
    {
        cout << "Reservation not found\n";
        return;
    }
    if (!res->isActive())
    {
        cout << "Reservation already canceled\n";
        return;
    }
    auto flight = dm->flightRepo->findByNumber(res->getFlightNumber());
    if (flight)
        flight->cancelSeat(res->getSeatRow(), res->getSeatCol());
    res->cancel();
    auto pay = dm->paymentRepo->findByReservationId(reservationId);
    if (pay)
    {
        string details = pay->getDetails();
        pay->refund();
        if (pay->getMethod() == PaymentMethod::Wallet && !details.empty())
        {
            cout << "Processing refund to PayPal account: " << details << "\n";
        }
        else
        {
            cout << "Processing refund...\n";
        }
        cout << "Refund Amount: $" << pay->getAmount();
        if (pay->getMethod() == PaymentMethod::Wallet && !details.empty())
        {
            cout << " has been credited to your PayPal account.\n";
        }
        else
        {
            cout << " has been credited to your account.\n";
        }
    }
    dm->saveAll("data");
    cout << "Reservation R" << reservationId << " has been successfully canceled.\n";
}

void BookingAgent::viewMenu() const
{
    cout << "\n=== Booking Agent Menu (" << getUsername() << ") ===\n";
    cout << "1. Search Flights\n2. Book a Flight\n3. Modify Reservation\n4. Cancel Reservation\n5. Logout\n";
}