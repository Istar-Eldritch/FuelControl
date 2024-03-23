# CONFIGURATION

In the profiles folder you'll find some default configurations. Only change the configurations manually when the server is not runing. Otherwise you can update the configurations using the COT GUI while in game.

## `settings.json`

This file is used to change general settings and toggle features on and off:
```js
{
    "spawn_enabled": true, // If the automated spawn is enabled.
    "spawn_interval": 1800, // How often the spawn task runs in seconds. Defaults to 30 min
    "spawn_amount": 1, // How much fuel in liters the spawn task spawns. Defaults to 1. 
    "spawn_minimum": 10, // During spawning, what is the minimum amount of fuel in liters that should be added to a station (if available).
    "pump_car_refueling": 1, // Enables refueling a car if its close to a fuel pump. 1 = enabled, 0 = disabled
    "pump_barrel_refueling": 1, // Enables refueling a barrel if its close to a fuel pump. 1 = enabled, 0 = disabled
    "siphoning": 1, // Enables siphoning fuel from cars. 1 = enabled, 0 = disabled.
    "siphoning_limit": 98.0, // The percentage of fuel that is possible to siphon out of a car fuel tank.
    "default_pumps_have_fuel": 0, // The behaviour of pumps that don't belong to a station. If set to 1, this pumps will have infinite fuel. Defaults to  0.
    "pumps_get_ruined": 1, // If a pump gets ruined it won't produce fuel.
    "measure_fuel_with_stick": 1, // If enabled, you can measure the fuel of cars and pumps with a long stick
    "pumps_require_electricity": 1, // If enabled, player need to power the powerboxes in order to operate the pumps.
    "pump_consumption": 10 // The amount of electriciy used by the pumps per L of fuel consumed.
}
```

## `stations_config.json`
This file is used to define the location and the fuel capacity of each station. All pumps in a 100m radious around the station will be identified as belonging to the station.


```js
[
    {
        "id": "8A2F7F8CB2AEE0F67C82C2CD7499F80D340E59EBA443B6D7107D144249970E85", // Uniquie identifier, can be anything as long as its unique. Will be generated if created with the UI.
        "x": 3000.45751953125, // x coordinate
        "y": 7482.8154296875, // y coordinate
        "name": "Pustoshka", // helper name. Used for logging and identification of the station.
        "fuels": {
            "GASOLINE": {
                "capacity": -1.0 // Defines how much fuel this station can hold. -1 means infinite. During a spawn event, no more fuel will be added to a station if it has reached its capacity.
            },
            "AVGAS": {
                "capacity": -1.0
            },
            "DIESEL": {
                "capacity": -1.0
            }
        }
    },
    ...
]
```
The provided `stations_config.json` is an example configuration for the Chernarous map. If you are not using Chernarous, you'll have to create a file similar to the one provided with the locations of the fuel stations you want to configure.

## `stations_state.json`
This file defines the state of the stations. This file is updated by the mod while the game is running. Never update this file manually while the server is running or its likely to cause corruption.

```js
[
    {
        "id": "8A2F7F8CB2AEE0F67C82C2CD7499F80D340E59EBA443B6D7107D144249970E85", // The unique identifier of the station, as defined in `sations_config.json`.
        "fuels": {
            "GASOLINE": {
                "quantity": 0.0 // How much fuel is available for consumption at the station. Once this reaches 0, players won't be able to take fuel out of the station.
            },
            "AVGAS": {
                "quantity": 0.0
            },
            "DIESEL": {
                "quantity": 0.0
            }
        }
    },
]
```

## `power_boxes.json`
This file is used to describe the fuel availability in each of the stations. Each station configuration looks like the following:
```js
{
    "id": "A0D594F949619811C1F2C2D27FF71785702D9310465B583EB25AE3D27ABD888B", // Uniquie identifier, can be anything as long as its unique. Will be generated if created with the UI.
    "x": 4530.330078125, // x coordinate
    "y": 9699.150390625, // y coordinate
    "orientation": 60.0900993347168, // orientation / rotation of the model.
    "name": "Airport" // A helper name to identify the station.
}
```
The provided `stations.json` is an example configuration for the Chernarous map. If you are not using Chernarous, you'll have to create a file similar to the one provided with the locations of the fuel stations you want to configure.

## `liquid_transfer_rates.json`
This is a map of `{classname(string): float}`, defines the transfer rate of containers. When transfering between two different containrs, it will use the slower rate. Transfer rates are in ml/s.
```js
{
    "Land_FuelStation_Feed": 1000.0,

    "CivilianSedan": 500.0,
    "CivilianSedan_Black": 500.0,
    "CivilianSedan_Wine": 500.0,
    "Hatchback_02": 500.0,
    "Hatchback_02_Blue": 500.0,
    "Hatchback_02_Black": 500.0,
    "OffroadHatchback": 500.0,
    "OffroadHatchback_Blue": 500.0,
    "OffroadHatchback_White": 500.0,
    "Sedan_02": 500.0,
    "Sedan_02_Red": 500.0,
    "Sedan_02_Grey": 500.0,
    "Truck_01_Covered": 500.0,
    "Truck_01_Covered_Blue": 800.0,
    "Truck_01_Covered_Orange": 800.0,
    "Offroad_02": 800.0,

    "Barrel_Blue": 1000.0,
    "Barrel_Green": 1000.0,
    "Barrel_Red": 1000.0,
    "Barrel_Yellow": 1000.0,

    "CanisterGasoline": 500.0,
    "Canteen": 100.0,
    "Cauldron": 500.0,
    "Pot": 300.0,
    "WaterBottle": 100.0
  }
```

## `vehicle_config.json`
This file defines the autonomy in Km a vehicle has with a full tank of fuel when driving at an average RPM. Driving at lower RPMs will increase autonomy while driving more aggresively will decrease it.
If set to 0, the custom atonomy logic won't take place and instead the vanilla consumption rates will happen.
It also defines which fuel the vehicle uses. Options are `GASOLINE`, `DIESEL` and `AVGAS`.
```js
{
    "OffroadHatchback_Blue": {
        "autonomy": 25.0,
        "fuel_type": "GASOLINE"
    },
    "OffroadHatchback": {
        "autonomy": 25.0,
        "fuel_type": "GASOLINE"
    },
    "Sedan_02": {
        "autonomy": 25.0,
        "fuel_type": "GASOLINE"
    },
    ...
}
```