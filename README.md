# FuelControl

This mod provides tools to control your sever's fuel economy.

## Features:

- Enable / Disable fuel stations
- Set fuel limit per station.
- Refuel car directly if its placed close to a fuel pump.

## Configuration:
In the profiles folder you'll find an example configuration for the Chernarous map. If you are not using Chernarous, you'll have to create a file similar to the one provided with the locations of the fuel stations you want to configure.


### Structure of the configuration file

```js
{
    "pump_refueling": 1 // Enables refueling a car if its close to a fuel pump. 1 = enabled, 0 = disabled
    "siphoning": 1 // Enables siphoning fuel from cars. 1 = enabled, 0 = disabled.
    "stations": [] // An array of StationConfig. Describes the configurations for each one of the stations.
}
```

Each one of the fuel **StationConfig** looks like this:
```js
      {
        "x": 314.600555, // x coordinate
        "y": 9390.578125, // y coordinate
        "name": "Tri Kresta", // helper name, must be unique. Used for logging and identification of the station.
        "fuel": -1, // Defines how much fuel is available in this station. -1 means infinite. Set it to 0 to disable the station.
        "capacity": -1 // Defines how much fuel this station can hold. -1 means infinite. This property is not used at the moment.
      }
```