#include <memory>
#include <string>
#include <iostream>
#include "Passenger.h"
#include "DataManager.h"
#include "Utils.h"
#include "IdGenerator.h"
#include "Payment.h"
#include "BoardingPass.h"

using namespace std;

Passenger::Passenger(int id, const string &username, const string &password, const string &name, const string &contact)
    : User(id, username, password, Role::Passenger), name(name), contact(contact), loyaltyPoints(0) {}

void Passenger::viewReservations() const
{
    auto dm = DataManager::getInstance();
    cout << "--- My Reservations ---\n";
    cout << "Fetching reservations for Passenger " << getName() << "...\n\n";
    auto list = dm->reservationRepo->findByPassengerId(getId());
    int i = 1;
    for (const auto &r : list)
    {
        auto flight = dm->flightRepo->findByNumber(r->getFlightNumber());
        cout << i++ << ". Reservation ID: R" << r->getId() << "\n";
        if (flight)
        {
            cout << "   Flight: " << flight->getFlightNumber() << " from " << flight->getOrigin() << " (" << flight->getOrigin() << ") to " << flight->getDestination() << " (" << flight->getDestination() << ")\n";
            cout << "   Departure: " << flight->getDepartureTime() << "\n";
        }
        else
        {
            cout << "   Flight: " << r->getFlightNumber() << "\n";
            cout << "   Departure: Unknown\n";
        }
        cout << "   Seat: " << r->getSeatRow() << r->getSeatCol() << "\n";
        cout << "   Status: " << r->getStatus() << "\n\n";
    }
    if (i == 1)
        cout << "No reservations found.\n";
}

shared_ptr<Reservation> Passenger::bookFlight(const string &flightNumber, int row, char col, PaymentMethod method, const string &methodStr)
{
    auto dm = DataManager::getInstance();
    auto flight = dm->flightRepo->findByNumber(flightNumber);
    if (!flight)
    {
        cout << "Flight not found\n";
        return nullptr;
    }
    if (!flight->isSeatAvailable(row, col))
    {
        cout << "Error: Seat " << row << col << " on Flight " << flightNumber << " is already occupied. Please choose a different seat.\n";
        return nullptr;
    }
    int reservationId = IdGenerator::getNextId();
    // Dynamic pricing based on route
    double price = 300.0;
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
    auto res = make_shared<Reservation>(reservationId, getId(), flightNumber, row, col, price);
    if (!flight->reserveSeat(row, col, reservationId))
    {
        cout << "Error: Seat " << row << col << " on Flight " << flightNumber << " is already occupied. Please choose a different seat.\n";
        return nullptr;
    }
    dm->reservationRepo->addReservation(res);
    int paymentId = IdGenerator::getNextId();
    auto pay = make_shared<Payment>(paymentId, reservationId, price, method);
    if (pay->processPayment())
    {
        dm->paymentRepo->addPayment(pay);
        dm->saveAll("data");
        cout << "Booking successful!\n";
        cout << "Reservation ID: R" << reservationId << "\n";
        cout << "Flight: " << flightNumber << "\n";
        cout << "Seat: " << row << col << "\n";
        cout << "Total Cost: $" << price << "\n";
        cout << "Payment Method: " << methodStr << "\n";
        return res;
    }
    else
    {
        flight->cancelSeat(row, col);
        dm->reservationRepo->removeReservation(reservationId);
        cout << "Payment failed, reservation rolled back\n";
        return nullptr;
    }
}

string Passenger::getName() const { return name; }
string Passenger::getContact() const { return contact; }
int Passenger::getLoyaltyPoints() const { return loyaltyPoints; }
void Passenger::addLoyaltyPoints(int points)
{
    if (points > 0)
        loyaltyPoints += points;
}
void Passenger::setLoyaltyPoints(int points) { loyaltyPoints = points; }

void Passenger::searchFlights(const string &origin, const string &destination, const string &date) const
{
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
        if (!destination.empty() && f->getDestination().find(destination) == string::npos)
            continue;
        if (!date.empty() && f->getDepartureTime().rfind(date, 0) != 0)
            continue;
        cout << idx++ << ". Flight Number: " << f->getFlightNumber() << "\n";
        cout << "   Departure: " << f->getDepartureTime() << "\n";
        cout << "   Arrival: " << f->getArrivalTime() << "\n";
        cout << "   Aircraft: " << (f->getAircraft() ? f->getAircraft()->getModel() : string("Unknown")) << "\n";
        cout << "   Available Seats: " << f->getAvailableSeats() << "\n";
        // Dynamic pricing based on route and availability
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

void Passenger::checkIn(int reservationId)
{
    cout << "Passenger checked in for reservation ID: R" << reservationId << "\n";
}

void Passenger::viewMenu() const
{
    cout << "--- Passenger Menu ---\n";
    cout << "1. Search Flights\n";
    cout << "2. View My Reservations\n";
    cout << "3. Check-In\n";
    cout << "4. Logout\n";
}