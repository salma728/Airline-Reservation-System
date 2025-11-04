CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Wextra
SRC = src/Admin.cpp src/Aircraft.cpp src/BookingAgent.cpp src/DataManager.cpp src/Flight.cpp src/FlightRepository.cpp src/IdGenerator.cpp src/MaintenanceRecord.cpp src/Passenger.cpp src/Payment.cpp src/PaymentRepository.cpp src/Reservation.cpp src/ReservationRepository.cpp src/Seat.cpp src/Security.cpp src/SystemFacade.cpp src/User.cpp src/UserRepository.cpp src/BoardingPass.cpp main.cpp
BIN = AirlineSystem

all: build

build: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(SRC)

run: build
	./$(BIN)

clean:
	- del /Q $(BIN) 2> NUL || true
	- del /Q *.o 2> NUL || true
	- del /Q src\*.o 2> NUL || true
