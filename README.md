# FuelControl

This mod provides tools to control your sever's fuel economy.

## Features:

- Enable / Disable fuel stations
- Set fuel limit per station.
- Refuel a car directly if its placed close to a fuel pump.
- Refuel a barrel directly if its placed close to a fuel pump.
- Siphon fuel out of a car fuel tank.
- Configurable liquid transfer rates for arbitrary containers (using class names)
- Configurable consumption rates for arbitrary vehicles (using class names)

All this features are configurable.

## Commands

### `#fc addstation [x] [y] <station name>`
Adds a station group at x and y position with the given name. If x and y are not provided, it will add an station at the player position.

```
#fc addstation 13123 12314 "Cherno North"
```

```
#fc addstation "Cherno North"
```

### `#fc delstation [station name]`
Deletes the station group with the given name. If the name is not provided not provided, it will delete the station at the player position.

```
#fc delstation "Cherno North"
```

```
#fc addstation
```

### `#fc setfuel [station name] <value>`
Sets the fuel of a station to the given value given in Liters.

```
#fc setfuel "Cherno North" 1000
```

```
#fc setfuel 1000
```

### `#fc getfuel [station name]`
Returns the fuel available at the station. If no name is provided it will use the player location.

```
#fc getfuel "Cherno North"
```

```
#fc getfuel
```

## Configuration:
In the profiles folder you'll find some default configurations.

## `settings.json`

This file is used to change general settings and toggle features on and off:
```js
{
    "administrators": [], // A list of steam ids, separated by commas, allows for usage of the administration commands.
    "pump_car_refueling": 1, // Enables refueling a car if its close to a fuel pump. 1 = enabled, 0 = disabled
    "pump_barrel_refueling": 1, // Enables refueling a barrel if its close to a fuel pump. 1 = enabled, 0 = disabled
    "siphoning": 1, // Enables siphoning fuel from cars. 1 = enabled, 0 = disabled.
    "siphoning_limit": 98, // The percentage of fuel that is possible to siphon out of a car fuel tank.
}
```

## `stations.json`
This file is used to describe the fuel availability in each of the stations. Each station configuration looks like the following:
```js
{
    "x": 314.600555, // x coordinate
    "y": 9390.578125, // y coordinate
    "name": "Tri Kresta", // helper name, must be unique. Used for logging and identification of the station.
    "fuel": -1, // Defines how much fuel is available in this station. -1 means infinite. Set it to 0 to disable the station.
    "capacity": -1 // Defines how much fuel this station can hold. -1 means infinite. This property is not used at the moment.
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

## `sonsumption_rates.json`
This file defines the consumption rates of vehicles. Consumptions are set by default to 1. For example you could set 0.5 to half the consumption or 3 to triplicate it.
```js
{
    "CivilianSedan": 1,
    "CivilianSedan_Black": 1,
    "CivilianSedan_Wine": 1,
    "Hatchback_02": 1,
    "Hatchback_02_Blue": 1,
    "Hatchback_02_Black": 1,
    "OffroadHatchback": 1,
    "OffroadHatchback_Blue": 1,
    "OffroadHatchback_White":1,
    "Sedan_02": 1,
    "Sedan_02_Red": 1,
    "Sedan_02_Grey": 1,
    "Truck_01_Covered": 1,
    "Truck_01_Covered_Blue": 1,
    "Truck_01_Covered_Orange": 1
}
```