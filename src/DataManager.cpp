#include "DataManager.h"
#include "UserRepository.h"
#include "FlightRepository.h"
#include "ReservationRepository.h"
#include "PaymentRepository.h"
#include "User.h"
#include "Passenger.h"
#include "Admin.h"
#include "BookingAgent.h"
#include "Flight.h"
#include "Aircraft.h"
#include "Reservation.h"
#include "Payment.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <memory>
#include "nlohmann\json.hpp"

using namespace std;
using json = nlohmann::json;
namespace fs = filesystem;

DataManager *DataManager::instance = nullptr;

DataManager::DataManager()
{
    userRepo = make_unique<UserRepository>();
    flightRepo = make_unique<FlightRepository>();
    reservationRepo = make_unique<ReservationRepository>();
    paymentRepo = make_unique<PaymentRepository>();
}

DataManager *DataManager::getInstance()
{
    if (!instance)
        instance = new DataManager();
    return instance;
}

bool DataManager::saveAll(const string &path)
{
    try
    {
        if (!fs::exists(path))
            fs::create_directories(path);

        // users
        json jusers = json::array();
        auto users = userRepo->listAll();
        for (auto &u : users)
        {
            json ju;
            ju["id"] = u->getId();
            ju["username"] = u->getUsername();
            // persist salted hash
            ju["passwordHash"] = u->getPasswordHash();
            ju["salt"] = u->getSalt();
            ju["role"] = static_cast<int>(u->getRole());
            if (auto p = dynamic_pointer_cast<Passenger>(u))
            {
                ju["type"] = "Passenger";
                ju["name"] = p->getName();
                ju["contact"] = p->getContact();
                ju["loyaltyPoints"] = p->getLoyaltyPoints();
            }
            else if (auto a = dynamic_pointer_cast<Admin>(u))
            {
                ju["type"] = "Admin";
            }
            else if (auto ag = dynamic_pointer_cast<BookingAgent>(u))
            {
                ju["type"] = "BookingAgent";
            }
            else
            {
                ju["type"] = "User";
            }
            jusers.push_back(ju);
        }
        {
            ofstream f(path + "/users.json");
            f << jusers.dump(2);
        }

        // flights
        json jflights = json::array();
        auto flights = flightRepo->listAll();
        for (auto &f : flights)
        {
            json jf;
            jf["flightNumber"] = f->getFlightNumber();
            jf["origin"] = f->getOrigin();
            jf["destination"] = f->getDestination();
            jf["departureTime"] = f->getDepartureTime();
            jf["arrivalTime"] = f->getArrivalTime();
            jf["status"] = static_cast<int>(f->getStatus());
            if (auto ac = f->getAircraft())
            {
                jf["aircraft"] = {
                    {"regNumber", ac->getRegNumber()},
                    {"model", ac->getModel()},
                    {"totalSeats", ac->getTotalSeats()}};
            }
            else
            {
                jf["aircraft"] = nullptr;
            }
            jf["seats"] = f->exportSeatMapForPersistence();
            jflights.push_back(jf);
        }
        {
            ofstream f(path + "/flights.json");
            f << jflights.dump(2);
        }

        // reservations
        json jreserv = json::array();
        auto reservations = reservationRepo->listAll();
        for (auto &r : reservations)
        {
            json jr;
            jr["reservationId"] = r->getId();
            jr["passengerId"] = r->getPassengerId();
            jr["flightNumber"] = r->getFlightNumber();
            jr["seatRow"] = r->getSeatRow();
            jr["seatCol"] = string(1, r->getSeatCol());
            jr["status"] = r->getStatus();
            jr["price"] = r->getPrice();
            jr["checkedIn"] = r->isCheckedIn();
            jr["gate"] = r->getGate();
            jr["boardingTime"] = r->getBoardingTime();
            jreserv.push_back(jr);
        }
        {
            ofstream f(path + "/reservations.json");
            f << jreserv.dump(2);
        }

        // payments
        json jpayments = json::array();
        auto payments = paymentRepo->listAll();
        for (auto &p : payments)
        {
            json jp;
            jp["paymentId"] = p->getPaymentId();
            jp["reservationId"] = p->getReservationId();
            jp["amount"] = p->getAmount();
            jp["method"] = static_cast<int>(p->getMethod());
            jp["succeeded"] = p->isSucceeded();
            jp["transactionRef"] = p->getTransactionRef();
            jpayments.push_back(jp);
        }
        {
            ofstream f(path + "/payments.json");
            f << jpayments.dump(2);
        }

        cout << "[DataManager] Saved data to " << path << "\n";
        return true;
    }
    catch (const exception &ex)
    {
        cerr << "[DataManager] saveAll failed: " << ex.what() << "\n";
        return false;
    }
}

bool DataManager::loadAll(const string &path)
{
    try
    {
        if (!fs::exists(path))
        {
            cout << "[DataManager] loadAll: path '" << path << "' not found — nothing to load\n";
            return true;
        }

        // users
        string usersFile = path + "/users.json";
        if (fs::exists(usersFile))
        {
            ifstream f(usersFile);
            json j;
            f >> j;
            for (auto &ju : j)
            {
                int id = ju.value("id", 0);
                string username = ju.value("username", string());
                string password = ju.value("password", string()); // legacy
                string hash = ju.value("passwordHash", string());
                string salt = ju.value("salt", string());
                string type = ju.value("type", string("User"));
                bool legacy = !password.empty() && hash.empty();
                if (legacy && password == "hidden")
                {
                    if (type == "Admin")
                        password = "Admin123";
                    else if (type == "BookingAgent")
                        password = "Agent123";
                    else if (type == "Passenger")
                        password = username + "123";
                    else
                        password = "password";
                }
                if (type == "Passenger")
                {
                    string name = ju.value("name", username);
                    string contact = ju.value("contact", string());
                    auto p = make_shared<Passenger>(id, username, legacy ? password : std::string("temp"), name, contact);
                    if (!legacy)
                        p->setPasswordHashAndSalt(hash, salt);
                    int lp = ju.value("loyaltyPoints", 0);
                    if (lp > 0)
                        p->setLoyaltyPoints(lp);
                    userRepo->addUser(p);
                }
                else if (type == "Admin")
                {
                    auto a = make_shared<Admin>(id, username, legacy ? password : std::string("temp"));
                    if (!legacy)
                        a->setPasswordHashAndSalt(hash, salt);
                    userRepo->addUser(a);
                }
                else if (type == "BookingAgent")
                {
                    auto ag = make_shared<BookingAgent>(id, username, legacy ? password : std::string("temp"));
                    if (!legacy)
                        ag->setPasswordHashAndSalt(hash, salt);
                    userRepo->addUser(ag);
                }
                else
                {
                    auto u = make_shared<Passenger>(id, username, legacy ? password : std::string("temp"), username, string());
                    if (!legacy)
                        u->setPasswordHashAndSalt(hash, salt);
                    userRepo->addUser(u);
                }
            }
        }

        // flights
        string flightsFile = path + "/flights.json";
        if (fs::exists(flightsFile))
        {
            ifstream f(flightsFile);
            json j;
            f >> j;
            for (auto &jf : j)
            {
                string flightNumber = jf.value("flightNumber", string());
                string origin = jf.value("origin", string());
                string destination = jf.value("destination", string());
                string departureTime = jf.value("departureTime", string());
                string arrivalTime = jf.value("arrivalTime", string());
                int statusInt = jf.value("status", 0);
                FlightStatus status = static_cast<FlightStatus>(statusInt);
                json jac = jf.value("aircraft", json::object());
                shared_ptr<Aircraft> aircraft = nullptr;
                if (!jac.is_null())
                {
                    string reg = jac.value("regNumber", string("REG"));
                    string model = jac.value("model", string());
                    int totalSeats = jac.value("totalSeats", 0);
                    aircraft = make_shared<Aircraft>(reg, model, totalSeats);
                }
                auto flight = make_shared<Flight>(flightNumber, origin, destination, departureTime, arrivalTime, aircraft);
                flight->updateStatus(status);
                if (jf.contains("seats"))
                {
                    flight->importSeatMapFromPersistence(jf["seats"]);
                }
                flightRepo->addFlight(flight);
            }
        }

        // reservations
        string reservationsFile = path + "/reservations.json";
        if (fs::exists(reservationsFile))
        {
            ifstream f(reservationsFile);
            json j;
            f >> j;
            for (auto &jr : j)
            {
                int reservationId = jr.value("reservationId", 0);
                int passengerId = jr.value("passengerId", 0);
                string flightNumber = jr.value("flightNumber", string());
                int seatRow = jr.value("seatRow", 0);
                string seatColS = jr.value("seatCol", string("A"));
                char seatCol = seatColS.empty() ? 'A' : seatColS[0];
                double price = jr.value("price", 0.0);
                auto r = make_shared<Reservation>(reservationId, passengerId, flightNumber, seatRow, seatCol, price);
                string status = jr.value("status", string("Confirmed"));
                if (status == "Cancelled")
                    r->cancel();
                bool checkedIn = jr.value("checkedIn", false);
                string gate = jr.value("gate", string());
                string boardingTime = jr.value("boardingTime", string());
                if (checkedIn)
                    r->markCheckedIn(gate, boardingTime);
                reservationRepo->addReservation(r);
                // mark seat as occupied in flight seatMap
                auto flight = flightRepo->findByNumber(flightNumber);
                if (flight)
                    flight->reserveSeat(seatRow, seatCol, reservationId);
            }
        }

        // payments
        string paymentsFile = path + "/payments.json";
        if (fs::exists(paymentsFile))
        {
            ifstream f(paymentsFile);
            json j;
            f >> j;
            for (auto &jp : j)
            {
                int paymentId = jp.value("paymentId", 0);
                int reservationId = jp.value("reservationId", 0);
                double amount = jp.value("amount", 0.0);
                int methodInt = jp.value("method", 0);
                PaymentMethod pm = static_cast<PaymentMethod>(methodInt);
                bool succeeded = jp.value("succeeded", true);
                string tx = jp.value("transactionRef", string());
                auto p = make_shared<Payment>(paymentId, reservationId, amount, pm);
                if (succeeded)
                {
                    p->processPayment();
                    if (!tx.empty())
                        p->setTransactionRef(tx);
                }
                paymentRepo->addPayment(p);
            }
        }
        return true;
    }
    catch (const exception &ex)
    {
        cerr << "[DataManager] loadAll failed: " << ex.what() << "\n";
        return false;
    }
}