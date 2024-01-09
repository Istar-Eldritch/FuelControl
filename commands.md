# COMMANDS

These commands come in handy if you don't have COT installed. If you do, using the GUI is the recommended way of doing aministration.

### `#fc station add [--pos x,y] <station name>`
Adds a station group at x and y position with the given name. If x and y are not provided, it will add an station at the player position.

```
#fc station add --pos 13123,12314 "Chernogorsk East"
```

```
#fc station add "Chernogorsk East"
```

### `#fc station del [station name]`
Deletes the station group with the given name. If the name is not provided not provided, it will delete the station at the player position.

```
#fc station del "Chernogorsk East"
```

```
#fc station del
```

### `#fc station info [station name]`
Returns the fuel available at the station. If no name is provided it will use the player location.

```
#fc station info "Cherno North"
```

```
#fc station info
```

### `#fc station edit [--fuel x] [--capacity y] [--name z] [station name]`
Updates a station. Sets the fuel and capacity lin liters and updates the name.

```
#fc station edit --fuel 1000 --name Cherno "Chernogorsk East"
```

```
#fc station edit --fuel 1000
```

### `#fc station list [--min-fuel x] [--max-fuel y] [search on name]`
Lists the stations that match the search criteria.

```
#fc station list Cherno
```

```
#fc station list --min-fuel 10 --max-fuel 100
```


### `#fc status`
Shows some general metrics about the fuel across all stations, currently:
- Total fuel available.
- Total fuel capacity.
- Percentage of the reserves of fuel. (capacity / available)

```
#fc status
```

### `#fc spawn <amount>`
Spreads the provided amount in liters randomly across all the stations with available capacity.
- The fuel amount must be an integer, if a decimal point its provided it will be rounded down.
- This functionality is affected by the `spawn_minimum` setting from `settings.json`
- This functionality ignores stations with unlimited fuel.

```
#fc spawn 112
```