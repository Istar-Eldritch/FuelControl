# CONFIGURATION

In the profiles folder you'll find some default configurations. Only change the configurations manually when the server is not runing. Otherwise you can update the configurations using the COT GUI while in game.

## `settings.json`

This file is used to change general settings and toggle features on and off:
```js
{
    "administrators": [], // A list of steam ids, separated by commas, allows for usage of the administration commands.
    "spawn_enabled": true, // If the automated spawn is enabled.
    "spawn_interval": 1800, // How often the spawn task runs in seconds. Defaults to 30 min
    "spawn_amount": 1, // How much fuel in liters the spawn task spawns. Defaults to 1. 
    "spawn_minimum" 10, // During spawning, what is the minimum amount of fuel in liters that should be added to a station (if available).
    "pump_car_refueling": 1, // Enables refueling a car if its close to a fuel pump. 1 = enabled, 0 = disabled
    "pump_barrel_refueling": 1, // Enables refueling a barrel if its close to a fuel pump. 1 = enabled, 0 = disabled
    "siphoning": 1, // Enables siphoning fuel from cars. 1 = enabled, 0 = disabled.
    "siphoning_limit": 98, // The percentage of fuel that is possible to siphon out of a car fuel tank.
    "default_pumps_have_fuel": 0, // The behaviour of pumps that don't belong to a station. If set to 1, this pumps will have infinite fuel. Defaults to  0.
    "pumps_get_ruined": 1, // If a pump gets ruined it won't produce fuel.
    "measure_fuel_with_stick": 1 // If enabled, you can measure the fuel of cars and pumps with a long stick
}
```

## `stations.json`
This file is used to describe the fuel availability in each of the stations. Each station configuration looks like the following:
```js
{
    "id": "A0D594F949619811C1F2C2D27FF71785702D9310465B583EB25AE3D27ABD888B", // Uniquie identifier, can be anything as long as its unique. Will be generated if created with the UI.
    "x": 314.600555, // x coordinate
    "y": 9390.578125, // y coordinate
    "name": "Tri Kresta", // helper name. Used for logging and identification of the station.
    "fuel": -1, // Defines how much fuel is available in this station. -1 means infinite. Set it to 0 to disable the station.
    "capacity": -1 // Defines how much fuel this station can hold. -1 means infinite. This property is not used at the moment.
}
```
The provided `stations.json` is an example configuration for the Chernarous map. If you are not using Chernarous, you'll have to create a file similar to the one provided with the locations of the fuel stations you want to configure.

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

## `vehicle_autonomy.json`
This file defines the autonomy in Km a vehicle has with a full tank of fuel when driving at an average RPM. Driving at lower RPMs will increase autonomy while driving more aggresively will decrease it.
If set to 0, the custom atonomy logic won't take place and instead the vanilla consumption rates will happen.
```js
{
    "OffroadHatchback_Blue": 25.0,
    "OffroadHatchback": 25.0,
    "Sedan_02": 25.0,
    "Sedan_02_Red": 25.0,
    "Truck_01_Covered_Blue": 25.0,
    "Truck_01_Covered_Orange": 25.0,
    "CivilianSedan_Black": 25.0,
    "CivilianSedan": 25.0,
    "CivilianSedan_Wine": 25.0,
    "Sedan_02_Grey": 25.0,
    "Hatchback_02": 25.0,
    "Truck_01_Covered": 25.0,
    "OffroadHatchback_White": 25.0,
    "Hatchback_02_Blue": 25.0,
    "Hatchback_02_Black": 25.0,
    "Offroad_02": 25.0
}
```