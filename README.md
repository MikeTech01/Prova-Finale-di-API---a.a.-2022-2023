# Final Project of Algorithms and Principles of Informatics (year: 2022-2023)

## Overview

This project implements a routing system for vehicles on a highway, aiming to find the optimal path between service stations and according to the specifics.

### Key Features

- Management of service stations with vehicle fleets
- Route planning with autonomy constraints
- Finding routes with the minimum number of stops
- Resolving equally optimal routes based on station distances

## System Rules

### Station Constraints
- Each station is identified by a unique distance from the highway start
- Maximum of 512 vehicles per station
- Each vehicle has an autonomy expressed in kilometers

### Travel Rules
- Travel always proceeds forward (backtracking is not allowed)
- A new vehicle is rented at each stop
- A subsequent stop must be reachable using the vehicle's autonomy from the previous station

## Supported Commands

### 1. Add Station
```
add-station <distance> <number-of-cars> <car-autonomy-1> ... <car-autonomy-n>
```
- Adds a station at the specified distance
- Fills the vehicle fleet with the indicated autonomies
- Output: "added" or "not added"

### 2. Demolish Station
```
demolish-station <distance>
```
- Removes the station at the specified distance
- Output: "demolished" or "not demolished"

### 3. Add Vehicle
```
add-car <station-distance> <car-autonomy>
```
- Adds a vehicle to the specified station
- Output: "added" or "not added"

### 4. Scrap Vehicle
```
scrap-car <station-distance> <car-autonomy>
```
- Removes a vehicle with the specified autonomy from the station
- Output: "scrapped" or "not scrapped"

### 5. Plan Route
```
plan-route <departure-station-distance> <arrival-station-distance>
```
- Calculates the optimal path between two stations
- Output: Distances of stations in travel order
- If no path is possible: "no route"

## Route Selection Criteria

1. Minimum number of stops
2. In case of equivalent routes, prefers the one with stations at shorter distances

## Example

### Sample Input
```
add-station 20 3 5 10 15
add-station 30 2 25 40
add-station 45 1 30
add-station 50 2 20 25
plan-route 20 50
```

### Expected Output
```
20 30 50
```

## Implementation Notes
- All integer values are encodable in 32 bits
- The system does not modify stations during route planning

## Data Structures Implemented
- Red and Black Tree
- Double Linked List
- LIFO Stack

## Academic Achievement

### Exam Result
- **Final Grade:** 30 cum laude
- **Course:** Algorithms and Principles of Informatics
- **Academic Year:** 2022-2023
- **Project Type:** Final Exam Project

---
## Contact

- **Email:** [michael.alibeaj@mail.polimi.it]
- **GitHub Profile:** [MikeTech01]

## Requirements
- Programming Language: C
