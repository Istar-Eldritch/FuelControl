# FuelControl

This mod provides tools to control your sever's fuel economy.

## Features:

- Enable / Disable fuel stations
- Set fuel limit per station.
- Refuel a car directly if its placed close to a fuel pump.
- Refuel a barrel directly if its placed close to a fuel pump.
- Stations can provide Gasoline, Diesel and Aviaton Fuel (avgas).
- Siphon fuel out of a car fuel tank.
- Disable / Enable being able to use fuel pumps that were damaged / exploded.
- Automatically spawn fuel on configured stations.
- Configurable liquid transfer rates for arbitrary containers (using class names)
- Configurable consumption rates (autonomy) per vehicles (using class names)
- Configurable required fuel type per vehicle, GASOLINE, DIESEL or AVGAS.
- Measure the fuel in a car or fuel pump with a long stick.
- Configuration to require stations to be powered with electricity (Requires battery with wire / generator & cable reel)
- Configuration for how many energy units are consumed per liter of fuel.
- Administration UI (If COT is installed)

All this features are configurable. Check Configuration or use the COT UI.

## Dependencies

- Community Framework: https://steamcommunity.com/sharedfiles/filedetails/?id=1559212036
- [OPTIONAL for Admin UI] Commmunity Online tools: https://steamcommunity.com/sharedfiles/filedetails/?id=1564026768

## Mod compatibility

If you are a modder and you want to detect if FuelControl is installed, the mod defines `IE_FC`. You can check it using #ifdef.

## Mod support:

On discord server: https://discord.gg/3BH6HeMBas

## Configuration:

Check [configuration.md](https://github.com/Istar-Eldritch/FuelControl/blob/main/configuration.md)
You can some example configurations [HERE](https://github.com/Istar-Eldritch/FuelControl/tree/main/profiles/FuelControl)

## Building the mod.

Run `./workspace/build.sh`
Run `AddonBuilder` from DayZ-Tools
    - Configure it to include `.paa`
    - Configure it to sign the pbo.
    - Use FuelControl/mod as the source with `FuelControl` as prefix.